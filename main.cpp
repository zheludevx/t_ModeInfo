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

const char c_szDelimiters[] = "._-+=";


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

bool outputCfg()
{
    plug_key::CModeInfoPlug lib;
    bool bRes = lib.Load();
    std::cout << "RES>" << bRes << std::endl << std::endl;
    std::vector<std::string> vExpandable;
    vExpandable.push_back("%NITAROOT%");
    vExpandable.push_back("[GROUP]");
    vExpandable.push_back("[CFG]");
    vExpandable.push_back("[PRODUCT]");
    vExpandable.push_back("[MODE]");
    vExpandable.push_back("[LANG]");
    vExpandable.push_back("[COMPUTER]");
    vExpandable.push_back("[UID]");
    for (unsigned int i = 0; i < vExpandable.size(); i++)
    {
        std::cout << vExpandable[i] << " = ";
        lib.ExpandString(vExpandable[i]);
        std::cout<< "\"" << vExpandable[i] << "\"" << std::endl;
    }
    std::cout << std::endl << std::endl;
    lib.Free();
    return bRes;
}

bool outputGroups(std::vector<std::string>& vFileName, const std::string& sPath, const std::string& sSystemXml)
{
    bool bRes = false;
    if (boost::filesystem::is_symlink(sPath+sSystemXml))
        std::cout << "system.xml -> " << boost::filesystem::read_symlink(sPath+sSystemXml) << std::endl;
    else
        std::cout << "ERR> Symlink not found" << std::endl;
    std::cout << "NITAETC groups:" << std::endl;
    for (unsigned int i = 0; i < vFileName.size(); i++)
    {
        if(vFileName[i] != "system.xml")
        {
            if (checkSystem(vFileName[i]) && checkXml(vFileName[i]))
            {
                if(boost::filesystem::read_symlink(sPath+sSystemXml) == vFileName[i])
                    std::cout << "*";
                std::cout << vFileName[i] << std::endl;
                bRes = true;
            }
        }
    }
    std::cout << std::endl << std::endl;
    return bRes;
}

bool checkArgSimple(std::string& sFileName, std::string& sArgName)
{
    bool bRes = false;
    sFileName.erase(0, 6);
    sFileName.erase(sFileName.length() - 4);
    std::string sDelimiter;
    if (sFileName.find_first_of(c_szDelimiters) == 0)
    {
        sDelimiter = sFileName.substr(0, 1);
        sFileName.erase(0, 1);
    }
    else
        sDelimiter = "notFound";

    if (sArgName == sFileName)
        bRes = true;

    sFileName.insert(0, "system");
    sFileName.insert(sFileName.length(), ".xml");
    if (sDelimiter.find_first_of(c_szDelimiters)!=std::string::npos)
        sFileName.insert(6, sDelimiter);

    if (bRes == 1)
        sArgName = sFileName;
    return bRes;
}

bool cinArg(int ac, char* av[], boost::program_options::variables_map& vm)
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

bool checkArg(std::vector<std::string>& vFileName, std::string& sArgName)              
{   
    bool bRes = false;
    for (unsigned int i = 0; i < vFileName.size(); i++)
    {
        if(checkSystem(vFileName[i]) && checkXml(vFileName[i]))
        {
            checkArgSimple(vFileName[i], sArgName);
            if (sArgName == vFileName[i])
            {
                bRes = true;
                break;
            }
        }
    }
    return bRes;
}

bool setSystemXml(const std::string& sPath,const std::string& sSystemXml,const std::string& sArgName)
{
    bool bRes = false;
    if (boost::filesystem::is_symlink(sPath+sSystemXml))
        boost::filesystem::remove(sPath+sSystemXml);

    boost::filesystem::create_symlink(sArgName, sPath+sSystemXml);
    if (boost::filesystem::is_symlink(sPath+sSystemXml))
        bRes = true;
    else
        std::cout << "Symlink not created" << std::endl;
    return bRes;
}

int main(int argc, char* argv[])
{
    std::string sPath = getenv("NITAETC");
    std::string sSystemXml = "/system.xml";
    std::vector<std::string> vFileName;
    try
    {
        boost::filesystem::directory_iterator it(sPath), end;
        for ( ; it != end; ++it)
            vFileName.push_back(it->path().filename().string());

        boost::program_options::variables_map vm;
        cinArg(argc, argv, vm);

        if (vm.count("set_system_xml") == 1)
        {
            std::cout << "arg:" << vm["set_system_xml"].as<std::string>() << std::endl << std::endl << std::endl;
            std::string sArgName = vm["set_system_xml"].as<std::string>();

            if (checkArg(vFileName, sArgName) == 1)
                setSystemXml(sPath, sSystemXml, sArgName);
            else
                std::cout << "ERR> File not found: " << sArgName  << std::endl << std::endl << std::endl;
        }

        if(vm.count("show_config") == 1)
            outputCfg();

        if(vm.count("show_groups") == 1)
            outputGroups(vFileName, sPath, sSystemXml);
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
