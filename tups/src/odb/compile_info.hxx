#ifndef COMPILEINFO_HXX
#define COMPILEINFO_HXX

#include <string>
#include <cstddef>

#include <odb/core.hxx>

#pragma db object
class compile_info
{
    private:
        compile_info () {}

        friend class odb::access;

        #pragma db id auto
        unsigned int id_;
        unsigned int solution_id_;
        std::string info_;
    public:
        compile_info(unsigned int solution_id, std::string &info)
            :solution_id_(solution_id), info_(info)
        {
        }
        const unsigned int getId() const
        {
            return id_;
        }
        void setSolutionId(unsigned int solution_id) 
        {
            solution_id_ = solution_id;
        }
        void setInfo(std::string info)
        {
            info_ = info;
        }
};

#endif
