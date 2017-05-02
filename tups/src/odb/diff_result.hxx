#ifndef DIFFRESULT_HXX
#define DIFFRESULT_HXX

#include <string>
#include <cstddef>

#include <odb/core.hxx>

#pragma db object 
class diff_result 
{
    private:
        diff_result () {}

        friend class odb::access;
        
        #pragma db id auto
        unsigned int id_;
        unsigned int solution_id_;
        unsigned int test_case_id_;
        std::string output_;
    public:
        diff_result(unsigned int solution_id, unsigned int test_case_id, std::string &output)
            :solution_id_(solution_id), test_case_id_(test_case_id), output_(output)
        {
        }
        void setOutput(std::string &output)
        {
            output_ = output;
        }
};

#endif
