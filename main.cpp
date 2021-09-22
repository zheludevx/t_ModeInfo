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

bool checkSystem (std::string &s)
{
    bool bRes = false;
    unsigned int system = s.find("system");
    std::cout << "Подстрока найдена в позиции "<< system << std::endl;
    if (system == 0)
    {
        std::cout << "Данная подстрока подходит" << std::endl << std::endl;
        bRes = true;
    }
    else
        std::cout << "Данная подстрока не подходит" << std::endl << std::endl;
    return bRes;
}

bool checkXml (std::string &s)
{
    bool bRes = false;
    unsigned int xml = s.find(".xml");
    std::cout << "Индекс позиции расширения "<< xml << std::endl;
    if ((xml + 4) == s.length())
    {
        std::cout << "Данная подстрока подходит" << std::endl << std::endl;
        bRes = true;
    }
    else
        std::cout << "Данная подстрока не подходит" << std::endl << std::endl;
    return bRes;
}

bool checkRxml(std::string &s)
{
    bool bRes = false;
    unsigned int rxml = s.find(".xml");
    std::cout << "Индекс r позиции расширения "<< rxml << std::endl;
    if (rxml  == (s.length() - 4 ))
    {
        std::cout << "Данная подстрока подходит" << std::endl << std::endl;
        bRes = true;
    }
    else
        std::cout << "Данная подстрока не подходит" << std::endl << std::endl;
    return bRes;
}

bool sVivod (std::string &s)
{
    bool bRes = false;
    std::string sSubstr = s.substr(s.length() - 4);
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
    std::cout << std::endl << std::endl;
    std::string str = getenv("NITAETC");
    std::vector<std::string> s;
    boost::filesystem::directory_iterator begin(str), end;
    try
    {
        for ( ; begin != end; ++begin)
            s.push_back(begin->path().filename());
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception: " << e.what();
        return 1;
    }
    for (unsigned int i = 0; i < s.size(); i++)
    {
        std::cout << s[i] << std::endl;
        checkSystem (s[i]);
        checkXml (s[i]);
        checkRxml (s[i]);
        sVivod (s[i]);
    }
    lib.Free();
    return 0;
}
