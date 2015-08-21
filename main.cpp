#include "TestThreadPool.h"
#include "thread_pool.h"

#include <iostream>
#include <exception>
#include <list>
#include <algorithm>
#include <future>
#include <functional>
#include <chrono>


struct B
{
    B() = default;
    B(int i) : value(i)
    {
        throw std::exception();
    }
    int value;
};

struct C
{
    C()
    {
        try
        {
            _b = 1;
        }
        catch(...)
        {
            std::cout << "CACA" << std::endl;
        }
    }

C(int v) try :
        _b {v}
    {}
    catch(...)
    {
        std::cout << "CACA" << std::endl;
    }

    B _b;
};

/*
template<typename T>
struct sorter
{
    thread_pool pool;
    std::list<T> do_sort(std::list<T>& chunk_data)
    {
        if(chunk_data.empty())
        {
            return chunk_data;
        }
        std::list<T> result;
        result.splice(result.begin(),chunk_data,chunk_data.begin());
        T const& partition_val=*result.begin();
        typename std::list<T>::iterator divide_point=
            std::partition(chunk_data.begin(),chunk_data.end(),
                           [&](T const& val)
        {
            return val<partition_val;
        });
        std::list<T> new_lower_chunk;
        new_lower_chunk.splice(new_lower_chunk.end(),
                               chunk_data,chunk_data.begin(),
                               divide_point);

        auto f = std::bind(&sorter::do_sort,this,
                           std::move(new_lower_chunk));

        std::future<std::list<T> > new_lower=
            pool.submit(f);
        std::list<T> new_higher(do_sort(chunk_data));
        result.splice(result.end(),new_higher);
        while(!(new_lower.wait_for(std::chrono::seconds(0)) ==
                std::future_status::timeout))
        {
            pool.run_pending_task();
        }

        result.splice(result.begin(),new_lower.get());
        return result;
    }
};

template<typename T>
std::list<T> parallel_quick_sort(std::list<T> input)
{
    if(input.empty())
    {
        return input;
    }
    sorter<T> s;
    return s.do_sort(input);
}
*/
/*
template<typename Iterator,typename T>
struct accumulate_block
{
    void operator()(Iterator first,Iterator last,T& result)
    {
        result=std::accumulate(first,last,result);
    }
};

template<typename Iterator,typename T>
T parallel_accumulate(Iterator first,Iterator last,T init)
{
    unsigned long const length=std::distance(first,last);
    if(!length)
        return init;
    unsigned long const block_size=25;
    unsigned long const num_blocks=(length+block_size-1)/block_size;

    std::vector<std::future<T> > futures(num_blocks-1);
    thread_pool pool;
    Iterator block_start=first;
    for(unsigned long i=0; i<(num_blocks-1); ++i)
    {
        Iterator block_end=block_start;
        std::advance(block_end,block_size);
        futures[i]=pool.submit(accumulate_block<Iterator,T>());
        block_start=block_end;
    }
    T last_result=accumulate_block<Iterator,T>()(block_start,last);
    T result=init;
    for(unsigned long i=0; i<(num_blocks-1); ++i)
    {
        result+=futures[i].get();
    }
    result += last_result;
    return result;

}
*/














int main()
{
    //C c(2);

    std::default_random_engine e;
    std::uniform_int_distribution<> d(0, 1000);
    std::function<int()> rnd = std::bind(d, e); // a copy of e is stored in rnd
/*
    std::list<int> l;
    for(int n=0; n<1000; ++n)
        l.push_back(rnd());

    {
        std::cout << '[';
        char comma[3] = {'\0', ' ', '\0'};
        for (const auto& e : l)
        {
            std::cout << comma << e;
            comma[0] = ',';
        }
        std::cout  << ']' << std::endl;
    }*/

    //parallel_quick_sort(l);
    //parallel_accumulate(l.begin(), l.end(), 0);

    thread_pool p;
    p.submit([](){
        std::this_thread::sleep_for(std::chrono::seconds(2));
         std::cout << "caca";
     });

    p.submit([](){
        std::this_thread::sleep_for(std::chrono::seconds(2));
         std::cout << "pipi";
     });

    p.submit([](){
        std::this_thread::sleep_for(std::chrono::seconds(2));
         std::cout << "popo";
     });


    TestThreadPool pool(4);
    std::vector< std::future<int> > results;

    for(int i = 0; i < 8; ++i) {
        results.emplace_back(
            pool.enqueue([i] {
                std::cout << "hello " << i << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
                std::cout << "world " << i << std::endl;
                return i*i;
            })
        );
    }

    for(auto && result: results)
        std::cout << result.get() << ' ';
    std::cout << std::endl;

/*
    {
        std::cout << '[';
        char comma[3] = {'\0', ' ', '\0'};
        for (const auto& e : l)
        {
            std::cout << comma << e;
            comma[0] = ',';
        }
        std::cout  << ']' << std::endl;
    }
*/
    return 0;
}
