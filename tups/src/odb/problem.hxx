#ifndef PROBLEM_HXX
#define PROBLEM_HXX

#include <string>
#include <cstddef>

#include <odb/core.hxx>

#pragma db object
class problem 
{
    private:
        problem () {}

        friend class odb::access;

        #pragma db id auto
        unsigned int id_;
        unsigned int time_limit_;
        unsigned int memory_limit_;
        unsigned int diff_option_;
    public:
        const unsigned int getId() const
        {
            return id_;
        }
        const unsigned int getTimeLimit() const
        {
            return time_limit_;
        }
        const unsigned int getMemoryLimit() const
        {
            return memory_limit_;
        }
        const unsigned int getDiffOption() const
        {
            return diff_option_;
        }
};

#endif
