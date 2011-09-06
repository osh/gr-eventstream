

#include <qa_es_source.h>
#include <cppunit/TestAssert.h>

#include <stdio.h>
#include <es/es.h>


// Test event generation, queue insertion, general non gr-runtime operation
void 
qa_es_source::t1()
{
    printf("t1\n");

    es_queue_sptr q = es_make_queue();
    pmt_t arb = es_make_arbiter();
    //es_source_sptr s = es_make_source(arb,q,sizeof(float));

    gr_io_signature_sptr outsig( gr_make_io_signature( 1, 1, sizeof(float) ) );
    es_source_sptr s = es_make_source(arb,q, outsig);
 
    pmt_t e1 = event_create( es::event_type_1, 1, 4 );
    pmt_t e2 = event_create( es::event_type_1, 10, 4 );
    pmt_t e3 = event_create( es::event_type_1, 20, 4 );
 
    q->add_event(e1);
    q->add_event(e2);
    q->add_event(e3);
 
    q->print();
     
}


#include <gnuradio/gr_top_block.h>
#include <gnuradio/gr_vector_sink_f.h>

// Test gr-runtime operation of single event item
void
qa_es_source::t2()
{

    printf("QA_ES_SOURCE::t2\n");
    es_queue_sptr q = es_make_queue();
    pmt_t arb = es_make_arbiter();

    gr_io_signature_sptr outsig( gr_make_io_signature( 1, 1, sizeof(float) ) );
    //es_source_sptr s = es_make_source(arb,q,sizeof(float));
    es_source_sptr s = es_make_source(arb,q,outsig);

    s->set_max(10);
 
    std::vector<float> const_ramp;
    const_ramp.push_back(1.0);
    const_ramp.push_back(0.75);
    const_ramp.push_back(0.5);
    const_ramp.push_back(0.25);
    
    // set up the event
    pmt_t e1_vector = pmt_init_f32vector( const_ramp.size(), &const_ramp[0] );
    pmt_t e1 = event_create_gen_vector_f( (uint64_t)3, e1_vector );   
    
    
//    es_event_sptr e1 = es_event_sptr(new event_gen_vector_f( arb, const_ramp ));   
//    e1->set_time(3);

    // set up handlers for the event
//    es_handler_sptr h1( new es_handler_print() );
//    es_handler_sptr h2( new es_handler_insert_vector_f() );

    // register the event
//    q->register_event_type(e1->type());

    // bind handlers to the event
//    q->bind_handler( e1->type(), h1 );
//    q->bind_handler( e1->type(), h2 );

    // manually add event to queue
    q->add_event(e1);
    
    printf("QA_ES_SOURCE::t200CMT BARRIER\n");

    
    gr_top_block_sptr tb = gr_make_top_block("t2 graph");
    gr_vector_sink_f_sptr vs = gr_make_vector_sink_f();
 
    tb->connect( s, 0, vs, 0 );
    printf("running flowgraph.\n");
    tb->run();
 
    std::vector<float> out_data = vs->data();
    printf("total_output_vector=[");
    for(int i=0; i<out_data.size(); i++){
        printf("%f,", out_data[i]);
    }
    printf("]\n");
    
}




