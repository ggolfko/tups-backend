#include "BeanstalkSim.h"

BeanstalkSim::BeanstalkSim(const char *host, unsigned int port)
    :client(host, port)
{
    client.use("PSUOJSIM");
    client.watch("PSUOJSIM");
    client.ignore("default");
}

void BeanstalkSim::setTube(std::string tube)
{
    client.use(tube);
    client.watch(tube);
    client.ignore("default");
}

void BeanstalkSim::addJob(std::string solutionId)
{
    client.put(solutionId);
}

Beanstalk::Job BeanstalkSim::getJob(void)
{
    assert(client.reserve(job) && job);
    return job;
}

void BeanstalkSim::deleteJob(Beanstalk::Job injob)
{
    client.del(injob);
}
