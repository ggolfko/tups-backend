#include <cstdlib>
#include <iostream>

#include "config.h"
#include "sim.h"

#define MAXCLIENT 1

using namespace std;

void workerTask(CONFIG);

int main()
{
    CONFIG config;

    try
    {
        loadConfig(config);
        workerTask(config);
    }
    catch (const odb::exception &e)
    {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }
    catch (const char *e)
    {
        cerr << e << endl;
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}

void workerTask(CONFIG config)
{
    Sim sim(config);
    sim.run();
}

