#include "debug.h"

Debug::Debug(unsigned int id, std::string workDIR, CONFIG config)
    :db(new odb::mysql::database(config.USERNAME, config.PASSWORD, config.DBNAME, config.DBHOST))
{
    workDIRPath = workDIR.append("/");
    sourceCodePath = workDIRPath;
    inputPath = workDIRPath;
    outputPath = workDIRPath;
    resultPath = workDIRPath;
    commandPath = workDIRPath;
    logPath = workDIRPath;
    debugId = id;
    log = "Wait for gdb";
    programOutput = "Wait for gdb";

    inputPath.append("test.in");
    outputPath.append("test.out");
    resultPath.append("test.res");
    commandPath.append("gdb.cmd");
    logPath.append("gdb.log");
    currentLine = 0;

    loadDB(id);
}

void Debug::doRun(void)
{
    tresult = RN;
    updateRecord();

    if (language == 1 || language == 2 || language == 4)
        runC();
    else
        return;

    if (tresult == AC || tresult == WANS || tresult == RE)
    {
        runDebug();
    }
    else
    {
        tresult = RF;
        updateRecord();
    }
}

void Debug::runDebug(void)
{
    //append debug command 
    std::string tmp;
    tmp = "set confirm off\n";
    tmp += "set args < " + inputPath + " > " + outputPath + "\n";
    tmp += "set logging off\n";
    tmp += "set logging file " + logPath + "\n";
    tmp += "set logging on\n";
    tmp += gcommand + "\n";
    tmp += "k\n";
    tmp += "q\n";

    saveToFile(tmp, commandPath);

    //execute gdb
    std::string command;
    command = "gdb -batch -x ";
    command += commandPath;
    command += " ";
    command += workDIRPath + "main.o"; 

    //running
    tresult = RN;
    
    FILE *pipe;
    if ((pipe = (FILE *)popen(command.c_str(), "r")) == NULL)
    {
        throw "Pipe error";
    }

    pclose(pipe);

    loadLog();

    tresult = AC;
    updateRecord();
}

void Debug::loadLog(void)
{
    std::ifstream logFile(logPath.c_str());
    if (!logFile.is_open())
        throw "Log file not found";

    std::string res, tmpResult;
    while (getline(logFile, tmpResult))
    {
        if (tmpResult.find("Line") == 0)
        {
            std::stringstream convert;
            convert.str(tmpResult) ;
            std::string tmp;
            convert >> tmp;
            convert >> currentLine;
            continue;
        }

        res.append(tmpResult);
        res.append("\n");
    }

    logFile.close();
    log = res;

    std::ifstream outFile(outputPath.c_str());
    res = "";

    if (!outFile.is_open())
        throw "Output file not found";

    while (getline(outFile, tmpResult))
    {

        res.append(tmpResult);
        res.append("\n");
    }

    outFile.close();
    programOutput = res;
}

void Debug::run(void)
{
    //save resource to file
    saveToFile(sourceCode, sourceCodePath);
    saveToFile(input, inputPath);

    if (!doCompile())
        return;

    doRun();
}

void Debug::runC(void)
{
    FILE *pipe;
    char tmpTime[] = "10000";
    char tmpMemory[] = "5120";

    std::string cmd;
    cmd = "./esandbox ";
    cmd.append(workDIRPath);
    cmd.append("main.o ");
    cmd.append(tmpTime);
    cmd.append(" ");
    cmd.append(tmpMemory);
    cmd.append(" ");
    cmd.append(resultPath);
    cmd.append(" < ");
    cmd.append(inputPath);
    cmd.append(" > ");
    cmd.append(outputPath);
    cmd.append(" 2>&1");

    if ((pipe = (FILE *)popen(cmd.c_str(), "r")) == NULL)
    {
        throw "Pipe failure";
    }

    pclose(pipe);

    std::ifstream res(resultPath.c_str());
    if (!res.is_open())
        throw "Result file not found";

    std::string sandboxResult;
    std::vector <std::string> v;

    while (std::getline(res, sandboxResult))
    {
        v.push_back(sandboxResult);
    }

    res.close();

    int re = atoi(v[1].c_str());

    if (re != 1)
    {
        switch (re)
        {
        case 2:
            tresult = RF;
            break;
        case 3:
            tresult = MLE;
            break;
        case 4:
            tresult = OLE;
            break;
        case 5:
            tresult = TLE;
            break;
        case 7:
            tresult = AT;
            break;
        default:
            tresult = RE;
            break;
        }
        return;
    }
    tresult = AC;
}

