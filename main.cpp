#include <ModeInfo.h>
#include <vector>
#include <string>
#include <stdlib.h>
#include <cstdio>
#include <exception>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/path.hpp>
#include <iostream>

bool checkSystem (const std::string& sFileName)
{
    bool bRes = false;
    size_t sztSystem = sFileName.find("system");
    if (sztSystem != std::string::npos)
    {
        if (sztSystem == 0)
            bRes = true;
    }
    return bRes;
}

bool checkXml (const std::string& sFileName)
{
    bool bRes = false;
    size_t sztXml = sFileName.find(".xml");
    if (sztXml != std::string::npos)
    {
        if ((sztXml + 4) == sFileName.length())
            bRes = true;
    }
    return bRes;
}

int main(int argc, char* argv[])
{
    plug_key::CModeInfoPlug lib;
    bool bRes = lib.Load();
    std::cout << "RES>" << bRes << std::endl << std::endl;
    std::vector<std::string> vArr;
    vArr.push_back("%NITAROOT%");
    vArr.push_back("[GROUP]");
    vArr.push_back("[CFG]");
    vArr.push_back("[PRODUCT]");
    vArr.push_back("[MODE]");
    vArr.push_back("[LANG]");
    vArr.push_back("[COMPUTER]");
    vArr.push_back("[UID]");
    std::string sText =
            "%%NITAROOT%%"
         "/[GROUP]"
         "/[CFG]"
         "/[PRODUCT]"
         "/[MODE]"
         "/[LANG]"
         "/[COMPUTER]"
         "/[UID]";
    for (unsigned int i = 0; i < vArr.size(); i++)
    {
        std::cout << vArr[i] << " = ";
        lib.ExpandString(vArr[i]);
        std::cout<< "\"" << vArr[i] << "\"" << std::endl;
    }
    std::cout << std::endl << std::endl;
    std::string sEnvir = getenv("NITAETC");
    std::vector<std::string> vFileName;
    boost::filesystem::directory_iterator it(sEnvir), end;
    try
    {
        for ( ; it != end; ++it)
            vFileName.push_back(it->path().filename());
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception: " << e.what();
        return 1;
    }
    for (unsigned int i = 0; i < vFileName.size(); i++)
    {
        if (checkSystem(vFileName[i]) && checkXml(vFileName[i]))
            std::cout << vFileName[i] << std::endl;
    }
    lib.Free();
    return 0;
}
