#include <ModeInfo.h>
#include <vector>
#include <string>
#include <stdlib.h>
#include <cstdio>
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
    boost::filesystem::directory_iterator begin(str), end;
    std::cout << std::endl << "$NITAETC groups:" << std::endl;
    for ( ; begin != end; ++begin)
    {
        if (begin->path().extension() == ".xml")
        {
            if (argc == 1)
            {
                if(argv[0] == "system-msk.xml")
                {
                    std::cout << "Set \"system-msk.xml\" as system.xml? (y/n)" << std::endl;
                    std::string x;
                    if (x == "y")
                    {
                        try
                        {
                            boost::filesystem::path from =
                                    complete(boost::filesystem::path("system-msk.xml", boost::filesystem::native));
                            boost::filesystem::path here =
                                    complete(boost::filesystem::path("system.xml", boost::filesystem::native));
                            copy_file(from, here);
                        }
                        catch (std::exception& e)
                        {
                            std::cerr <<e.what() << std::endl;
                        }
                        char oldfilename[] = "system.xml";
                        char newfilename[] = "system.xml.bak";
                        if ( rename(oldfilename, newfilename) == 0)
                            continue;
                        else
                            std::cout << "rename error" << std::endl;
                    }

                    if (x == "n")
                        std::cout << "Operation canceled." << std::endl;
                }
            }
            else
                std::cout << "ERR> Wrong command line" << std::endl;

            std::cout << *begin << std::endl;
        }
    }
    lib.Free();
    return 0;
}
