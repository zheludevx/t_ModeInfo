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

bool checkSystem (std::string &vStr)
{
    bool bRes = false;
    size_t sztSystem = vStr.find("system");
    if (sztSystem != std::string::npos)
    {
        std::cout << "Подстрока найдена в позиции "<< sztSystem << std::endl;
        if (sztSystem == 0)
        {
            std::cout << "Данная подстрока подходит" << std::endl << std::endl;
            bRes = true;
        }
        else
            std::cout << "Данная подстрока не подходит" << std::endl << std::endl;
        return bRes;
    }
    else
    { 
        std::cout << "Данная подстрока не найдена" << std::endl << std::endl;
        return bRes;
    }   
}

bool checkXml (std::string &vStr)
{
    bool bRes = false;
    size_t sztXml = vStr.find(".xml");
    if (sztXml != std::string::npos)
    {
        std::cout << "Индекс позиции расширения "<< sztXml << std::endl;
        if ((sztXml + 4) == vStr.length())
        {
            std::cout << "Данная подстрока подходит" << std::endl << std::endl;
            bRes = true;
        }
        else
            std::cout << "Данная подстрока не подходит" << std::endl << std::endl;
        return bRes;
    }
    else
    {
        std::cout << "Данное расширение не найдено" << std::endl << std::endl;
        return bRes;
    }
}

bool checkRxml(std::string &vStr)
{
    bool bRes = false;
    size_t sztRxml = vStr.find(".xml");
    if (sztRxml != std::string::npos)
    {
        std::cout << "Индекс r позиции расширения "<< sztRxml << std::endl;
        if (sztRxml  == (vStr.length() - 4 ))
        {
            std::cout << "Данная подстрока подходит" << std::endl << std::endl;
            bRes = true;
        }
        else
            std::cout << "Данная подстрока не подходит" << std::endl << std::endl;
        return bRes;
    }
    else
    {
     	std::cout << "Данное r расширение не найдено" << std::endl << std::endl;
        return bRes;
    }
}

bool sVivod (std::string &vStr)
{
    bool bRes = false;
    std::string sSubstr = vStr.substr(vStr.length() - 4);
    std::cout << "Подстрока из последних 4 символов: " << "\""<< sSubstr << "\"" << std::endl;
    if (sSubstr == ".xml")
    {
       std::cout << "Данная подстрока подходит" << std::endl << std::endl;
       bRes = true;
    }
    else
       std::cout << "Данная подстрока не подходит" << std::endl << std::endl;
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
    std::vector<std::string> vStr;
    boost::filesystem::directory_iterator it(sEnvir), end;
    try
    {
        for ( ; it != end; ++it)
            vStr.push_back(it->path().filename());
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception: " << e.what();
        return 1;
    }
    for (unsigned int i = 0; i < vStr.size(); i++)
    {
        std::cout << vStr[i] << std::endl;
        checkSystem (vStr[i]);
        checkXml (vStr[i]);
        checkRxml (vStr[i]);
        sVivod (vStr[i]);
    }
    lib.Free();
    return 0;
}
