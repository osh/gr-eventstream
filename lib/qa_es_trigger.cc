

#include <qa_es_trigger.h>
#include <cppunit/TestAssert.h>

#include <stdio.h>
#include <es/es.h>


#include <gnuradio/gr_top_block.h>
#include <gnuradio/gr_vector_source_f.h>

// Test gr-runtime operation of single event item
void
qa_es_trigger::t1()
{
    printf(" *** BEGIN QA_ES_TRIGGER_T1\n");
 
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
    gr_top_block_sptr tb = gr_make_top_block("qa_es_trigger_t1_top");
    gr_vector_source_f_sptr src = gr_make_vector_source_f(vec);

    gr_io_signature_sptr insig( gr_make_io_signature( 1, 1, sizeof(float) ) );
    es_sink_sptr snk = es_make_sink( arb, queue, insig, 1 );
//    es_sink_sptr snk = es_make_sink( arb, queue, sizeof(float) );

    es_trigger_edge_f_sptr trig = es_make_trigger_edge_f(arb, queue, 0.1, 4, 1);

    // connect traditional flowgraph portion
    tb->connect( src, 0, trig, 0 );
    tb->connect( trig, 0, snk, 0 );
    
    // create the event handler, just a print handler in this case   
    pmt_t h1 = make_handler_pmt( new es_handler_print(es_handler_print::TYPE_F32) );
    
    // register the trigger blocks event type as a known event type handled by this queue
    queue->register_event_type( trig->event_type(0) );

    // bind a handler to the trigger block's first output event type
    queue->bind_handler( trig->event_type(0), h1 );

    // run the waveform
    tb->run();

    printf(" *** END QA_ES_TRIGGER_T1\n");
}


