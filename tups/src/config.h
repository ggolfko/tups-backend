#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <fstream>

#define PD 0
#define RJ 1
#define RN 3
#define JG 4
#define AC 5
#define WANS 6
#define TLE 7
#define MLE 8
#define RF 9
#define RE 10
#define CE 11
#define OLE 12
#define AT 13

typedef struct
{
    //DB Config
    std::string DBHOST;
    std::string DBNAME;
    std::string USERNAME;
    std::string PASSWORD;

    //Beanstalkd config
    std::string BEANSTALKHOST;
    std::string TUBEJUDGE;
    std::string TUBECOMPILE;
    std::string TUBESIM;
    std::string TUBEDEBUG;
} CONFIG;

void loadConfig(CONFIG &config);

#endif
