
#include <es/es_queue.h>
#include <es/es_common.h>

#include <limits.h>
#include <stdio.h>

es_queue_sptr es_make_queue(){
    return es_queue_sptr(new es_queue());
}

es_queue::es_queue(){
}

int es_queue::add_event(pmt_t evt){
    printf("es_queue::add_event...\n");

    std::map< std::string, std::vector<pmt_t> >::iterator it;
    it = bindings.find( event_type(evt) );
    if(it == bindings.end() ){
        printf("WARNING: attempted to add event to queue of type which we are unaware! Discarding\n");
        event_print(evt);
        return -1;
        }

    
    std::vector<pmt_t> handlers = bindings.at(event_type(evt));

    // AAA -- this should be unneccisary once start() time checking is in?
    if(handlers.size() == 0){
        printf("WARNING: attempted to add event to queue for which no handler is defined!\n");
        }
    
    if(event_time(evt) == ULLONG_MAX){
        fprintf(stderr, "WARNING: event recieved with unset time. (%s)\n", event_type(evt).c_str());
    }
    int idx;
    //for(idx=0; (idx < event_queue.size() && event_time(evt) > event_time(eh_pair_event(event_queue[idx])) ); idx++){ }
    for(idx=0; (idx < event_queue.size() && event_time(evt) > event_queue[idx]->time() ); idx++){ }

    queue_lock.lock();
    for(int i=0; i<handlers.size(); i++){
        es_eh_pair* eh_pair = new es_eh_pair( evt, handlers[i] );
        printf("created new eh_pair = %x\n", eh_pair);
//        pmt_t eh_pair = pmt_make_tuple( evt, handlers[i] );
        event_queue.insert(event_queue.begin()+idx, eh_pair); 
    }
    queue_lock.unlock();
       
    
}


void es_queue::print(){

    printf("EVENTSTREAM_BINDINGS...\n");

    std::map<std::string, std::vector< pmt_t> >::iterator it;
    
    //iterate over all keys in bindings
    for(it = bindings.begin(); it != bindings.end(); it++){
        printf(" * EVENT: (%s) # Handlers = %d\n", (*it).first.c_str(), (*it).second.size());
        //iterate over list items in each key value
        for(int i=0; i<(*it).second.size(); i++){
            //print the binding to screen
            printf("   * Handler--> ");
            printf("%s\n", pmt_write_string( (*it).second[i] ).c_str() );
        }
    }

    printf("EVENTSTREAM_QUEUE (size = %d)\n", event_queue.size());
    for(int i=0; i<event_queue.size(); i++){
        //event_queue[i].print();
    }

}

int es_queue::register_event_type(pmt_t type){
    register_event_type( pmt_symbol_to_string(type) );
}
    
int es_queue::register_event_type(std::string type){
    printf("es_queue::register_event_type...\n");
    std::map< std::string, std::vector<pmt_t> >::iterator it;
    it = bindings.find( type );
    if(it != bindings.end() ){
        printf("WARNING: type already registered (%s)\n", type.c_str());
        return -1;
    } 
    
    std::vector<pmt_t> emptylist;
    bindings[type] =  emptylist; 
    return 0;
}

void es_queue::bind_handler(pmt_t type, pmt_t handler){
    bind_handler( pmt_symbol_to_string(type), handler);
    }

void es_queue::bind_handler(std::string type, pmt_t handler){
    printf("EVENTSTREAM_QUEUE::BIND_HANDLER (%s).\n",type.c_str());

    boost::shared_ptr<gruel::msg_accepter> h = pmt_msg_accepter_ref(handler);

    printf("pmt adx = %x\n", &(*handler));
    printf("handler adx = %x\n",&(*(h)));

    std::map< std::string, std::vector<pmt_t> >::iterator it;
    it = bindings.find( type );
    if(it == bindings.end() ){
        printf("could not add handler, event type not found.\n");
        assert(0);
    } 

    printf("Registering new handler for evt type %s\n", type.c_str());
    bindings.at(type).push_back(handler);

}



