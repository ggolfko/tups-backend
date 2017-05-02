#ifndef SOURCECODE_HXX
#define SOURCECODE_HXX

#include <string>
#include <cstddef>

#include <odb/core.hxx>

#pragma db object
class source_code
{
    private:

        friend class odb::access;

        #pragma db id auto
        unsigned int id_;
        std::string source_code_;
    public:
        source_code () {}

        unsigned int getID() const
        {
            return id_;
        }
        const std::string getSourceCode() const
        {
            return source_code_;
        }
};

#endif
