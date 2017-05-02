#include "compile.h"

Compile::Compile(unsigned int id, std::string workDIR, CONFIG config)
    :db(new odb::mysql::database(config.USERNAME, config.PASSWORD, config.DBNAME, config.DBHOST))
{
    workDIRPath = workDIR.append("/");
    sourceCodePath = workDIRPath;
    inputPath = workDIRPath;
    outputPath = workDIRPath;
    resultPath = workDIRPath;
    compileId = id;

    inputPath.append("test.in");
    outputPath.append("test.out");
    resultPath.append("test.res");

    loadDB(id);
}

void Compile::doRun(void)
{
    tresult = RN;
    updateRecord();

    if (language == 1 || language == 2 || language == 4)
        runC();
    else if (language == 3)
        runJava();

    if (tresult == AC)
        loadOutput();

    updateRecord();
}

void Compile::loadOutput(void)
{
    std::ifstream outputFile(outputPath.c_str());
    if (!outputFile.is_open())
        throw "Output file not found";

    std::string res, tmpResult;
    while (getline(outputFile, tmpResult))
    {
        res.append(tmpResult);
        res.append("\n");
    }

    outputFile.close();

    output = res;
}

void Compile::run(void)
{
    //save resource to file
    saveToFile(sourceCode, sourceCodePath);
    saveToFile(input, inputPath);

    if (!doCompile())
        return;

    doRun();
}

void Compile::runJava(void)
{
    pid_t runId;
    struct rusage ruse;
    struct rlimit LIM;

    std::string cmd = "java -Xrs -cp ";
    cmd.append(workDIRPath);
    cmd.append(" -Xmx256m");
    cmd.append(" -Djava.security.manager");
    cmd.append(" -Djava.security.policy=../data/java.policy");
    cmd.append(" ");
    cmd.append(main_class);
    cmd.append(" < ");
    cmd.append(inputPath);
    cmd.append(" > ");
    cmd.append(outputPath);

    runId = fork();
    if (runId == 0)
    {
        getrlimit(RLIMIT_CPU, &LIM);
        LIM.rlim_cur=LIM.rlim_max=10+2+1;
        setrlimit(RLIMIT_CPU, &LIM);
        execl("/bin/sh", "sh", "-c", cmd.c_str(), NULL);
        exit(EXIT_SUCCESS);
    }
    
    pid_t wallId = fork();
    if (wallId == 0)
    {
        for (int i = 0 ; i < 10 ; i++)
            sleep(1);

        kill(-runId, SIGTERM);
        exit(EXIT_SUCCESS);
    }


    pid_t wait_result;
    int status;

    do
    {
        wait_result = wait4(runId, &status, 0, &ruse);
    }
    while (wait_result != runId);

    memory_used = ruse.ru_minflt*getpagesize()/1024;
    time_used = ruse.ru_utime.tv_sec*1000 + ruse.ru_utime.tv_usec/1000;
    time_used += (ruse.ru_stime.tv_sec*1000 + ruse.ru_stime.tv_usec/1000);

    if (time_used/1000 > 10)
    {
        tresult = TLE;
        return;
    }

    if (memory_used/1024 > 256)
    {
        tresult = MLE;
        return;
    }

    if (status)
    {
        tresult = RE;
        return;
    }

    tresult = AC;
}

void Compile::runC(void)
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

    time_used = atoi(v[2].c_str());
    memory_used = atoi(v[3].c_str());
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
bool Compile::doCompile(void)
{
    std::string command;

    switch (language)
    {
    case 1:
        command = "gcc -w -static -pipe ";
        command.append(sourceCodePath);
        command.append(" -lm ");
        command.append(" -o ");
        command.append(workDIRPath);
        command.append("main.o");
        break;
    case 2:
        command = "g++ -w -static -pipe ";
        command.append(sourceCodePath);
        command.append(" -lm ");
        command.append(" -o ");
        command.append(workDIRPath);
        command.append("main.o");
        break;
    case 3:
        command = "javac ";
        command.append(sourceCodePath);
        command.append(" -nowarn");
        break;
    case 4:
        command = "g++ -std=c++11 -w -static -pipe ";
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

    compile_info = "";

    while (read(pipefd[0], buffer, sizeof(buffer)) != 0)
    {
        error = true;
        compile_info.append(buffer);
        compile_info.append("\n");
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

    if (!error)
        compile_info.append("Error when compile");

    if (status != 0)
    {
        time_used = 0;
        memory_used = 0;
        tresult = CE;
        updateRecord();
        return false;
    }
    return true;
}

void Compile::updateRecord(void)
{
    transaction t(db->begin());

    std::auto_ptr <compile> record (db->load <compile> (compileId));

    record->setResult(tresult);
    record->setOutput(output);
    record->setTimeUsed(time_used);
    record->setMemoryUsed(memory_used);
    record->setCompileInfo(compile_info);

    db->update(*record);
    t.commit();

}

void Compile::saveToFile(std::string data, std::string path)
{
    std::ofstream outfile;
    outfile.open(path.c_str());

    if (!outfile.is_open())
        throw "Error when create file";

    outfile << data;
    outfile.close();
}


void Compile::loadDB(unsigned int id)
{
    typedef odb::query <source_code> qs;
    typedef odb::result <source_code> rs;
    typedef odb::query <compile> qr;
    typedef odb::result <compile> rr;

    transaction t(db->begin());

    rr objCompile(db->query <compile> (qr::id == id));

    if (objCompile.begin() == objCompile.end())
    {
        t.commit();
        throw "Not found compile record";
    }

    sourceCodeId = objCompile.begin()->getSourceCode();
    language = objCompile.begin()->getLanguage();
    main_class = objCompile.begin()->getMainClass();
    input = objCompile.begin()->getInput();

    t.commit();

    if (language == 1)
        sourceCodePath.append("main.c");
    else if (language == 2 || language == 4)
        sourceCodePath.append("main.cpp");
    else if (language == 3)
    {
        sourceCodePath.append(main_class);
        sourceCodePath.append(".java");
    }

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
