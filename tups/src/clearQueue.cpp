#include <iostream>
#include <cstdlib>
#include <csignal>

#include "beanstalk.hpp"

using namespace std;
using namespace Beanstalk;

void signalHandler(int signum);

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        cerr << "Usage: ./createQueue <host> <port> <tube>" << endl;
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, signalHandler);

    Client client(argv[1], atoi(argv[2]));

    client.use(argv[3]);
    client.watch(argv[3]);

    Job job;

    while (true)
    {
        if (client.reserve(job) && job)
        {
            cout << "Delete job id: " << job.id() << endl;
            client.del(job.id());
        }
    }

    exit(EXIT_SUCCESS);
}

void signalHandler(int signum)
{
    if (signum == SIGINT)
    {
        cout << "Terminated" << endl;
        exit(EXIT_SUCCESS);
    }
}

