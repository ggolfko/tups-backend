#include "judge.h"
#include <iostream>

Judge::Judge(unsigned int id, std::string workDIR, CONFIG config, bool inDiff)
    :db(new odb::mysql::database(config.USERNAME, config.PASSWORD, config.DBNAME, config.DBHOST)), bClient(config.BEANSTALKHOST.c_str(), 11300)
{
    //set path
    workDIRPath = workDIR.append("/");
    sourceCodePath = workDIRPath;
    inputPath = workDIRPath;
    outputPath = workDIRPath;
    resultPath = workDIRPath;
    answerPath = workDIRPath;

    inputPath.append("test.in");
    outputPath.append("test.out");
    resultPath.append("test.res");
    answerPath.append("solution.out");

    solutionId = id;
    diff = inDiff;

    loadDB(id);

    time_used = 0;
    memory_used = 0;

    //set beanstalk sim tube
    bClient.setTube(config.TUBESIM);
}

void Judge::runSim(void)
{
    std::ostringstream convert;
    convert.str("");
    convert << solutionId;

    bClient.addJob(convert.str());
}

void Judge::updateStats(void)
{
    transaction t(db->begin());

    odb::result <NSOLVED> r(db->query <NSOLVED> (query <solution>::user_id == user_id && query <solution>::result == 5));
    odb::result <NSUBMIT> rs(db->query <NSUBMIT> (query <solution>::user_id == user_id));

    const NSOLVED& sol(*r.begin());
    const NSUBMIT& sub(*rs.begin());

    int nsubmit = sub.submit;
    int nsolved = sol.solved;

    std::auto_ptr <user> record(db->load <user> (user_id));

    record->setSubmit(nsubmit);
    record->setSolved(nsolved);

    db->update(*record);
    t.commit();
}

void Judge::finishedDiff(void)
{
    transaction t(db->begin());

    std::auto_ptr <solution> record (db->load <solution> (solutionId));

    record->setFinishDiff();

    db->update(*record);
    t.commit();
}

void Judge::runDiff(void)
{
    FILE *pipe;
    char line[1000] = "";

    std::string cmd = "/usr/bin/diff ";
    cmd.append(diffCommand);
    cmd.append(" ");
    cmd.append(outputPath);
    cmd.append(" ");
    cmd.append(answerPath);
    cmd.append(" 2>&1");

    if ((pipe = (FILE *)popen(cmd.c_str(), "r")) == NULL)
    {
        throw "Pipe failure";
    }

    fgets(line, sizeof(line), pipe);
    pclose(pipe);

    if (!strlen(line))
    {
        vResult.push_back(AC);
        return;
    }

    //save diff to db
    if (diff)
    {
        loadOutput();
        setDiff();
    }

    vResult.push_back(WANS);
}

void Judge::setDiff(void)
{
    diff_result record(solutionId, testCaseId, output);

    typedef odb::query <diff_result> query;
    typedef odb::result <diff_result> result;

    transaction t(db->begin());

    result r (db->query <diff_result> (query::solution_id == solutionId && query::test_case_id == testCaseId));

    //insert if not exists
    if (r.begin() == r.end())
        db->persist(record);
    else
    {
        for (result::iterator i(r.begin()) ; i != r.end() ; i++)
        {
            std::auto_ptr <diff_result> orecord (i.load());
            orecord->setOutput(output);
            db->update(*orecord);
        }
    }

    t.commit();
}

void Judge::setCompileInfo(void)
{
    compile_info record(solutionId, compileInfo);

    typedef odb::query <compile_info> query;
    typedef odb::result <compile_info> result;

    transaction t(db->begin());

    try
    {
        db->persist(record);
        t.commit();
    }
    catch (std::exception)
    {
        t.reset(db->begin());

        result orecord(db->query <compile_info> (query::solution_id == solutionId));

        if (orecord.begin() == orecord.end())
        {
            t.commit();
            return;
        }

        orecord.begin()->setInfo(compileInfo);
        db->update(*(orecord.begin()));
        t.commit();
    }
}

void Judge::doRun(void)
{
    tresult = RN;
    updateRecord();

    if (language == 1 || language == 2 || language == 4)
        runC();
    else if (language == 3)
        runJava();

    if (tresult == AC)
        runDiff();
    else
        vResult.push_back(tresult);

    if (diff)
        finishedDiff();
}

void Judge::loadOutput(void)
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

void Judge::run(void)
{
    //save resource to file
    setSourceCodePath();
    saveToFile(sourceCode, sourceCodePath);

    if (!doCompile())
    {
        tresult = CE;
        updateRecord();
        setCompileInfo();
        updateStats();
        return;
    }


    int len = vTestCase.size();
    vResult.clear();

    for (int i = 0 ; i < len ; i++)
    {
        testCaseId = vTestCase[i].id;
        saveToFile(vTestCase[i].input, inputPath);
        saveToFile(vTestCase[i].output, answerPath);
        doRun();
    }

    len = vResult.size();
    int nAccept = 0;

    tresult = AC;

    //cal result and ratio
    for (int i = 0 ; i < len ; i++)
    {
        if (vResult[i] == AC)
        {
            nAccept++;
            continue;
        }
        if (vResult[i] == RF)
            tresult = RF;
        else if (vResult[i] == RE && tresult == AC)
            tresult = RE;
        else if ((vResult[i] == TLE || vResult[i] == MLE || vResult[i] == OLE) && (tresult == AC || tresult == RF || tresult == RE || tresult == WANS))
            tresult = vResult[i];
        else if (tresult == AC)
            tresult = vResult[i];
    }

    ratio = nAccept/(float)len * 100;
    updateRecord();

    updateStats();

    //TODO devide class
    if (tresult == AC)
        runSim();
}

