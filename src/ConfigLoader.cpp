#include "ConfigLoader.hpp"
#include <fstream>
#include <string>
#include <cerrno>
#include <assert.h>
#include <sstream>
#include "NetworkEvents.h"

ConfigLoader::ConfigLoader()
{
    //ctor
    std::string configFileContent = get_file_contents(getConfigPath());

    bool parsingSuccessfull = reader.parse( configFileContent, root);
    if(parsingSuccessfull)
    {
    std::cout << "Parsing succesfull" << std::endl;
	settings = root["settings"];
	std::cout << getAllSettings();

    }
    else{
		std::cout << "Failed to parse configuration \n"
                  << reader.getFormattedErrorMessages();

		sf::sleep(sf::seconds(5.0f));
		assert(parsingSuccessfull);
    }






}

ConfigLoader::~ConfigLoader()
{
    //dtor
}
bool ConfigLoader::getBoolSetting(std::string str)
{
    return settings[str].asBool();
}
float ConfigLoader::getFloat(std::string path)
{
    return getObjectAtPath(path).asFloat();
}
std::string ConfigLoader::getString(std::string path)
{
    return getObjectAtPath(path).asString();
}
int ConfigLoader::getInt(std::string path)
{
    return getObjectAtPath(path).asInt();
}
std::vector<int> ConfigLoader::getIntArray(std::string path, bool report)
{
    Json::Value obj = getObjectAtPath(path, report);
    assert(obj.isArray());
    std::vector<int> ints(obj.size());
    for(unsigned int index = 0; index < obj.size(); index++)
    {
        assert(obj[index].isIntegral());
        ints[index] = obj[index].asInt();
    }
    return ints;

}

std::vector<float> ConfigLoader::getFloatArray(std::string path)
{
    Json::Value obj = getObjectAtPath(path);
    assert(obj.isArray());
    std::vector<float> floats(obj.size());
    for(unsigned int index = 0; index < obj.size(); index++)
    {
        assert(obj[index].isNumeric());
        floats[index] = obj[index].asFloat();
    }
    return floats;
}


std::vector<std::string> ConfigLoader::getStringArray(std::string path)
{
    Json::Value obj = getObjectAtPath(path);
    assert (obj.isArray());
    std::vector<std::string> strings(obj.size());
    for(unsigned int index = 0; index < obj.size(); index++)
    {
        assert(obj[index].isString());
        strings[index] = obj[index].asString();
    }
    return strings;
}


Json::Value ConfigLoader::getObjectAtPath(std::string path, bool report)
{
    assert(path.size() > 0);

    Json::Value obj = root;


    if(Debug::VERBOSE_MESSAGES && report)std::cout << "checking : " << path << std::endl;
    /*
    for(size_t found = found = path.find(",",0); found != std::string::npos; found = path.find(",",last)){

    	std::string test(path, last, found);
    	std::cout << found << " = " << test << std::endl;
    	obj = obj[test];
    	last = found + 1;
    }
    */

    size_t last = 0;
    size_t found;

    while(true)
    {

        found = path.find(",", last);
        std::string test(path, last, found - last);
        last = found + 1;
        assert(obj.isMember(test));
        obj = obj[test];
        if(found == std::string::npos)break;
    }


    if(Debug::VERBOSE_MESSAGES && report)std::cout << obj << std::endl;
    return obj;
}
std::string ConfigLoader::getMasterServerAddress()
{
    return settings["masterServerAddress"].asString();
}

std::string ConfigLoader::getAllSettings()
{
    std::stringstream stream;
    stream << "----Reading settings:" << std::endl;

    Json::Value::Members members = settings.getMemberNames();
    for(unsigned int i = 0 ; i < members.size(); ++i)
    {
        std::string key = members[i];
        stream << key << " = " << settings[key];
    }

    stream << "--------------------" << std::endl;
    return stream.str();

}
std::string ConfigLoader::get_file_contents(const char *filename)
{
//http://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return(contents);
    }
    throw(errno);
}
