#ifndef ES_EH_PAIR_HH
#define ES_EH_PAIR_HH

#include <gruel/pmt.h>
#include <gr_block.h>
using namespace pmt;

class es_eh_pair {

    public:
        es_eh_pair(pmt_t event, pmt_t handler);
        pmt_t event;
        pmt_t handler;

        void run();
        unsigned long long time();
        unsigned long long length();
        ~es_eh_pair();

    private:
        es_eh_pair() {};
};      

#endif
