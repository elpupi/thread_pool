#include "thread_pool.h"
//thread_local std::unique_ptr<thread_pool::local_queue_type> thread_pool::local_work_queue = nullptr;



thread_local work_stealing_queue* thread_pool::local_work_queue = nullptr;
thread_local unsigned thread_pool::my_index = 0;

thread_pool::thread_pool():
    done(false),joiner(threads)
{
    unsigned const thread_count=std::thread::hardware_concurrency();
    try
    {
        for(unsigned i=0; i<thread_count; ++i)
        {
            queues.push_back(std::unique_ptr<work_stealing_queue>(
                                 new work_stealing_queue));
            threads.push_back(
                std::thread(&thread_pool::worker_thread,this,i));
        }
    }
    catch(...)
    {
        done=true;
        throw;
    }
}

thread_pool::~thread_pool()
{
    done=true;
}


void thread_pool::worker_thread(unsigned my_index_)
{
    my_index=my_index_;
    local_work_queue=queues[my_index].get();
    while(!done)
    {
        run_pending_task();
    }
}

bool thread_pool::pop_task_from_other_thread_queue(task_type& task)
{
    for(unsigned i=0; i<queues.size(); ++i)
    {
        unsigned const index=(my_index+i+1)%queues.size();
        if(queues[index]->try_steal(task))
        {
            return true;
        }
    }
    return false;
}

void thread_pool::run_pending_task()
{
    task_type task;
    if(pop_task_from_local_queue(task) ||
            pop_task_from_pool_queue(task) ||
            pop_task_from_other_thread_queue(task))
    {
        task();
    }
    else
    {
        std::this_thread::yield();
    }
}
