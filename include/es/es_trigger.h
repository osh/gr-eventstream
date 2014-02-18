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
#ifndef INCLUDED_EVENTSTREAM_TRIGGER_H
#define INCLUDED_EVENTSTREAM_TRIGGER_H

#include <gnuradio/sync_block.h>
#include <pmt/pmt.h>
#include <es/es_queue.h>
#include <es/es_handler.h>
#include <string>

class es_trigger;
using namespace pmt;

//typedef boost::shared_ptr<es_trigger> es_trigger_sptr;

//es_trigger_sptr es_make_trigger (pmt_t arb, es_queue_sptr queue, int itemsize, std::string type);

//class es_trigger : public gr::sync_block
class es_trigger : public es_handler
{
private:
//  friend es_trigger_sptr es_make_trigger (pmt_t arb, es_queue_sptr queue, int itemsize, std::string type);

 protected:
  es_trigger (std::string type, gr::io_signature::sptr in_sig, gr::io_signature::sptr out_sig);  	// private constructor


 public:
  ~es_trigger ();	// public destructor

  virtual int work (int noutput_items,
	    gr_vector_const_void_star &input_items,
	    gr_vector_void_star &output_items);
 
  
  void register_handler(std::string name){
//    std::cout << "Register_handler: " << name << "\n";
    event_types.push_back(pmt::mp(name));
    message_port_register_out(pmt::intern(name));
    }

  virtual bool start(){
//        std::cout << "es_trigger::start() << " << name() << "\n";
        for(int i=0; i< event_types.size(); i++){
            pmt::pmt_t reg = pmt::cons(event_types[i], message_subscribers(event_types[i]));
            reg = pmt::cons(pmt::mp("ES_REGISTER_HANDLER"), reg);
            message_port_pub(pmt::mp("which_stream"), reg);
//            std::cout << "sent registeration message: " << reg << "\n";
            }
        }
  
  std::vector<pmt_t> event_types;
  pmt_t event_type(int idx);
  pmt_t arb;
  es_queue_sptr event_queue;
  unsigned long long d_time;

  // event sizing parameters around the trigger moment
  unsigned int d_length;
  unsigned int d_lookback;
};

#endif /* INCLUDED_EVENTSTREAM_SQUARE2_FF_H */
