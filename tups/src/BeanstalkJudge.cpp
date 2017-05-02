#include "BeanstalkJudge.h"
#include <iostream>
#include <cstdlib>

//init beanstalk client and test
BeanstalkJudge::BeanstalkJudge(const char *host, unsigned int port)
    :client(host, port)
{
}

void BeanstalkJudge::setTube(std::string tube)
{
    client.use(tube);
    client.watch(tube);
    client.ignore("default");
}

bool BeanstalkJudge::getDiff(void)
{
    return diff;
}

//fetch job queue
Beanstalk::Job BeanstalkJudge::getJob(void)
{
    assert(client.reserve(job) && job);
    return job;
}

//get solutionID from job
std::string BeanstalkJudge::getSolutionID(Beanstalk::Job job)
{
    std::string body = job.body();
    size_t semiPos[4] = {0};
    size_t quotePos[8] = {0};

    for (int i = 0 ; i < 4 ; i++)
    {
        if (i == 0)
            semiPos[i] = body.find(";");
        else
            semiPos[i] = body.find(";", semiPos[i-1]+1);

        if (semiPos[i] == std::string::npos)
        {
            throw "Job body invalid";
        }
    }

    for (int i = 0 ; i < 8 ; i++)
    {
        if (i == 0)
            quotePos[i] = body.find("\"");
        else
            quotePos[i] = body.find("\"", quotePos[i-1]+1);

        if (quotePos[i] == std::string::npos)
        {
            throw "Job body invalid";
        }
    }

    std::string tmpDiff;

    //diff
    tmpDiff = body.substr(quotePos[6]+1, semiPos[3]-quotePos[6]-2);

    if (tmpDiff == "true")
        diff = true;
    else
        diff = false;

    //solutionid
    return body.substr(quotePos[2]+1, semiPos[1]-quotePos[2]-2);
}

//delete job from jobID
void BeanstalkJudge::deleteJob(unsigned int jobID)
{
    client.del(jobID);
}

//delete job from job
void BeanstalkJudge::deleteJob(Beanstalk::Job job)
{
    client.del(job.id());
}
