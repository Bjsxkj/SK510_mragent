#ifdef MESSAGECORE
#include <chrono>
#include <ctime>
#include <random>
#include <sstream>
#include <iomanip>

std::string generate_unique_name() {
    // Get the current time
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 10000;

    // Generate a random number
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(1000, 9999);
    int random_number = distribution(generator);

    // Convert random number and milliseconds to string using stringstream
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(3) << now_ms.count();
    std::string ms_str = ss.str();
    ss.str(""); // Clear the stringstream
    ss << random_number;
    std::string random_number_str = ss.str();

    // Combine time string with milliseconds and random number
    std::string unique_name = "imi_" + ms_str + random_number_str;

    return unique_name;
}

// Function to generate a filename with an incremented number
std::string generate_filename(const std::string& baseName, const std::string& extension) {
    static int counter = 1; // Static variable to maintain the counter
    std::string filename = baseName + std::to_string(counter) + extension;
    counter++;
    return filename;
}

// Function to write a PPM image
void write_ppm(const char *filename, int width, int height, unsigned char *data) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        perror("Failed to open file for writing");
        exit(EXIT_FAILURE);
    }

    // Write the PPM header
    fprintf(fp, "P3\n%d %d\n255\n", width, height);

    // Write the pixel data
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = (y * width + x) * 3;
            fprintf(fp, "%d %d %d ", data[index], data[index + 1], data[index + 2]);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
}

uint64_t getCurrentTimeInMs()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}
#endif

#ifdef MRAGENT

#include "utils.h"
#include <chrono>
#include <cstdint>
uint64_t getCurrentTimeInMs(){
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

#ifdef ANDROID
extern "C" JNIEXPORT jobject JNICALL Java_com_sximi_mragent_IImiColorFrame_toByteBuffer(JNIEnv *env, jobject obj, jlong address, jint length) {
    if (address == 0 || length <= 0) {
        return nullptr; // Return null if address is invalid or length is zero
    }
    return env->NewDirectByteBuffer((void*)address, length);
}
#endif
#endif
