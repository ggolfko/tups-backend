#ifndef DEBUG_HXX
#define DEBUG_HXX

#include <string>
#include <cstddef>

#include <odb/core.hxx>

#pragma db object
class debug 
{
    private:
        debug () {}

        friend class odb::access;

        #pragma db id auto
        unsigned int id_;
        unsigned int compile_id_;
        std::string command_;
        std::string output_;
        std::string program_output_;
        unsigned int result_;
        unsigned int current_line_;
 
    public:
        void setCurrentLine(unsigned int current_line)
        {
            current_line_ = current_line;
        }

        const std::string getCommand(void) const
        {
            return command_;
        }

        const unsigned int getCompileId(void) const
        {
            return compile_id_;
        }

        void setProgramOutput(std::string program_output)
        {
            program_output_ = program_output;
        }

        void setResult(unsigned int result)
        {
            result_ = result;
        }
        void setOutput(std::string output)
        {
            output_ = output;
        }
};

#endif
