
#include <es/es_tobj.h>
#include <stdio.h>

template <class T> es_tobj<T>::es_tobj( std::string name, T& _val ) : es_object(name){
    printf("assigning value.\n");
    v = _val;
}


template <class T> T* es_tobj<T>::val(){
    return &v;
}
