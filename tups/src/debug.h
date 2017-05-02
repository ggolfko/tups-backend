#ifndef DEBUG_H
#define DEBUG_H

#include <memory> //auto_ptr
#include <string>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <csignal>
#include <cstdio>
#include <vector>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>

#include <odb/database.hxx>
#include <odb/mysql/database.hxx>
#include <odb/transaction.hxx>

#include "odb/compile-odb.hxx"
#include "odb/source_code-odb.hxx"
#include "odb/debug-odb.hxx"
#include "config.h"

using namespace odb::core;

class Debug 
{
private:
    std::auto_ptr <odb::database> db;
    std::string workDIRPath;
    std::string sourceCodePath;
    std::string inputPath;
    std::string outputPath;
    std::string commandPath;
    std::string logPath;
    std::string sourceCode;
    std::string resultPath;
    std::string programOutput;
    std::string input;
    std::string log;
    std::string gcommand;
    unsigned int tresult;
    unsigned int sourceCodeId;
    unsigned int language;
    unsigned int treuslt;
    unsigned int debugId;
    unsigned int currentLine;
    void loadDB(unsigned int);
    void saveToFile(std::string, std::string);
    bool doCompile(void);
    void updateRecord(void);
    void doRun(void);
    void runC(void);
    void runDebug(void);
    void loadLog(void);
public:
    Debug(unsigned int, std::string, CONFIG);
    void run(void);
};

#endif
