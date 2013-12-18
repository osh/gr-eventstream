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

#include <es/es.h>
#include <es/es_handler_file.h>
#include <stdio.h>

es_handler_sptr es_make_handler_file(es_handler_file::DATATYPE type, std::string path, std::string desc){
    return es_handler_sptr(new es_handler_file(type, path, desc));
}

es_handler_file::es_handler_file( DATATYPE type, std::string path, std::string desc){
    d_type = type;
    d_path = path;
    d_desc = desc;
}

//void es_handler_file::handler( pmt_t msg, void* buf ){
void es_handler_file::handler( pmt_t msg, gr_vector_void_star buf ){
    event_print(msg);
    switch(d_type){
        case TYPE_F32:
            {
            std::string filename =  (boost::format("%s/%s_%lu.f32.dat")%d_path%d_desc%event_time(msg)).str();
            FILE* f = fopen( filename.c_str(),  "w");
            fwrite(buf[0], sizeof(float), event_length(msg), f);
            fclose(f);
            std::cout << "Wrote file: " << filename << "\n";
            break;
            }
        case TYPE_C32:
            {
            std::string filename =  (boost::format("%s/%s_%lu.c32.dat")%d_path%d_desc%event_time(msg)).str();
            FILE* f = fopen( filename.c_str(),  "w");
            fwrite(buf[0], sizeof(gr_complex), event_length(msg), f);
            fclose(f);
            std::cout << "Wrote file: " << filename << "\n";
            break;
            }
        default:
            printf("unknown vector content type.\n");
        }

}




