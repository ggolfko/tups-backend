#include "config.h"

void loadConfig(CONFIG &config)
{
    std::ifstream configFile("../data/config");

    if (!configFile.is_open())
    {
        throw "Error: Cannot load config file";
    }

    std::string line;

    while (std::getline(configFile, line))
    {
        size_t pos = line.find("=");
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos+1);

        if (key == "DBHOST")
            config.DBHOST = value;
        else if (key == "DBNAME")
            config.DBNAME = value;
        else if (key == "PASSWORD")
            config.PASSWORD = value;
        else if (key == "USERNAME")
            config.USERNAME = value;
        else if (key == "BEANSTALKHOST")
            config.BEANSTALKHOST = value;
        else if (key == "TUBEJUDGE")
            config.TUBEJUDGE = value;
        else if (key == "TUBECOMPILE")
            config.TUBECOMPILE = value;
        else if (key == "TUBESIM")
            config.TUBESIM = value;
        else if (key == "TUBEDEBUG")
            config.TUBEDEBUG = value;
    }

    configFile.close();
}
