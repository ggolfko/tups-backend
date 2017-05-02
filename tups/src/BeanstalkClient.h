#ifndef BEANSTALKCLIENT_H
#define BEANSTALKCLIENT_H

#include "beanstalk.hpp"
#include <cstdlib>
#include <string>
#include <cassert>
#include <algorithm>

class BeanstalkClient
{
private:
    Beanstalk::Client client;
    Beanstalk::Job job;
public:
    BeanstalkClient(const char *host, unsigned int port);
    Beanstalk::Job getJob(void);
    void setTube(std::string tube);
    void deleteJob(unsigned int);
    void deleteJob(Beanstalk::Job);
    std::string getId(Beanstalk::Job);
    void clearJob(void);
};

#endif