//TODO Compiler path 
bool Debug::doCompile(void)
{
    std::string command;

    switch (language)
    {
    case 1:
        command = "gcc -w -static -pipe -g ";
        command.append(sourceCodePath);
        command.append(" -lm ");
        command.append(" -o ");
        command.append(workDIRPath);
        command.append("main.o");
        break;
    case 2:
        command = "g++ -w -static -pipe -g ";
        command.append(sourceCodePath);
        command.append(" -lm ");
        command.append(" -o ");
        command.append(workDIRPath);
        command.append("main.o");
        break;
    case 4:
        command = "g++ -std=c++11 -w -static -pipe -g ";
        command.append(sourceCodePath);
        command.append(" -lm ");
        command.append(" -o ");
        command.append(workDIRPath);
        command.append("main.o");
        break;
    }

    int pipefd[2];
    bool error = false;
    pipe(pipefd);

    pid_t runId = fork();

    if (runId == 0)
    {
        close(pipefd[0]);
        dup2(pipefd[1], 1);
        dup2(pipefd[1], 2);
        close(pipefd[1]);
        setpgid(0, 0);
        execl("/bin/sh", "sh", "-c", command.c_str(), NULL);
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    close(pipefd[1]);

    pid_t wallId = fork();
    if (wallId == 0)
    {
        for (int i = 0 ; i < 10 ; i++)
            sleep(1);

        kill(-runId, SIGTERM);
        exit(EXIT_SUCCESS);
    }

    while (read(pipefd[0], buffer, sizeof(buffer)) != 0)
    {
        error = true;
    }

    pid_t waitId;
    int status = 0;
    int wallstatus = 0;

    do
    {
        waitId = waitpid(runId, &status, 0);
    }
    while (waitId != runId);

    kill(wallId, SIGTERM);
    waitpid(wallId, &wallstatus, 0);

    if (error)
        return false;
    return true;
}

void Debug::updateRecord(void)
{
    transaction t(db->begin());

    std::auto_ptr <debug> record (db->load <debug> (debugId));

    record->setResult(tresult);
    record->setOutput(log);
    record->setProgramOutput(programOutput);
    record->setCurrentLine(currentLine);

    db->update(*record);
    t.commit();
}

void Debug::saveToFile(std::string data, std::string path)
{
    std::ofstream outfile;
    outfile.open(path.c_str());

    if (!outfile.is_open())
        throw "Error when create file";

    outfile << data;
    outfile.close();
}


void Debug::loadDB(unsigned int id)
{
    typedef odb::query <source_code> qs;
    typedef odb::result <source_code> rs;
    typedef odb::query <compile> qr;
    typedef odb::result <compile> rr;
    typedef odb::query <debug> qd;
    typedef odb::result <debug> rd;

    transaction t(db->begin());

    rd objDebug(db->query <debug> (qd::id == id));
    
    if (objDebug.begin() == objDebug.end())
    {
        t.commit();
        throw "Not found debug record";
    }

    gcommand = objDebug.begin()->getCommand();
    unsigned int compile_id = objDebug.begin()->getCompileId();

    rr objCompile(db->query <compile> (qr::id == compile_id));

    if (objCompile.begin() == objCompile.end())
    {
        t.commit();
        throw "Not found compile record";
    }

    sourceCodeId = objCompile.begin()->getSourceCode();
    language = objCompile.begin()->getLanguage();
    input = objCompile.begin()->getInput();

    t.commit();

    if (language == 1)
        sourceCodePath.append("main.c");
    else if (language == 2 || language == 4)
        sourceCodePath.append("main.cpp");

    t.reset(db->begin());

    rs objSourceCode(db->query <source_code> (qs::id == sourceCodeId));

    if (objSourceCode.begin() == objSourceCode.end())
    {
        t.commit();
        throw "Source code not found";
    }

    sourceCode = objSourceCode.begin()->getSourceCode();

    t.commit();
}
