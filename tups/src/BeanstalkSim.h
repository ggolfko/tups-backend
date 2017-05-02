#ifndef BEANSTALKSIM_H
#define BEANSTALKSIM_H

#include "beanstalk.hpp"
#include <cstdlib>
#include <string>
#include <cassert>

class BeanstalkSim
{
private:
    Beanstalk::Client client;
    Beanstalk::Job job;
public:
    BeanstalkSim(char const *host, unsigned int port);
    void addJob(std::string solutionId);
    void setTube(std::string tube);
    Beanstalk::Job getJob(void);
    void deleteJob(Beanstalk::Job injob);

};

#endif
