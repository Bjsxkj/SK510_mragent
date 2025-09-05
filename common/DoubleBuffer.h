#include <atomic>
#include <vector>

class DoubleBuffer
{
public:
    explicit DoubleBuffer(std::size_t capacity)
        : capacity_(capacity)
    {
        // Allocate two buffers of the same capacity
        buffers_[0].resize(capacity_);
        buffers_[1].resize(capacity_);

        // Initialize sizes to 0
        sizes_[0].store(0, std::memory_order_relaxed);
        sizes_[1].store(0, std::memory_order_relaxed);

        // Start with frontIndex = 0
        frontIndex_.store(0, std::memory_order_relaxed);
    }

    /**
     * Produce (write) raw bytes into the "back" buffer.
     * Returns true if successful, false if length > capacity.
     */
    bool write(const uint8_t *data, std::size_t length, int frameNum)
    {
        // Ensure we do not exceed capacity
        if (length > capacity_)
        {
            std::cerr << "Error: Not enough capacity ("
                      << length << " > " << capacity_ << ")\n";
            return false;
        }

        // Determine which buffer is currently front
        int oldFront = frontIndex_.load(std::memory_order_acquire);
        int back = 1 - oldFront; // The other buffer is "back"

        // Write into back buffer
        std::memcpy(buffers_[back].data(), data, length);
        // Update size of the back buffer
        sizes_[back].store(length, std::memory_order_release);

        // Flip front index to point to the back
        frontIndex_.store(back, std::memory_order_release);

        frameNums_[back] = frameNum;
        return true;
    }

    /**
     * Consume (read) bytes from the "front" buffer into outBuffer.
     *  - outBuffer: memory where data should be copied
     *  - maxLength: max bytes to read
     * Returns the number of bytes actually read.
     */
    std::size_t read(uint8_t *outBuffer, std::size_t maxLength)
    {
        // Which buffer is front right now?
        int currentFront = frontIndex_.load(std::memory_order_acquire);

        // How many bytes are stored in this front buffer?
        std::size_t frontSize = sizes_[currentFront].load(std::memory_order_acquire);

        // We'll read the minimum of frontSize and maxLength
        std::size_t bytesToRead = (frontSize < maxLength) ? frontSize : maxLength;

        // Copy from the front buffer to outBuffer
        if (bytesToRead == 0)
        {
            // No data to read, return 0
            return 0;
        }
        
        std::memcpy(outBuffer, buffers_[currentFront].data(), bytesToRead);

        // Reset the front buffer size to 0 after reading
        sizes_[currentFront].store(0, std::memory_order_release);

        
        return bytesToRead; // frameNums_[currentFront];
    }

    /**
     * to see how many bytes are in the current front buffer
     */
    std::size_t getFrontSize() const
    {
        int currentFront = frontIndex_.load(std::memory_order_acquire);
        return sizes_[currentFront].load(std::memory_order_acquire);
    }

    int getFrameNum() {
        int currentFront = frontIndex_.load(std::memory_order_acquire);
        return frameNums_[currentFront];
    }

    void setFrameNum(int value) {
        int currentFront = frontIndex_.load(std::memory_order_acquire);
        frameNums_[currentFront] = value;
    }

private:
    std::size_t capacity_;

    // Two buffers of raw bytes
    std::vector<uint8_t> buffers_[2];

    // Atomic sizes for each buffer
    std::atomic<std::size_t> sizes_[2];

    // frontIndex_ is either 0 or 1; the other buffer is the "back"
    std::atomic<int> frontIndex_;

    int frameNums_[2];
};
