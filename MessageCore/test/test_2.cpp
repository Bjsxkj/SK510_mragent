
#include <iostream>
#include <memory.h>

#include "MessageCoreApi.h"

using namespace imimr;
using namespace mrcore;
using namespace std;

int main(int argc, char** argv){

    ImiBodySkeletonData skel;
    memset(&skel, 0, sizeof(skel));

    cout<<"len="<<sizeof(skel)<<endl;

    cout << skel << endl;

    return 0;
}

