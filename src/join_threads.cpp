#include "join_threads.h"

join_threads::join_threads(std::vector<std::thread>& threads_):
    threads(threads_)
{}

join_threads::~join_threads()
{
    for(unsigned long i=0; i<threads.size(); ++i)
    {
        if(threads[i].joinable())
            threads[i].join();
    }
}
