#ifndef EVENTSTREAM_TOBJ_H
#define EVENTSTREAM_TOBJ_H

#include <es/es_object.h>

template<class T>
class es_tobj : public es_object {
    
    public: 
        es_tobj( std::string name, T& _val );
        T* val();
        T v;

};

template class es_tobj<std::string>;



#endif
