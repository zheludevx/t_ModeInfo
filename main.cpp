#include <ModeInfo.h>
#include <iostream>
#include <vector>
#include <string>

int main(int argc, char* argv[])
{
    plug_key::CModeInfoPlug lib;
    bool bRes = lib.Load();
    std::cout << "RES>" << bRes << std::endl;
    std::vector<std::string> arr;
    arr.push_back("%NITAROOT%");
    arr.push_back("[GROUP]");
    arr.push_back("[CFG]");
    arr.push_back("[PRODUCT]");
    arr.push_back("[MODE]");
    arr.push_back("[LANG]");
    arr.push_back("[COMPUTER]");
    arr.push_back("[UID]");
    std::string sText =
         "%%NITAROOT%%"
         "/[GROUP]"
         "/[CFG]"
         "/[PRODUCT]"
         "/[MODE]"
         "/[LANG]"
         "/[COMPUTER]"
         "/[UID]";
    for (unsigned int i = 0; i < arr.size(); i++)
    {
    std::cout << arr[i] << " = ";
    lib.ExpandString(arr[i]);
    std::cout<< "\"" << arr[i] << "\"" << std::endl;
    }
    lib.Free();
    return 0;
}
