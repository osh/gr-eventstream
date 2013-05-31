/* -*- c++ -*- */
/*
 * Copyright 2011 Free Software Foundation, Inc.
 * 
 * This file is part of gr-eventstream
 * 
 * gr-eventstream is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * gr-eventstream is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with gr-eventstream; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>
#include <boost/lockfree/queue.hpp>
#include <pmt/pmt.h>
#include <es/es_queue.h>
#include <es/es_common.h>
#include <es/es_eh_pair.hh>
#include <semaphore.h>

using namespace pmt;

class es_event_loop_thread {
    
    public:
    
        //es_event_loop_thread();
        es_event_loop_thread(pmt_t _arb, es_queue_sptr _queue, boost::lockfree::queue<es_eh_pair*> *qq, boost::lockfree::queue<unsigned long long> *dq, boost::condition *qq_cond, boost::atomic<int> *nevents);
        void start();
        void stop();
        void do_work();
        boost::atomic<int> *d_nevents;

    private:
        pmt_t arb;
        es_queue_sptr queue;

        bool finished;       
        boost::shared_ptr<boost::thread> d_thread;        

        //boost::lockfree::queue<pmt_t*> *qq;

        boost::condition *qq_cond;

        boost::lockfree::queue<es_eh_pair*> *qq;
        boost::lockfree::queue<unsigned long long> *dq;

        void eh_run(pmt_t eh);
        sem_t* thread_notify_sem;

};


