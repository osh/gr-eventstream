
#include <es/es_eh_pair.hh>
#include <es/es_common.h>
#include <stdio.h>

es_eh_pair::es_eh_pair(pmt_t _event, pmt_t _handler) :
    handler(_handler), 
    event(_event)
    {

}

void es_eh_pair::run(){
    printf("run()\n");
    boost::shared_ptr<gruel::msg_accepter> h = pmt_msg_accepter_ref( handler );
    h->post( event );
}


unsigned long long es_eh_pair::time(){
    return event_time( event );
}

unsigned long long es_eh_pair::length(){
    return event_length( event );
}

es_eh_pair::~es_eh_pair(){
    printf("es_eh_pair::destructor running.\n");
}


