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

#include <es/es_queue.h>
#include <es/es_common.h>
#include <es/es_exceptions.h>

#include <limits.h>
#include <stdio.h>

#define DEBUG(X) 
//#define DEBUG(X)  X

es_queue_sptr es_make_queue(es_queue_early_behaviors eb){
    return es_queue_sptr(new es_queue(eb));
}

es_queue::es_queue(es_queue_early_behaviors eb) :
    d_early_behavior(eb)
{
    bindings = pmt::make_dict();
}

void es_queue::set_early_behavior(enum es_queue_early_behaviors v){
    d_early_behavior = v;
}

int es_queue::length(){
    queue_lock.lock();
    int l = event_queue.size();
    queue_lock.unlock();
    return l;
}

int es_queue::add_event(pmt_t evt){

//    printf("WARNING: currently events must be added to the queue after binding it to a source block to avoid issues ... the add callback must be first defined\n");
//    DEBUG(printf("es_queue::add_event...\n");)

    std::map< std::string, std::vector<pmt_t> >::iterator it;
    
//    printf("add_event of type :: %s\n", event_type(evt).c_str() );

    bool found = pmt::dict_has_key(bindings, pmt::intern(event_type(evt)));
    if(!found){
        printf("WARNING: attempted to add event to queue of type which we are unaware! Discarding\n");
        event_print(evt);
        return -1;
        }

    pmt_t keys = pmt::dict_keys(bindings);
    while(pmt::is_pair(keys)){
//        printf("entry: %s\n", pmt_symbol_to_string(pmt_car(keys)).c_str() );
//        int a = pmt_eqv( pmt_car(keys), event_type_pmt(evt) );
//        printf("pmt_eqv = %d\n", a); 
        keys = pmt::cdr(keys);
        }

    pmt_t handlers = pmt::dict_ref(bindings, event_type_pmt(evt), PMT_NIL);
//    printf("handlers.size() = %d\n", (int)pmt_length(handlers));

    // TODO: We may not really want to check this at runtime ... 
    if(pmt::length(handlers) == 0){
        printf("WARNING: attempted to add event to queue for which no handler is defined!\n");
        }
    
    if(event_time(evt) == ULLONG_MAX){
        fprintf(stderr, "WARNING: event recieved with unset time. (%s)\n", event_type(evt).c_str());
    }
    int idx;

    queue_lock.lock();

    for(idx=0; (idx < event_queue.size() && event_time(evt) > event_queue[idx]->time() ); idx++){ }

    //for(int i=0; i<handlers.size(); i++){

    while(pmt::is_pair(handlers)){
        es_eh_pair* eh_pair = new es_eh_pair( evt, pmt::car(handlers) );
        
        DEBUG(printf("created new eh_pair = %x\n", eh_pair);)
        DEBUG(printf("evt = %s\n",  pmt::write_string(evt).c_str());)
        DEBUG(printf("handler = %s\n", pmt::write_string(pmt::car(handlers)).c_str());)
        DEBUG(printf("is any = %d, is ma = %d\n", pmt::is_any(pmt::car(handlers)), pmt::is_msg_accepter(pmt::car(handlers)));)

        // by default we add to queue
        bool append_pair = true;


        // TODO: when the flowgraph is not yet running, we should not call callbacks yet!!
        // they may need to be queued for start() somehow.... (or we could not start the thread pool until then ... )

        // call any callbacks bound with each new eh pair
        // if any return false, suppress addition to queue
        for(int i=0; i < cb_list.size(); i++ ){
            bool rv = cb_list[i](&eh_pair);
            append_pair = append_pair | rv;
        }

        // conditionally add the eh pair to the queue
        if(append_pair){
            event_queue.insert(event_queue.begin()+idx, eh_pair); 
        }

        // iterate to next handler
        handlers = pmt::cdr(handlers);

    }
    queue_lock.unlock();
    
}


void es_queue::print_queue(bool already_holding){

    printf("EVENTSTREAM_BINDINGS...\n");

    std::map<std::string, std::vector< pmt_t> >::iterator it;
    
/*    //iterate over all keys in bindings
    for(it = bindings.begin(); it != bindings.end(); it++){
        printf(" * EVENT: (%s) # Handlers = %d\n", (*it).first.c_str(), (*it).second.size());
        //iterate over list items in each key value
        for(int i=0; i<(*it).second.size(); i++){
            //print the binding to screen
            printf("   * Handler--> ");
            printf("%s\n", pmt_write_string( (*it).second[i] ).c_str() );
        }
    }*/
    printf("%s\n", pmt::write_string( bindings ).c_str() );

    if(!already_holding)
        queue_lock.lock();

    printf("EVENTSTREAM_QUEUE (size = %lu)\n", event_queue.size());
    for(int i=0; i<event_queue.size(); i++){
        //event_queue[i].print();
    }

    if(!already_holding)
        queue_lock.unlock();

}

int es_queue::register_event_type(pmt_t type){
    register_event_type( pmt::symbol_to_string(type) );
}
    
int es_queue::register_event_type(std::string type){

    pmt_t type_pmt = pmt::intern(type);
    DEBUG(printf("es_queue::register_event_type...\n");)
/*    std::map< std::string, std::vector<pmt_t> >::iterator it;
    it = bindings.find( type );
    if(it != bindings.end() ){
        printf("WARNING: type already registered (%s)\n", type.c_str());
        return -1;
    } 
    
    std::vector<pmt_t> emptylist;
    bindings[type] =  emptylist; */
    bool already_reg = pmt::dict_has_key(bindings, type_pmt);
    if(already_reg){
        printf("WARNING: type already registered (%s)\n", type.c_str());
    } else {
        bindings = pmt::dict_add(bindings, type_pmt, PMT_NIL);
    }
    return 0;
}

