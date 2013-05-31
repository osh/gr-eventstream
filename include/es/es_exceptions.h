#ifndef ES_EXCEPTIONS_H
#define ES_EXCEPTIONS_H

#include <stdexcept>
#include <string>

class EarlyEventException {
    public:
        EarlyEventException(const std::string m = "early event exception!");
        ~EarlyEventException();
        const char* what(){ return msg.c_str();}
    private:
        std::string msg;
    };



#endif
