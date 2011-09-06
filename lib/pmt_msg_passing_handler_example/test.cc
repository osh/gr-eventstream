#include <gruel/pmt.h>
#include <gruel/msg_passing.h>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <stdio.h>

using namespace pmt;
using namespace gruel;

class test {
  public:
    test(int _A, int _B){ A=_A; B=_B;}
    int A;
    int B;
};

typedef boost::shared_ptr<test> test_sptr;




class h1 : public msg_accepter {
  public:
    void post(pmt_t msg){
        printf("got msg\n");
        std::string output = pmt_write_string( msg );
        printf("msg = {%s}\n", output.c_str() );
        }
};


class hdl {
  public:
    hdl(){ printf("hdl running.\n"); }
    void run(){ printf("run\n"); }
    //void (){ printf("aaa\n"); }
};


void exec_eh_pair(pmt_t eh_pair){
    assert(pmt_is_tuple(eh_pair));
    pmt_t e = pmt_tuple_ref(eh_pair, 0);
    pmt_t h = pmt_tuple_ref(eh_pair, 1);
    assert(pmt_is_msg_accepter(h));
    assert(pmt_is_tuple(e));
    pmt_msg_accepter_ref(h)->post(e);
}

int main(){
    
    msg_accepter_sptr hsptr1(new h1);
    msg_accepter_sptr hsptr2(new h1);
    
    pmt_t h1 = pmt_make_msg_accepter( hsptr1 );
    pmt_t h2 = pmt_make_msg_accepter( hsptr2 );
 
    pmt_t evt1 = pmt_make_tuple( pmt_intern("A"), pmt_intern("B") );
    pmt_t evt2 = pmt_make_tuple( pmt_intern("C"), pmt_intern("D") );
    
    pmt_t eh_pair_1 = pmt_make_tuple( evt1, h1 );
    pmt_t eh_pair_2 = pmt_make_tuple( evt2, h2 );
 

    pmt_t list = pmt_make_tuple( eh_pair_1, eh_pair_2 );
    int len = pmt_length(list);
    for(int i=0; i<len; i++){
        exec_eh_pair( pmt_tuple_ref( list, i ) );
    }   
   
    //exec_eh_pair(eh_pair_1);   
    //exec_eh_pair(eh_pair_2);   
    


    hdl hdl1;
    boost::thread t1(boost::bind( &hdl::run, &hdl1) );
    t1.join();

}


