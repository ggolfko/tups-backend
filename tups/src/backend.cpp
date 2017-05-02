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

#include "BeanstalkClient.h"
#include "compile.h"
#include "config.h"

using namespace std;
using namespace Beanstalk;

void workerTask(BeanstalkClient &, Job &, CONFIG);

int main()
{
    CONFIG config;

    try
    {
        loadConfig(config);
        BeanstalkClient bClient(config.BEANSTALKHOST.c_str(), 11300);
        bClient.setTube(config.TUBECOMPILE);

        while (true)
        {
            //get job for beanstalk
            Job job = bClient.getJob();

#ifdef DEBUG
            cout << "Solution id: " << bClient.getId(job) << endl;
#endif

            workerTask(bClient, job, config);

            usleep(500000);
        }
    }
    catch (const char *e)
    {
        cerr << e << endl;
    }

    exit(EXIT_SUCCESS);
}

void workerTask(BeanstalkClient &bClient, Job &job, CONFIG config)
{
    //create directory with pid
    char workDIR[20];
    string strWorkDIR;

    sprintf(workDIR, "%d", getpid());
    strWorkDIR.append(workDIR);

    if (mkdir(workDIR, 0755) == -1)
        throw "Error when create directory";

    //get solution id
    string compileId = bClient.getId(job);

    try
    {
        Compile objCompile(atoi(compileId.c_str()), strWorkDIR, config);
        objCompile.run();
    }
    catch (const odb::exception &e)
    {
        cerr << e.what() << endl;
    }
    catch (const char *e)
    {
        cerr << e << endl;
    }

    string cmd = "rm -R ";
    cmd.append(workDIR);
    system(cmd.c_str());

    //TODO
    bClient.deleteJob(job);
}
