#include <ModeInfo.h>
#include <iostream>


int main(int argc, char* argv[])
{
    plug_key::CModeInfoPlug lib;
    bool bRes = lib.Load();
    std::cout << "RES>" << bRes << std::endl;
    return 0;
}
