

#include <qa_es_common.h>
#include <cppunit/TestAssert.h>

#include <stdio.h>
#include <es/es.h>





// Test event generation, queue insertion, handler binding, general non gr-runtime operation
void 
qa_es_common::t1()
{
    printf("t1\n");
    es_queue_sptr q = es_make_queue();
    pmt_t arb = es_make_arbiter();

    pmt_t evt1 = event_create( es::event_type_1, 1, 4 );
    pmt_t evt2 = event_create( es::event_type_1, 100, 10 );
    pmt_t evt3 = event_create( "custom_evt", 200, 10 );
    event_print (evt1 );
    printf("event time = %llu\n", event_time( evt1 ));
    printf("event type = %s\n", event_type( evt2 ).c_str());
    printf("event type = %s\n", event_type( evt3 ).c_str());

    q->register_event_type( event_type( evt1 ) );
    q->register_event_type( event_type( evt2 ) );
    q->register_event_type( event_type( evt3 ) );

    pmt_t h1 = make_handler_pmt( new es_handler_print(es_handler_print::TYPE_F32) );
    pmt_t h2 = make_handler_pmt( new es_handler_print(es_handler_print::TYPE_C32) );

    q->bind_handler( event_type(evt1), h1 );
    q->bind_handler( event_type(evt3), h2 );

    q->add_event(evt1);
    q->add_event(evt2);
    q->add_event(evt3);

    q->print();

}



