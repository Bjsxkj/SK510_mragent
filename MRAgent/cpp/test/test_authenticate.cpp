
#include "ImiMrAgent.h"
#include <cstdio>
#include <fstream>
#include <vector>

using namespace std;
using namespace imimr;

int main(int argc, char **argv)
{
    ImiContext *ctx = ImiContext::create(nullptr);
    ctx->initialize("./mragent.toml");
    ImiBaseDevice dev = ctx->createDevice();
    auto result = dev->authenticate("IMIH-MRMANAGERTEST", "mrmangertest", "mrmanager20250819", "./license");
    printf("authenticate result: %s\n", result.c_str());
    return 0;
}