void es_queue::bind_handler(pmt_t type, gr::basic_block_sptr handler){
    bind_handler( pmt::symbol_to_string(type), handler);
    }

void es_queue::bind_handler(std::string type, gr::basic_block_sptr handler){


    es_handler_sptr h = boost::dynamic_pointer_cast<es_handler>(handler);
    pmt_t handler_pmt = pmt::make_any( (es_handler*) h.get() );
    
    pmt_t type_pmt = pmt::intern(type);

    DEBUG(printf("EVENTSTREAM_QUEUE::BIND_HANDLER (%s, %x).\n",type.c_str(), handler.get());)

    assert(pmt::dict_has_key(bindings, type_pmt));

    DEBUG(printf("Registering new handler for evt type %s\n", type.c_str());)
    pmt_t handler_list = pmt::dict_ref(bindings, type_pmt, PMT_NIL);
    handler_list = pmt::list_add(handler_list, handler_pmt);
    bindings = pmt::dict_add(bindings, type_pmt, handler_list);

}



int es_queue::fetch_next_event(unsigned long long min, unsigned long long max, es_eh_pair **eh){
  fstart:
    *eh = NULL;
    queue_lock.lock();
    if(event_queue.size() == 0){
        queue_lock.unlock();
        return false;
    }
    //if(event_time(eh_pair_event(event_queue[0])) < min){
    if(event_queue[0]->time() < min){
        switch(d_early_behavior){
            // discard event
            case DISCARD:
                printf("**WARNING** discarding bad event\n");
                printf("function call mandates min=%llu & max=%llu\n", min, max);
                printf("however event[0] start = %llu, end = %llu\n", event_queue[0]->time(), event_queue[0]->time() + event_queue[0]->length());
                event_queue.erase(event_queue.begin());
                queue_lock.unlock();
                goto fstart;
            // throw an assertion
            case BALK:
                printf("function call mandates min=%llu & max=%llu\n", min, max);
                printf("however event[0] start = %llu, end = %llu\n", event_queue[0]->time(), event_queue[0]->time() + event_queue[0]->length());
                print_queue();
                queue_lock.unlock();
                throw EarlyEventException("event arrived scheduled before allowed buffer!");
                break;
            // schedule as soon as possible
            case ASAP:
                // update event time to be as soon as possible
                event_queue[0]->event = event_args_add(event_queue[0]->event, pmt::intern("es::event_time") , pmt::from_uint64(min));      
                //event_print( event_queue[0] );
        }
    }
    for(int i=0; i<event_queue.size(); i++){
        es_eh_pair *eh_test = event_queue[i];
        if(eh_test->time() + eh_test->length() < max){
            event_queue.erase(event_queue.begin()+i);
            queue_lock.unlock();
            *eh = eh_test;
            return true;
        } else {
//            std::cout << "WARNING: skipping event that ends too late! evt:("<<eh_test->time()<<","<<eh_test->time() + eh_test->length() <<") buf:("<<min<<","<<max<<")\n";
            // sinks should pick this up the next time through ... 
        }
    }
    queue_lock.unlock();
    return false;
}

int es_queue::fetch_next_event2(unsigned long long min, unsigned long long max, es_eh_pair **eh){
  fstart2:
    *eh = NULL;
    if(event_queue.size() == 0){
        return false;
    }
    queue_lock.lock();
    if(event_queue[0]->time() < min){
        printf("early behavior = %d\n", d_early_behavior);
        switch(d_early_behavior){
            case DISCARD:
                printf("**WARNING** discarding bad event\n");
                printf("function call mandates min=%llu & max=%llu\n", min, max);
                printf("however event[0] start = %llu, end = %llu\n", event_queue[0]->time(), event_queue[0]->time() + event_queue[0]->length());
                //print();
                event_queue.erase(event_queue.begin());
                queue_lock.unlock();
                goto fstart2;
            case BALK:
                printf("function call mandates min=%llu & max=%llu\n", min, max);
                printf("however event[0] start = %llu, end = %llu\n", event_queue[0]->time(), event_queue[0]->time() + event_queue[0]->length());
                print_queue();
                queue_lock.unlock();
                throw EarlyEventException("event arrived scheduled before allowed buffer!");
                break;
            case ASAP:
                event_queue[0]->event = event_args_add(event_queue[0]->event, pmt::intern("es::event_time") , pmt::from_uint64(min));      
//                event_print( event_queue[0]->event );
                break;
            default:
                throw std::runtime_error("invalid early event behavior mode!");
            }
        //printf("function call mandates min=%llu & max=%llu\n", min, max);
        ////printf("however event[0] start = %llu, end = %llu\n", event_time(eh_pair_event(event_queue[0])), event_time(eh_pair_event(event_queue[0])) + event_length(eh_pair_event(event_queue[0])));
        //printf("however event[0] start = %llu, end = %llu\n", event_queue[0]->time(), event_queue[0]->time() + event_queue[0]->length());
        //printf("event arrived scheduled before allowed buffer!\n");
        //print();
    }
    for(int i=0; i<event_queue.size(); i++){
        es_eh_pair* eh_test = event_queue[i];
        if(eh_test->time() < max){
            event_queue.erase(event_queue.begin()+i);
            queue_lock.unlock();
            *eh = eh_test;
            DEBUG(printf("es_queue::fetch_next_event2() returning true!! es_eh_pair = %x\n", *eh);)
            DEBUG(printf("es_queue::fetch_next_event2() pair.handler = %x\n", &(*((*eh)->handler)) );)
            return true;
        }   
    }
       
    queue_lock.unlock();
    return false;
  
}







