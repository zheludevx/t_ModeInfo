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
        std::string& sExpandable = vExpandable[i];
        std::cout << sExpandable << " = \t";
        lib.ExpandString(sExpandable);
        std::cout<< "\"" << sExpandable << "\"" << std::endl;
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
        const std::string& sFileName = vFileName[i];
        if(sFileName != "system.xml")
        {
            if (checkSystem(sFileName) && checkXml(sFileName))
            {
                if(boost::filesystem::read_symlink(sPath+sSystemXml) == sFileName)
                    std::cout << " *";
                std::cout << sFileName << std::endl;
                bRes = true;
            }
        }
    }
    std::cout << std::endl << std::endl;
    return bRes;
}

bool checkArgSimple(const std::string& sFileName, const std::string& sArgName)
{
    bool bRes = false;
    std::string sFileNameForCheck = sFileName;
    sFileNameForCheck.erase(0, 6);
    sFileNameForCheck.erase(sFileNameForCheck.length() - 4);
    std::string sDelimiter;
    if (sFileNameForCheck.find_first_of(c_szDelimiters) == 0)
    {
        sDelimiter = sFileNameForCheck.substr(0, 1);
        sFileNameForCheck.erase(0, 1);
    }
    else
        sDelimiter = "notFound";

    if (sArgName == sFileNameForCheck)
        bRes = true;

    sFileNameForCheck.insert(0, "system");
    sFileNameForCheck.insert(sFileNameForCheck.length(), ".xml");
    if (sDelimiter.find_first_of(c_szDelimiters)!=std::string::npos)
        sFileNameForCheck.insert(6, sDelimiter);
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

std::string getFileByArg (const std::vector<std::string>& vFileName, const std::string& sArgName)
{   
    std::string sRetrievedFileName;
    for (unsigned int i = 0; i < vFileName.size(); i++)
    {
        std::string sFileName = vFileName[i];
        if(checkSystem(sFileName) && checkXml(sFileName))
        {
            if(checkArgSimple(sFileName, sArgName) == 1)
            {
                sRetrievedFileName = sFileName;
                break;
            }

            if (sArgName == sFileName)
            {
                sRetrievedFileName = sArgName;
                break;
            }
        }
    }
    return sRetrievedFileName;
}

bool setSystemXml(const std::string& sPath,const std::string& sSystemXml,const std::string& sArgName)
{
    bool bRes = false;
    if (boost::filesystem::is_symlink(sPath+sSystemXml))
        boost::filesystem::remove(sPath+sSystemXml);
    else
        boost::filesystem::rename(sPath+sSystemXml, sPath+sSystemXml+".bak");

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
    bool bCheckPath = getenv("NITAETC");
    if (!bCheckPath)
    {
       #ifdef WIN32
            sPath = "c:\\Nita\\Config";
       #else
            sPath = "/soft/etc";
       #endif
    }
    std::string sSystemXml = "/system.xml";
    std::vector<std::string> vFileName;
    try
    {
        boost::filesystem::directory_iterator it(sPath), end;
        for ( ; it != end; ++it)
            vFileName.push_back(it->path().filename().string());

        boost::program_options::variables_map vm;
        cinArg(argc, argv, vm);

        if (vm.count("set_system_xml"))
        {
            std::cout << "arg:" << vm["set_system_xml"].as<std::string>() << std::endl << std::endl << std::endl;
            std::string sArgName = vm["set_system_xml"].as<std::string>();
            std::string sFileName = getFileByArg(vFileName, sArgName);
            if (!sFileName.empty())
                setSystemXml(sPath, sSystemXml, sFileName);
            else
                std::cout << "ERR> Can't set system.xml by name: " << sArgName << std::endl << std::endl << std::endl;
        }

        if(vm.count("show_config"))
            outputCfg();

        if(vm.count("show_groups"))
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