/*
*  Fetch an event while guaranteeing that the entire event occurs within the given time window.
*/ 
/*
int es_queue::fetch_next_event(unsigned long long min, unsigned long long max, pmt_t &eh){

    if(event_queue.size() == 0){
        return false;
    }
    queue_lock.lock();
    if(event_time(eh_pair_event(event_queue[0])) < min){
        printf("function call mandates min=%llu & max=%llu\n", min, max);
        printf("however event[0] start = %llu, end = %llu\n", event_time(eh_pair_event(event_queue[0])), event_time(eh_pair_event(event_queue[0])) + event_length(eh_pair_event(event_queue[0])));
        printf("event arrived scheduled before allowed buffer!\n");
        print();
        assert(0);
    }
    for(int i=0; i<event_queue.size(); i++){
        pmt_t eh_test = event_queue[i];
        if(event_time(eh_pair_event(eh_test)) + event_length(eh_pair_event(eh_test)) < max){
            event_queue.erase(event_queue.begin()+i);
            queue_lock.unlock();
            eh = eh_test;
            return true;
        }   
    }
       
    queue_lock.unlock();
    return false;
  
}
*/

int es_queue::fetch_next_event(unsigned long long min, unsigned long long max, es_eh_pair **eh){
    *eh = NULL;
    if(event_queue.size() == 0){
        return false;
    }
    queue_lock.lock();
    //if(event_time(eh_pair_event(event_queue[0])) < min){
    if(event_queue[0]->time() < min){
        printf("function call mandates min=%llu & max=%llu\n", min, max);
        //printf("however event[0] start = %llu, end = %llu\n", event_time(eh_pair_event(event_queue[0])), event_time(eh_pair_event(event_queue[0])) + event_length(eh_pair_event(event_queue[0])));
        printf("however event[0] start = %llu, end = %llu\n", event_queue[0]->time(), event_queue[0]->time() + event_queue[0]->length());
        printf("event arrived scheduled before allowed buffer!\n");
        print();
        assert(0);
    }
    for(int i=0; i<event_queue.size(); i++){
        es_eh_pair *eh_test = event_queue[i];
//        pmt_t eh_test = event_queue[i];
        //if(event_time(eh_pair_event(eh_test)) + event_length(eh_pair_event(eh_test)) < max){
        if(eh_test->time() + eh_test->length() < max){
            event_queue.erase(event_queue.begin()+i);
            queue_lock.unlock();
            *eh = eh_test;
            return true;
        }   
    }
    queue_lock.unlock();
    return false;
}

/*
*  Fetch an event while guaranteeing only that the start occurs within the given time window.
*/ 
/*
int es_queue::fetch_next_event2(unsigned long long min, unsigned long long max, pmt_t &eh){

    if(event_queue.size() == 0){
        return false;
    }
    queue_lock.lock();
    if(event_time(eh_pair_event(event_queue[0])) < min){
        printf("function call mandates min=%llu & max=%llu\n", min, max);
        printf("however event[0] start = %llu, end = %llu\n", event_time(eh_pair_event(event_queue[0])), event_time(eh_pair_event(event_queue[0])) + event_length(eh_pair_event(event_queue[0])));
        printf("event arrived scheduled before allowed buffer!\n");
        print();
        assert(0);
    }
    for(int i=0; i<event_queue.size(); i++){
        pmt_t eh_test = event_queue[i];
        if(event_time(eh_pair_event(eh_test)) < max){
            event_queue.erase(event_queue.begin()+i);
            queue_lock.unlock();
            eh = eh_test;
            return true;
        }   
    }
       
    queue_lock.unlock();
    return false;
  
}
*/
int es_queue::fetch_next_event2(unsigned long long min, unsigned long long max, es_eh_pair **eh){
    *eh = NULL;
    if(event_queue.size() == 0){
        return false;
    }
    queue_lock.lock();
    if(event_queue[0]->time() < min){
        printf("function call mandates min=%llu & max=%llu\n", min, max);
        //printf("however event[0] start = %llu, end = %llu\n", event_time(eh_pair_event(event_queue[0])), event_time(eh_pair_event(event_queue[0])) + event_length(eh_pair_event(event_queue[0])));
        printf("however event[0] start = %llu, end = %llu\n", event_queue[0]->time(), event_queue[0]->time() + event_queue[0]->length());
        printf("event arrived scheduled before allowed buffer!\n");
        print();
        assert(0);
    }
    for(int i=0; i<event_queue.size(); i++){
        es_eh_pair* eh_test = event_queue[i];
        if(eh_test->time() < max){
            event_queue.erase(event_queue.begin()+i);
            queue_lock.unlock();
            *eh = eh_test;
            printf("returning true!! eh = %x\n", eh);
            return true;
        }   
    }
       
    queue_lock.unlock();
    return false;
  
}







