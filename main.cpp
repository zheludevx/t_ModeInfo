#include <ModeInfo.h>
#include <vector>
#include <string>
#include <stdlib.h>
#include <cstdio>
#include <exception>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/program_options.hpp>
#include <boost/optional.hpp>
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

bool Rename (std::string& sFileName)
{
    bool bRes = false;
    if (sFileName == "system.xml")
    {
        std::string sReplace = "system.xml.bak";
        sFileName.replace(0, 14, sReplace);
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
    std::vector<std::string> vCommandName;
    boost::filesystem::directory_iterator it(sEnvir), end;
    try
    {
        for ( ; it != end; ++it)
            vFileName.push_back(it->path().filename().string());

        for (unsigned int i = 0; i < vFileName.size(); i++)
        {
            Rename(vFileName[i]);
            if (checkSystem(vFileName[i]) && checkXml(vFileName[i]))
                std::cout << vFileName[i] << std::endl;
        }

        boost::program_options::options_description desc("Command Parser");
        desc.add_options()
                ("set_system_xml,x", boost::program_options::value<std::vector<std::string> >(&vCommandName)->multitoken(),
                 "set_system_xml <some_system_xml> file")
                ;
        boost::program_options::variables_map vm;
        boost::program_options::store(boost::program_options::parse_command_line(argc,argv,desc), vm);
        boost::program_options::notify(vm);
        std::cout << std::endl << desc << std::endl;
        
        if (vm.count("set_system_xml") >= 1)
            boost::filesystem::create_symlink("$NITAETC/system.xml", "vFileName");

    }
    catch (const std::exception& e)
    {
        std::cerr << "error: " << e.what();
    }
    catch(...)
    {
        std::cerr << "Exception of unknown type!" << std::endl;
    }
    lib.Free();
    return 0;
}
