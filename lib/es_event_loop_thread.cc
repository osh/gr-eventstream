
#include <stdio.h>
#include <es/es_common.h>
#include <es/es_event_loop_thread.hh>

/*
 * Constructor function, sets up parameters
 */
es_event_loop_thread::es_event_loop_thread(pmt_t _arb, es_queue_sptr _queue, boost::lockfree::fifo<es_eh_pair*> *_qq, boost::lockfree::fifo<unsigned long long> *_dq, boost::condition *_qq_cond) :
    arb(_arb),
    queue(_queue),
    qq(_qq),
    dq(_dq),
    qq_cond(_qq_cond),
    finished(false)
{
    start();
}

/*
 * kick off a thread running the do_work() main loop function
 */
void es_event_loop_thread::start(){
    d_thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&es_event_loop_thread::do_work, this)));
}

/* 
 * Shut down all the running threads and join them.
 *   Called by es_sink destructor
 */
void es_event_loop_thread::stop(){
    finished = true;
    qq_cond->notify_all();
    d_thread->join();
}


/*
 *  Main event loop thread work function,
 *    constantly receives and services event/handler pairs
 *    then queues the live_times for deletion.
 */
void es_event_loop_thread::do_work(){

    // used by boost::condition, has no scope outside of this thread, unneccisary
    boost::mutex access;
    boost::mutex::scoped_lock lock(access);

    // run the thread until we are told to shut down
    while(!finished){
        es_eh_pair* eh = NULL;

        // wait for a signal to start looking in the queue
        qq_cond->wait(lock);

        // get events to handle as long as they are available
        while( (*qq).dequeue(&eh) ){

            //if(finished){ return; }
            printf("dequeue returned.\n");

            // run the event/handler pair
            eh->run();

            // enqueue the time marker for deletion
            (*dq).enqueue( eh->time() );

            // delete the reference
            delete eh;
        }
    }
}






