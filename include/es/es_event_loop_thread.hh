
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>
#include <boost/lockfree/fifo.hpp>
#include <gruel/pmt.h>
#include <es/es_queue.h>
#include <es/es_common.h>
#include <es/es_eh_pair.hh>
#include <semaphore.h>

using namespace pmt;

class es_event_loop_thread {
    
    public:
    
        //es_event_loop_thread();
        es_event_loop_thread(pmt_t _arb, es_queue_sptr _queue, boost::lockfree::fifo<es_eh_pair*> *qq, boost::lockfree::fifo<unsigned long long> *dq, boost::condition *qq_cond);
        void start();
        void stop();
        void do_work();

    private:
        pmt_t arb;
        es_queue_sptr queue;

        bool finished;       
        boost::shared_ptr<boost::thread> d_thread;        

        //boost::lockfree::fifo<pmt_t*> *qq;

        boost::condition *qq_cond;

        boost::lockfree::fifo<es_eh_pair*> *qq;
        boost::lockfree::fifo<unsigned long long> *dq;

        void eh_run(pmt_t eh);
        sem_t* thread_notify_sem;

};


