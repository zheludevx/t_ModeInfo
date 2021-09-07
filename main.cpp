#include <ModeInfo.h>
#include <iostream>


int main(int argc, char* argv[])
{
    plug_key::CModeInfoPlug lib;
    bool bRes = lib.Load();
    std::cout << "RES>" << bRes << std::endl;
    std::string sText =
         "%%NITAROOT%%"
         "/[GROUP]"
         "/[CFG]"
         "/[PRODUCT]"
         "/[MODE]"
         "/[LANG]"
         "/[COMPUTER]"
         "/[UID]";
    lib.ExpandString( sText );
    std::cout << sText << std::endl;
    lib.Free();
    return 0;
}
