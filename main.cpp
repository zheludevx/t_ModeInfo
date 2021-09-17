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

void checkSystem (std::string &s)
{
    std::cout << "Подстрока найдена в позиции "<< s.find("system") << std::endl;
    if (s.find("system") == 0)
        std::cout << "Данная строка подходит" << std::endl;
    else
        std::cout << "Данная строка не подходит" << std::endl;
    std::cout << std::endl;
}

void checkXml (std::string &s)
{
    std::cout << "Индекс позиции расширения "<< s.find(".xml") << std::endl;
    if (( s.find(".xml") + 4) == s.length())
        std::cout << "Данная подстрока подходит" << std::endl;
    else
        std::cout << "Данная подстрока не подходит" << std::endl;
    std::cout << std::endl;
}

void checkRxml(std::string &s)
{
    std::cout << "Индекс r позиции расширения "<< s.find(".xml") << std::endl;
    if ( s.find(".xml")  == (s.length() - 4 ))
        std::cout << "Данная подстрока подходит" << std::endl;
    else
        std::cout << "Данная подстрока не подходит" << std::endl;
    std::cout << std::endl;
}

void sVivod (std::string &s)
{
    std::cout << "Подстрока из последних 4 символов: " << "\""<< s.substr(s.length() - 4 ) << "\"" << std::endl;
    if ( s.substr(s.length() - 4 ) == ".xml"  )
        std::cout << "Данная подстрока подходит" << std::endl;
    else
        std::cout << "Данная подстрока не подходит" << std::endl;
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
    std::string s;
    getline(std::cin, s);
    checkSystem (s);
    checkXml (s);
    checkRxml(s);
    sVivod (s);
    lib.Free();
    return 0;
}