void Judge::runJava(void)
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
    cmd.append(mainClass);
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
        setpgid(0, 0);
        execl("/bin/sh", "sh", "-c", cmd.c_str(), NULL);
        exit(EXIT_SUCCESS);
    }
    
    pid_t wallId = fork();
    if (wallId == 0)
    {
        for (int i = 0 ; i < 10 ; i++)
        {
            std::cout << i << std::cout << std::endl;
            sleep(1);
        }

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

    int wallstatus = 0;

    kill(wallId, SIGTERM);
    waitpid(wallId, &wallstatus, 0);

    unsigned int tmemory_used;
    unsigned int ttime_used;

    tmemory_used = ruse.ru_minflt*getpagesize()/1024;
    ttime_used = ruse.ru_utime.tv_sec*1000 + ruse.ru_utime.tv_usec/1000;
    ttime_used += (ruse.ru_stime.tv_sec*1000 + ruse.ru_stime.tv_usec/1000);

    memory_used = std::max(memory_used, tmemory_used);
    time_used = std::max(time_used, ttime_used);

    if (ttime_used/1000 > 10)
    {
        tresult = TLE;
        return;
    }

    if (tmemory_used/1024 > 256)
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

void Judge::runC(void)
{
    FILE *pipe;
    std::ostringstream convert;
    std::string tmpTime;
    std::string tmpMemory;

    convert.str("");
    convert << timeLimit;
    tmpTime = convert.str();

    convert.str("");
    convert << memoryLimit;
    tmpMemory = convert.str();

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

    time_used = std::max(time_used, (unsigned int)atoi(v[2].c_str()));
    memory_used = std::max(memory_used, (unsigned int)atoi(v[3].c_str()));
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

        vResult.push_back(tresult);

        return;
    }

    tresult = AC;
}

//TODO compiler path
bool Judge::doCompile(void)
{
    std::string command;

    switch (language)
    {
    case 1:
        command = "gcc -w -O2 -static -pipe ";
        command.append(sourceCodePath);
        command.append(" -lm ");
        command.append(" -o ");
        command.append(workDIRPath);
        command.append("main.o");
        break;
    case 2:
        command = "g++ -w -O2 -static -pipe ";
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
        command = "g++ -std=c++11 -w -O2 -static -pipe ";
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

    compileInfo = "";

    while (read(pipefd[0], buffer, sizeof(buffer)) != 0)
    {
        error = true;
        compileInfo.append(buffer);
        compileInfo.append("\n");
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
        compileInfo.append("Error when compile");

    if (status != 0)
    {
        time_used = 0;
        memory_used = 0;
        ratio = 0;
        return false;
    }
    return true;
}

void Judge::updateRecord(void)
{
    transaction t(db->begin());

    std::auto_ptr <solution> record (db->load <solution> (solutionId));

    record->setResult(tresult);
    record->setTime(time_used);
    record->setMemory(memory_used);
    record->setRatio(ratio);

    db->update(*record);
    t.commit();
}

void Judge::saveToFile(std::string data, std::string path)
{
    std::ofstream outfile;
    outfile.open(path.c_str());

    if (!outfile.is_open())
        throw "Error when create file";

    outfile << data;
    outfile.close();
}

void Judge::setSourceCodePath(void)
{
    if (language == 1)
        sourceCodePath.append("main.c");
    else if (language == 2 || language == 4)
        sourceCodePath.append("main.cpp");
    else if (language == 3)
    {
        sourceCodePath.append(mainClass);
        sourceCodePath.append(".java");
    }
}

void Judge::loadDB(unsigned int id)
{
    transaction t(db->begin());

    //load solution to memory
    std::auto_ptr <solution> recordSolution (db->load <solution> (id));

    problemId = recordSolution->getProblemId();
    sourceCodeId = recordSolution->getSourceCodeId();
    language = recordSolution->getLanguage();
    mainClass = recordSolution->getMainClass();
    user_id = recordSolution->getUserId();

    //load problem to memory
    std::auto_ptr <problem> recordProblem (db->load <problem> (problemId));

    timeLimit = recordProblem->getTimeLimit();
    memoryLimit = recordProblem->getMemoryLimit();
    diffOption = recordProblem->getDiffOption();

    //load source code to memory
    std::auto_ptr <source_code> recordSourceCode (db->load <source_code> (sourceCodeId));

    sourceCode = recordSourceCode->getSourceCode();

    //load test case to memory
    typedef odb::query <test_case> qTestCase;
    typedef odb::result <test_case> rTestCase;
    rTestCase recordTestCase (db->query <test_case> (qTestCase::problem_id == problemId));

    TESTCASE tmp;

    vTestCase.clear();

    for (rTestCase::iterator i (recordTestCase.begin()) ; i != recordTestCase.end() ; i++)
    {
        tmp.input = i->getInput();
        tmp.output = i->getOutput();
        tmp.id = i->getId();
        vTestCase.push_back(tmp);
    }

    std::auto_ptr <diff_option> recordDiffOption (db->load <diff_option> (diffOption));
    diffCommand = recordDiffOption->getCommand();

    t.commit();
}
