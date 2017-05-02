#ifndef SIM_H
#define SIM_H

#include <memory>
#include <string>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <csignal>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <odb/database.hxx>
#include <odb/mysql/database.hxx>
#include <odb/transaction.hxx>

#include "odb/sim_result-odb.hxx"
#include "odb/solution-odb.hxx"
#include "odb/source_code-odb.hxx"
#include "config.h"
#include "BeanstalkSim.h"

using namespace odb::core;

class Sim
{
private:
    //db
    std::auto_ptr <odb::database> db;

    //general
    std::string workDirPath;
    std::string simResultPath;

    //beanstalk
    BeanstalkSim bClient;
    Beanstalk::Job job;

    //convert
    std::ostringstream convert;

    //mainSolution
    solution mainSolution;
    std::string mainSourceCodePath;
    unsigned int language;

    //temp solution
    solution tempSolution;
    source_code tempSourceCode;

    //sim
    unsigned int maxPercent;
    unsigned int maxSolutionId;

    //method
    void runSim(void);
    void createWorkDir(void);
    void clearWorkDir(void);
    void getMainSolution(unsigned int id);
    void loadSolution(unsigned int id);
    void loadSourceCode(unsigned int id);
    void saveToFile(std::string data, std::string path);
    void updateResult(void);

    //thread
    bool notTerminate;
    bool running;
public:
    Sim(CONFIG config);
    void terminate(void);
    void run(void);
    bool isRunning(void);
};

#endif
