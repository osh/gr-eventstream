#include <es/es_exceptions.h>

EarlyEventException::EarlyEventException(const std::string m) : msg(m) {}
EarlyEventException::~EarlyEventException(){}
