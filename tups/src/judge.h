#ifndef JUDGE_H
#define JUDGE_H

#include <memory> //auto_ptr
#include <string>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <algorithm>
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

#include "odb/problem-odb.hxx"
#include "odb/solution-odb.hxx"
#include "odb/source_code-odb.hxx"
#include "odb/test_case-odb.hxx"
#include "odb/diff_option-odb.hxx"
#include "odb/compile_info-odb.hxx"
#include "odb/diff_result-odb.hxx"
#include "BeanstalkSim.h"
#include "config.h"

using namespace odb::core;

typedef struct
{
    unsigned int id;
    std::string input;
    std::string output;
} TESTCASE;

class Judge
{
private:
    //db
    std::auto_ptr <odb::database> db;
    //beanstalk sim queue
    BeanstalkSim bClient;
    //main
    bool diff;
    std::string workDIRPath;
    std::string sourceCodePath;
    std::string inputPath;
    std::string outputPath;
    std::string resultPath;
    std::string answerPath;
    //solution
    unsigned int solutionId;
    unsigned int problemId;
    unsigned int sourceCodeId;
    unsigned int language;
    unsigned int user_id;
    float ratio;
    std::string mainClass;
    //problem
    unsigned int timeLimit;
    unsigned int memoryLimit;
    unsigned int diffOption;
    //source code
    std::string sourceCode;
    //test case
    std::vector <TESTCASE> vTestCase;
    //diff option
    unsigned int testCaseId;
    std::string diffCommand;
    //compile info
    std::string compileInfo;
    //Other
    unsigned int tresult;
    std::vector <unsigned int> vResult;
    std::string input;
    std::string output;
    unsigned int time_used;
    unsigned int memory_used;
    void loadDB(unsigned int);
    void saveToFile(std::string, std::string);
    bool doCompile(void);
    void updateRecord(void);
    void doRun(void);
    void runC(void);
    void runJava(void);
    void loadOutput(void);
    void runDiff(void);
    void setSourceCodePath(void);
    void setCompileInfo(void);
    void setDiff(void);
    void updateStats(void);
    void finishedDiff(void);
    void runSim(void);
public:
    Judge(unsigned int, std::string, CONFIG, bool);
    void run(void);
};

#endif
