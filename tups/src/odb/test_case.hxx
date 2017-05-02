#ifndef TESTCASE_HXX
#define TESTCASE_HXX

#include <string>
#include <cstddef>

#include <odb/core.hxx>

#pragma db object
class test_case 
{
    private:
        test_case () {}

        friend class odb::access;

        #pragma db id auto
        unsigned int id_;
        unsigned int problem_id_;
        std::string input_;
        std::string output_;
    public:
        const unsigned int getId() const
        {
            return id_;
        }
        const unsigned int getProblemId() const
        {
            return problem_id_;
        }
        const std::string getOutput() const
        {
            return output_;
        }
        const std::string getInput() const
        {
            return input_;
        }
};

#endif
