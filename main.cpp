#include <ModeInfo.h>
#include <vector>
#include <string>
#include <stdlib.h>
#include <cstdio>
#include <exception>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <iostream>

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
    std::string str;
    str = getenv("NITAETC");
    boost::filesystem::directory_iterator begin(str), end, begin1(str);
    std::cout << std::endl << "$NITAETC groups:" << std::endl;
    try
    {
        for ( ; begin != end; ++begin)
        {
            if (begin->path().extension() == ".xml")
                std::cout << *begin << std::endl;
        }
        std::cout << std::endl;
        for ( ; begin1 != end; ++begin1)
        {
            if ((begin1->path().string().find("system") == 19) && (begin1->path().string().find(".xml") == 29 ))
                std::cout << begin1->path() << std::endl;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "Exception: " << e.what();
        return 1;
    }
    std::cout << std::endl << std::endl;
    lib.Free();
    return 0;
}
