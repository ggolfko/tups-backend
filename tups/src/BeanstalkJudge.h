#ifndef BEANSTALKJUDGE_H
#define BEANSTALKJUDGE_H

#include "beanstalk.hpp"
#include <cstdlib>
#include <string>
#include <cassert>
#include <algorithm>

class BeanstalkJudge
{
private:
    Beanstalk::Client client;
    Beanstalk::Job job;
    bool diff;
public:
    BeanstalkJudge(const char *, unsigned int);
    Beanstalk::Job getJob(void);
    void setTube(std::string tube);
    bool getDiff(void);
    void deleteJob(unsigned int);
    void deleteJob(Beanstalk::Job);
    std::string getSolutionID(Beanstalk::Job);
};

#endif
