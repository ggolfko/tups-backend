#ifndef COMPILE_HXX
#define COMPILE_HXX

#include <string>
#include <cstddef>

#include <odb/core.hxx>

#pragma db object
class compile 
{
    private:
        compile () {}

        friend class odb::access;

        #pragma db id auto
        unsigned int id_;

        unsigned int user_id_;
        unsigned int source_code_;
        unsigned int result_;
        unsigned int language_;
        std::string main_class_;
        std::string input_;
        std::string output_;
        unsigned int time_used_;
        unsigned int memory_used_;
        std::string compile_info_;
 
    public:
        void setCompileInfo(std::string compile_info)
        {
            compile_info_ = compile_info;
        }
        void setMemoryUsed(unsigned int memory_used)
        {
            memory_used_ = memory_used;
        }
        void setTimeUsed(unsigned int time_used)
        {
            time_used_ = time_used;
        }
        void setOutput(std::string output)
        {
            output_ = output;
        }
        const std::string& getInput(void) const
        {
            return input_;
        }
        const std::string& getMainClass(void) const
        {
            return main_class_;
        }
        const unsigned int getLanguage(void) const
        {
            return language_;
        }
        void setResult(unsigned int result)
        {
            result_ = result;
        }
        const unsigned int getSourceCode(void) const
        {
            return source_code_;
        }
};

#endif
