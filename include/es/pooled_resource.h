#ifndef ES_POOLED_RESOURCE_H
#define ES_POOLED_RESOURCE_H

#include <boost/lockfree/stack.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <map>
#include <vector>
#include <signal.h>

/*
 * this class pools a single resource type
 * providing the items from the pool when available
 * and NULL if all the pool items have been claimed
 * it does no allocation for you
 */
template <class T>
class pooled_resource {
 public:
        pooled_resource(size_t max=8) :
            d_pool(max)
            {
            }
        void populate(boost::shared_ptr<T> sptr, bool available=true){
            // not thread safe
            boost::mutex::scoped_lock(_populate_lock);
            if(d_keeper.find(sptr.get()) != d_keeper.end())
                throw std::runtime_error("attempted to add same item to pool twice!");
            d_keeper[sptr.get()] = sptr;
            if(available)
                d_pool.push(sptr.get());
        }
        void release(T* ptr){
            bool s = d_pool.push(ptr);
            if(!s){ throw std::runtime_error("push failed!"); }
            }
        T* acquire(){
            T* rv;
            bool s = d_pool.pop(rv);
            return s?rv:NULL;
            }
    private:
        boost::lockfree::stack<T* > d_pool;
        std::map<T*, boost::shared_ptr<T> > d_keeper;
        boost::mutex _populate_lock;
};

/*
 * This class establishes a map of resource pools indexed by IDX
 * it takes a factory boost::function argument to a function that will produce one of
 * the pool items in a boost::shared_ptr for you
 * and then allows arbitrarying querying of n types of items that will be allocated for you on demand
 */
template <class T, class IDX=int>
class managed_resource_pool {
    public:
 managed_resource_pool(int max_size=8, int initial_size=0, std::vector<IDX> pre_alloc_list = std::vector<IDX>() ) :
    d_factory(boost::bind(&managed_resource_pool<T,IDX>::default_factory, this, _1)),
    d_initial(initial_size),
    d_max(max_size)
    {
        ensure_allocated(pre_alloc_list);
    }
 managed_resource_pool(boost::function<boost::shared_ptr<T> (IDX) > factory, int max_size=8, int initial_size=0, std::vector<IDX> pre_alloc_list = std::vector<IDX>() ) :
    d_factory(factory),
    d_initial(initial_size),
    d_max(max_size)
    {
        ensure_allocated(pre_alloc_list);
    }
    void ensure_allocated(IDX i){ release(i, acquire(i)); }
    void ensure_allocated(std::vector<IDX> ensure_list){
        BOOST_FOREACH( IDX i, ensure_list) { ensure_allocated(i); }
    }
    T* acquire(IDX idx){
        // make sure we have a pool for this type
        if(d_map.find(idx) == d_map.end()){
            boost::mutex::scoped_lock(_map_lock);
            d_map[idx] = boost::shared_ptr< pooled_resource <T > >(new pooled_resource<T>(d_max) );
            while(d_map[idx].get() == 0)
                {
                    d_map[idx] = boost::shared_ptr< pooled_resource <T > >(new pooled_resource<T>(d_max) );
                }
            for(int i=0; i<d_initial; i++){
                d_map[idx]->populate(d_factory(idx),true);
            }
        }
        // try to get an existing pool object
        T* o = d_map[idx]->acquire();
        // if an object was not available make it and add to pool
        if(o==NULL){
            boost::shared_ptr<T> o_sptr = d_factory(idx);
            o = o_sptr.get();
            d_map[idx]->populate(o_sptr,false);
        }
        return o;
    }
    void release(IDX i, T* o){
        d_map[i]->release(o);
    }
 protected:
    boost::function<boost::shared_ptr<T>(IDX) > d_factory;
    std::map<IDX, boost::shared_ptr<pooled_resource<T> > > d_map;
    boost::mutex _map_lock;
    int d_initial;
    int d_max;
};

/*
 * This provides an interface to using managed resource pools where you do not need to write your own
 * factory function, presuming the constructor for the pooled objects takes IDX directly as an argument
 */
template <class T, class IDX=int>
class managed_resource_pool_nofactory : public managed_resource_pool<T,IDX> {
 public:
  managed_resource_pool_nofactory(int max_size=8, int initial_size=0, std::vector<IDX> pre_alloc_list = std::vector<IDX>() ) :
    managed_resource_pool<T,IDX>( boost::bind( &managed_resource_pool_nofactory::default_factory, this, _1), max_size, initial_size, pre_alloc_list ) { }
    boost::shared_ptr<T> default_factory(IDX i){ return boost::shared_ptr<T>(new T(i)); }
};
#endif
