#ifndef SOLUTION_HXX
#define SOLUTION_HXX

#include <string>
#include <cstddef>

#include <odb/core.hxx>

#pragma db object
class solution 
{
    private:
        //solution () {}

        friend class odb::access;

        #pragma db id auto
        unsigned int id_;

        unsigned int source_code_id_;
        unsigned int problem_id_;
        unsigned int user_id_;
        unsigned int time_used_;
        unsigned int memory_used_;
        unsigned int language_;
        std::string main_class_;
        unsigned int result_;
        unsigned int wait_diff_;
        float ratio_;
    public:
        solution () {}

        void setLanguage(unsigned int language)
        {
            language_ = language;
        }
        void setFinishDiff(void)
        {
            wait_diff_ = 0;
        }
        void setResult(unsigned int result)
        {
            result_ = result;
        }
        void setTime(unsigned int time)
        {
            time_used_ = time;
        }
        void setRatio(float ratio)
        {
            ratio_ = ratio;
        }
        void setMemory(unsigned int memory)
        {
            memory_used_ = memory;
        }
        void setId(unsigned int id)
        {
            id_ = id;
        }
        void setSourceCodeId(unsigned int source_code_id)
        {
            source_code_id_ = source_code_id;
        }
        const unsigned int getSourceCodeId() const
        {
            return source_code_id_;
        }
        const unsigned int getId() const
        {
            return id_;
        }
        const unsigned int getProblemId() const
        {
            return problem_id_;
        }
        const unsigned int getUserId() const
        {
            return user_id_;
        }
        const unsigned int getMemoryUsed() const
        {
            return memory_used_;
        }
        const unsigned int getLanguage() const
        {
            return language_;
        }
        
        const std::string &getMainClass() const
        {
            return main_class_;
        }

        const unsigned int getResult() const
        {
            return result_;
        }
        const float getRatio() const
        {
            return ratio_;
        }
};

#pragma db view object(solution) query ((?))
struct NSOLVED 
{
    #pragma db column("count(distinct(" + solution::problem_id_ + "))")
    unsigned int solved;
};

#pragma db view object(solution) query ((?))
struct NSUBMIT
{
    #pragma db column("count(" + solution::id_ +  ")")
    unsigned int submit;
};

#pragma db object
class user
{
    private:
        user () {}

        friend class odb::access;

        #pragma db id auto
        unsigned int id_;

        unsigned int submit_;
        unsigned int solved_;
    public:
        void setSubmit(unsigned int submit)
        {
            submit_ = submit;
        }
        void setSolved(unsigned int solved)
        {
            solved_ = solved;
        }
};

#endif
