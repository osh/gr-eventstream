
#include <stdio.h>
#include <es/es_pyhandler_def.h>

es_pyhandler_def::es_pyhandler_def(){
    printf("es_pyhandler_def\n");
}

void es_pyhandler_def::set_top_block(gr_top_block_sptr _tb){
    tb = _tb;
}



