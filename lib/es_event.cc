

#include <es/es_event.h>
#include <limits.h>
#include <stdio.h>

es_event_sptr es_make_event(pmt_t arb){
    return es_event_sptr(new es_event(arb));
}

es_event::es_event(pmt_t arb){
    args = pmt_make_dict();
    d_time = ULLONG_MAX;
    d_max_length = 0;
    d_name = "EVENT_BASE";
}

void es_event::set_time(unsigned long long time){
    d_time = time;
}

void es_event::print(){
    printf("EVENT(d_time=%llu, d_name=%s)\n", d_time, d_name.c_str());
    printf(" * ARGS :: \n");

//    std::string out = pmt_write_string(args);
//    printf("RAW = %s\n", out.c_str());

    pmt_t keys = pmt_dict_keys(args);
    for(int i=0; i<pmt_length(keys); i++){
        //printf("   * i = %d\n", i);
        pmt_t key = pmt_nth(i, keys);
        pmt_t val = pmt_dict_ref(args, key, key);

        printf("   * %s = %s\n", pmt_symbol_to_string(key).c_str(), pmt_write_string(val).c_str() );

    }

}





