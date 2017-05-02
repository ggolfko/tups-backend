#include "sim.h"

#ifdef DEBUG
#include <iostream>
#endif

Sim::Sim(CONFIG config)
    :db(new odb::mysql::database(config.USERNAME, config.PASSWORD, config.DBNAME, config.DBHOST)), bClient(config.BEANSTALKHOST.c_str(), 11300)
{
    bClient.setTube(config.TUBESIM);
    notTerminate = true;
    running = false;
}

void Sim::run(void)
{
    unsigned int mainSolutionId;
    running = true;

    while (notTerminate)
    {
        job = bClient.getJob();

        createWorkDir();

        mainSolutionId = atoi(job.body().c_str());

#ifdef DEBUG
        std::cout << "Check sim solution id: " << mainSolutionId << std::endl;
#endif

        //load mainsolution
        getMainSolution(mainSolutionId);

        runSim();

        bClient.deleteJob(job);

        clearWorkDir();
    }

    running = false;
}

void Sim::terminate(void)
{
    notTerminate = false;
}

bool Sim::isRunning(void)
{
    return running;
}

void Sim::updateResult(void)
{
    typedef odb::query <sim_result> query;
    typedef odb::result <sim_result> result;

    //TODO set minimum percent
    if (maxPercent >= 80)
    {
        //insert or update database
        sim_result record(mainSolution.getId(), maxSolutionId, maxPercent);

        transaction t(db->begin());

        result r(db->query <sim_result> (query::first_solution_id == mainSolution.getId()));

        if (r.begin() == r.end())
            db->persist(record);
        else
        {
            std::auto_ptr <sim_result> orecord(r.begin().load());
            orecord->setSecondSolutionId(maxSolutionId);
            orecord->setPercent(maxPercent);
            db->update(*orecord);
        }

        t.commit();
    }
}

void Sim::runSim(void)
{
    typedef odb::query <solution> qsolution;
    typedef odb::result <solution> rsolution;

    transaction t(db->begin());

    maxPercent = 0;

    rsolution r(db->query <solution> (qsolution::result == AC && qsolution::problem_id == mainSolution.getProblemId() && qsolution::user_id != mainSolution.getUserId() && qsolution::id < mainSolution.getId()));

    for (rsolution::iterator i(r.begin()) ; i != r.end() ; i++)
    {
        //load source code
        loadSourceCode(i->getSourceCodeId());

        //set source code path
        convert.str("");
        convert << i->getSourceCodeId();

        std::string sourceCodePath = workDirPath;
        sourceCodePath.append(convert.str());
        saveToFile(tempSourceCode.getSourceCode(), sourceCodePath);

        std::string cmd;

        if (language == 3)
            cmd = "./sim_java -p";
        else
            cmd = "./sim_c -p";
        cmd.append(" ");
        cmd.append(mainSourceCodePath);
        cmd.append(" ");
        cmd.append(sourceCodePath);
        cmd.append(" ");
        cmd.append("|awk '{print $4}'");
        cmd.append(" > ");
        cmd.append(simResultPath);

        system(cmd.c_str());

        std::ifstream simResultFile(simResultPath.c_str());
        if (!simResultFile.is_open())
            throw "Sim result file not found";

        std::string tmpResult;
        unsigned int tempPercent;
        while (getline(simResultFile, tmpResult))
        {
            tempPercent = atoi(tmpResult.c_str());
            if (maxPercent < tempPercent)
            {
                maxPercent = tempPercent;
                maxSolutionId = i->getId();
            }
        }

        simResultFile.close();
    }

    t.commit();

    updateResult();
}


void Sim::saveToFile(std::string data, std::string path)
{
    std::ofstream outfile;
    outfile.open(path.c_str());

    if (!outfile.is_open())
        throw "Error when create file";

    outfile << data;
    outfile.close();
}

void Sim::getMainSolution(unsigned int id)
{
    transaction t(db->begin());
    //get solution
    loadSolution(id);
    mainSolution = tempSolution;
    mainSourceCodePath = workDirPath;
    mainSourceCodePath.append("main");
    simResultPath = workDirPath;
    simResultPath.append("sim");
    language = mainSolution.getLanguage();

    //get sourcecode
    loadSourceCode(mainSolution.getSourceCodeId());
    saveToFile(tempSourceCode.getSourceCode(), mainSourceCodePath);

    t.commit();
}

void Sim::loadSourceCode(unsigned int id)
{
    std::auto_ptr <source_code> recordSourceCode(db->load <source_code> (id));
    tempSourceCode = *recordSourceCode;
}

void Sim::loadSolution(unsigned int id)
{
    std::auto_ptr <solution> recordSolution(db->load <solution> (id));

    tempSolution = *recordSolution;
}

void Sim::createWorkDir(void)
{
    convert.str("");
    convert << getpid();

    workDirPath = convert.str();
    workDirPath.append("s");
    workDirPath.append("/");

    if (mkdir(workDirPath.c_str(), 0755) == -1)
        throw "Error when create directory";
}

void Sim::clearWorkDir(void)
{
    std::string cmd = "rm -R ";
    cmd.append(workDirPath);
    system(cmd.c_str());
}
