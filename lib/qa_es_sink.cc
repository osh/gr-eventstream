

#include <qa_es_sink.h>
#include <cppunit/TestAssert.h>

#include <stdio.h>
#include <es/es.h>


#include <gnuradio/gr_top_block.h>
#include <gnuradio/gr_vector_source_f.h>

// Test gr-runtime operation of single event item
void
qa_es_sink::t1()
{
    printf(" *** BEGIN QA_ES_SINK_T1\n");
 
    // build input test vector
    std::vector<float> vec;
    vec.push_back(0);
    vec.push_back(0);
    vec.push_back(0);
    vec.push_back(4);
    vec.push_back(3);
    vec.push_back(2);
    vec.push_back(1);
    vec.push_back(0);
    vec.push_back(0);
    vec.push_back(0);
    vec.push_back(0);
    
    // create eventstream control objects
    pmt_t arb = es_make_arb();
    es_queue_sptr queue = es_make_queue();
    
    // build gnu radio blocks
    gr_top_block_sptr tb = gr_make_top_block("qa_es_sink_t1_top");
    gr_vector_source_f_sptr src = gr_make_vector_source_f(vec);
    
    gr_io_signature_sptr insig( gr_make_io_signature( 1, 1, sizeof(float) ) );
    es_sink_sptr snk = es_make_sink( arb, queue, insig, 1 );
    
    // connect traditional flowgraph portion
    tb->connect( src, 0, snk, 0 );
    
    pmt_t evt1 = event_create("NEW_EVT_TYPE_3_4", 3, 4);
    pmt_t evt2 = event_create("NEW_EVT_TYPE_5_4", 5, 4);

    pmt_t h1 = make_handler_pmt( new es_handler_print(es_handler_print::TYPE_F32) );
    
    // register the NEW_EVT_TYPE as a known event type handled by this queue
    queue->register_event_type( event_type( evt1 ) );

    // bind a handler to the NEW_EVT_TYPE
    queue->bind_handler( event_type( evt1 ), h1 );

    // to define this event type and not discard it run ...
//    queue->register_event_type( event_type( evt2 ) );
//    queue->bind_handler( event_type( evt2 ), h1 );

    // add the two events to the queue
    queue->add_event(evt1);
    queue->add_event(evt2);
 
    printf("tb->run()\n");
      
    // run the waveform
    tb->run();
    
    printf(" *** END QA_ES_SINK_T1\n");
}


