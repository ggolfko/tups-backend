#ifndef DIFFOPTION_HXX
#define DIFFOPTION_HXX

#include <string>
#include <cstddef>

#include <odb/core.hxx>

#pragma db object
class diff_option
{
    private:
        diff_option () {}

        friend class odb::access;

        #pragma db id auto
        unsigned int id_;
        std::string option_;
        std::string command_;
    public:
        const unsigned int getId() const
        {
            return id_;
        }
        const std::string getCommand() const
        {
            return command_;
        }
};

#endif
