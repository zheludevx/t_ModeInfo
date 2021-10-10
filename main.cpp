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
        sFileName += ".bak";
        bRes = true;
    }
    return bRes;
}

bool outputCfg(std::vector<std::string>& vArr)
{
    plug_key::CModeInfoPlug lib;
    bool bRes = lib.Load();
    std::cout << "RES>" << bRes << std::endl << std::endl;
    vArr.push_back("%NITAROOT%");
    vArr.push_back("[GROUP]");
    vArr.push_back("[CFG]");
    vArr.push_back("[PRODUCT]");
    vArr.push_back("[MODE]");
    vArr.push_back("[LANG]");
    vArr.push_back("[COMPUTER]");
    vArr.push_back("[UID]");
    for (unsigned int i = 0; i < vArr.size(); i++)
    {
        std::cout << vArr[i] << " = ";
        lib.ExpandString(vArr[i]);
        std::cout<< "\"" << vArr[i] << "\"" << std::endl;
    }
    std::cout << std::endl << std::endl;
    lib.Free();
    return bRes;
}

bool outputGroups(std::vector<std::string>& vFileName, const std::string& sEnvir, const std::string& sSystemXml)
{
    bool bRes = false;
    if (boost::filesystem::is_symlink(sEnvir+sSystemXml))
        std::cout << "system.xml -> " << boost::filesystem::read_symlink(sEnvir+sSystemXml) << std::endl;
    else
        std::cout << "ERR> Symlink not found" << std::endl;
    std::cout << "NITAETC groups:" << std::endl;
    for (unsigned int i = 0; i < vFileName.size(); i++)
    {
        Rename(vFileName[i]);
        if (checkSystem(vFileName[i]) && checkXml(vFileName[i]))
        {
            if(boost::filesystem::read_symlink(sEnvir+sSystemXml) == vFileName[i])
                std::cout << "*";
            std::cout << vFileName[i] << std::endl;
            bRes = true;
        }
    }
    std::cout << std::endl << std::endl;
    return bRes;
}

bool simplCheckArg(std::string& sFileName, std::string& sSymbolFileName)
{
    bool bRes = false;
    sFileName.erase(0, 6);
    sFileName.erase(sFileName.length() - 4);
    if (sFileName.find(".") == 0 || sFileName.find("_") == 0 || sFileName.find("-") == 0)
    {
        sSymbolFileName = sFileName.substr(0, 1);
        sFileName.erase(0, 1);
        bRes = true;
    }
    return bRes;
}

bool afterSimplCheckArg(std::string& sFileName, std::string& sSymbolFileName)
{
    bool bRes = false;
    sFileName.insert(0, "system");
    sFileName.insert(sFileName.length(), ".xml");
    if (sSymbolFileName == "." || sSymbolFileName == "_" || sSymbolFileName == "-")
    {
        sFileName.insert(6, sSymbolFileName);
        bRes = true;
    }
    return bRes;
}

bool cinArg (int ac, char* av[], boost::program_options::variables_map& vm)
{
    bool bRes = false;
    try
    {
        boost::program_options::options_description desc("Command Parser");
        desc.add_options()
                ("help,h", "show help")
                ("set_system_xml,x",
                 boost::program_options::value<std::string>(),
                 "set_system_xml <some_system_xml> file")
                ( "show_config,c", "output config")
                ( "show_groups,g", "output groups")
                ;
        boost::program_options::store(boost::program_options::parse_command_line(ac,av,desc), vm);
        std::cout << std::endl << std::endl;
        if(vm.count("help") || ( vm.size() == 0 ))
            std::cout << desc << std::endl << std::endl;
        else
        {
            boost::program_options::notify(vm);
            bRes = true;
        }
    }
    catch(std::exception& e)
    {
        std::cerr << "error: " << e.what() << std::endl;
    }
    catch(...)
    {
        std::cerr << "Exception of unknown type!" << std::endl;
    }
    return bRes;
}

bool checkArg(const boost::program_options::variables_map& vm, std::vector<std::string>& vFileName,
              const std::string& sEnvir, const std::string& sSystemXml, std::vector<std::string>& vArr)
{   
    bool bRes = false;
    try
    {
        if (vm.count("set_system_xml") == 1)
        {
            std::cout << "arg:" << vm["set_system_xml"].as<std::string>() << std::endl << std::endl << std::endl;
            boost::filesystem::path pArg = vm["set_system_xml"].as<std::string>();
            std::string sArgName = pArg.string();
            std::string sSymbolFileName;
            int iCheckFileExistence = 0;

            for (unsigned int i = 0; i < vFileName.size(); i++)
            {
                if(checkSystem(vFileName[i]) && checkXml(vFileName[i]))
                {
                    simplCheckArg(vFileName[i], sSymbolFileName);
                    if (sArgName == vFileName[i])
                    {
                        afterSimplCheckArg(vFileName[i], sSymbolFileName);
                        sArgName = vFileName[i];
                        iCheckFileExistence++;
                    }
                    afterSimplCheckArg(vFileName[i], sSymbolFileName);
                }
            }

            for (unsigned int i = 0; i < vFileName.size(); i++)
                if (sArgName == vFileName[i])
                    iCheckFileExistence++;

            if (iCheckFileExistence > 0)
            {
                if (checkSystem(sArgName) && checkXml(sArgName))
                {
                    if (boost::filesystem::is_symlink(sEnvir+sSystemXml))
                        boost::filesystem::remove(sEnvir+sSystemXml);
                    boost::filesystem::create_symlink(sArgName, sEnvir+sSystemXml);
                    bRes = true;
                }
            }
            else
                std::cout << "ERR> File not found: " << sArgName  << std::endl << std::endl << std::endl;
        }
        if(vm.count("show_config") == 1)
            outputCfg(vArr);

        if(vm.count("show_groups") == 1)
            outputGroups(vFileName, sEnvir, sSystemXml);
    }
    catch(std::exception& e)
    {
        std::cerr << "error: " << e.what() << std::endl;
    }
    catch(...)
    {
        std::cerr << "Exception of unknown type!" << std::endl;
    }
    return bRes;
}

int main(int argc, char* argv[])
{
    std::string sEnvir = getenv("NITAETC");
    std::string sSystemXml = "/system.xml";
    std::vector<std::string> vArr;
    std::vector<std::string> vFileName;
    try
    {
        boost::filesystem::directory_iterator it(sEnvir), end;
        for ( ; it != end; ++it)
            vFileName.push_back(it->path().filename().string());

        boost::program_options::variables_map vm;
        cinArg(argc, argv, vm);
        checkArg(vm, vFileName, sEnvir, sSystemXml, vArr);
    }
    catch (const std::exception& e)
    {
        std::cerr << "error: " << e.what();
    }
    catch(...)
    {
        std::cerr << "Exception of unknown type!" << std::endl;
    }
    return 0;
}
