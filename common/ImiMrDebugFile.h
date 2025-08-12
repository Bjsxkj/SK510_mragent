#ifndef IMI_MR_DEBUG_FILE_H
#define IMI_MR_DEBUG_FILE_H

#include <fstream>
#include <string>

class ImiMrDebugFile {
public:
    ImiMrDebugFile(const std::string& filename);
    ~ImiMrDebugFile();

    void log(const char* tag, int frameNumber, int dataType, uint64_t timeStampe);
    void log(int frameNumber, int dataType, uint64_t timeStampe);

private:
    std::ofstream debugFile;
};

#endif // IMI_MR_DEBUG_FILE_H