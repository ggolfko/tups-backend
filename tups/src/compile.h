#ifndef COMPILE_H
#define COMPILE_H

#include <memory> //auto_ptr
#include <string>
#include <cstdlib>
#include <fstream>
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
#include "config.h"

using namespace odb::core;

class Compile
{
private:
    std::auto_ptr <odb::database> db;
    std::string workDIRPath;
    std::string sourceCodePath;
    std::string inputPath;
    std::string outputPath;
    std::string resultPath;
    unsigned int tresult;
    std::string compile_info;
    unsigned int language;
    std::string main_class;
    std::string input;
    std::string output;
    unsigned int time_used;
    unsigned int memory_used;
    unsigned int sourceCodeId;
    unsigned int compileId;
    std::string sourceCode;
    void loadDB(unsigned int);
    void saveToFile(std::string, std::string);
    bool doCompile(void);
    void updateRecord(void);
    void doRun(void);
    void runC(void);
    void runJava(void);
    void loadOutput(void);
public:
    Compile(unsigned int, std::string, CONFIG);
    void run(void);
};

#endif
