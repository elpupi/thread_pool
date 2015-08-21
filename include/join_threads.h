#ifndef JOIN_THREADS_H
#define JOIN_THREADS_H

#include <vector>
#include <thread>

class join_threads
{
private:
    std::vector<std::thread>& threads;
public:
    explicit join_threads(std::vector<std::thread>& threads_);
    ~join_threads();
};

#endif // JOIN_THREADS_H
