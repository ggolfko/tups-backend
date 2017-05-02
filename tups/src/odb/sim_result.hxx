#ifndef SIMRESULT_HXX
#define SIMRESULT_HXX

#include <string>
#include <cstddef>

#include <odb/core.hxx>

#pragma db object
class sim_result
{
    private:

        friend class odb::access;

        #pragma db id auto
        unsigned int id_;
        unsigned int first_solution_id_;
        unsigned int second_solution_id_;
        unsigned int percent_;
    public:
        sim_result() {}
        sim_result(unsigned int first_solution_id, unsigned int second_solution_id, unsigned int percent)
        {
            first_solution_id_ = first_solution_id;
            second_solution_id_ = second_solution_id;
            percent_ = percent;
        }
        void setPercent(unsigned int percent)
        {
            percent_ = percent;
        }
        void setSecondSolutionId(unsigned int second_solution_id)
        {
            second_solution_id_ = second_solution_id;
        }
        void setFirstSolutionId(unsigned int first_solution_id)
        {
            first_solution_id_ = first_solution_id;
        }
};

#endif
