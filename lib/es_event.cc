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

#include <es/es_event.h>
#include <limits.h>
#include <stdio.h>

es_event_sptr es_make_event(pmt_t arb){
    return es_event_sptr(new es_event(arb));
}

es_event::es_event(pmt_t arb){
    args = pmt::make_dict();
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

    pmt_t keys = pmt::dict_keys(args);
    for(int i=0; i<pmt::length(keys); i++){
        //printf("   * i = %d\n", i);
        pmt_t key = pmt::nth(i, keys);
        pmt_t val = pmt::dict_ref(args, key, key);
        printf("   * %s = %s\n", pmt::symbol_to_string(key).c_str(), pmt::write_string(val).c_str() );
    }

}





