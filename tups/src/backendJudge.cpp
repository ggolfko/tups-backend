#include <iostream>
#include <cstdlib>
#include <string>
#include <cstdio>
#include <fstream>
#include <sstream>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include "BeanstalkJudge.h"
#include "judge.h"
#include "config.h"

using namespace std;
using namespace Beanstalk;

void workerTask(BeanstalkJudge &, Job &, CONFIG);

int main()
{
    CONFIG config;

    try
    {
        loadConfig(config);
        BeanstalkJudge bClient(config.BEANSTALKHOST.c_str(), 11300);
        bClient.setTube(config.TUBEJUDGE);

        while (true)
        {
            //get job for beanstalk
            Job job = bClient.getJob();

#ifdef DEBUG
            cout << "Solution id: " << bClient.getSolutionID(job) << endl;
#endif

            workerTask(bClient, job, config);

            usleep(500000);
        }
    }
    catch (const char *e)
    {
        cerr << e << endl;
    }

    cerr << "terminated" << endl;

    exit(EXIT_SUCCESS);
}

void workerTask(BeanstalkJudge &bClient, Job &job, CONFIG config)
{
    //create directory with pid
    char workDIR[20];
    string strWorkDIR;

    sprintf(workDIR, "%d", getpid());
    strWorkDIR.append(workDIR);

    unsigned int solutionId = atoi(bClient.getSolutionID(job).c_str());

    if (mkdir(workDIR, 0755) == -1)
        throw "Error when create directory";

    //get solution id
    try
    {
        Judge judge(solutionId, strWorkDIR, config, bClient.getDiff());
        judge.run();
    }
    catch (const odb::exception &e)
    {
        cerr << e.what() << endl;
    }
    catch (const char *e)
    {
        cerr << e << endl;
    }

    //TODO remove when release
    string cmd = "rm -R ";
    cmd.append(workDIR);
    system(cmd.c_str());

    //TODO
    bClient.deleteJob(job);
}
