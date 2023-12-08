#include "ThreadPool.h"

/*
    Basis for threadPool.h / .cpp
    Reference:
    https://stackoverflow.com/questions/15752659/thread-pooling-in-c11
*/

void ThreadPool::start()
{
    const uint32_t numThreads = std::thread::hardware_concurrency(); // the max number of threads the system supports
    for (uint32_t i = 0; i < numThreads; ++i) 
    {
        threads.emplace_back(std::thread(&ThreadPool::ThreadLoop, this));
    }
}

void ThreadPool::ThreadLoop()
{
    while (true) {
        std::function<void()> job;
        {
            std::unique_lock<std::mutex> lock(queuedMutex);

            mutexCondition.wait(lock, [this] 
            {
                return !jobs.empty() || shouldTerminate;
            } );

            if (shouldTerminate) 
            {
                return;
            }

            job = jobs.front();
            jobs.pop();
        }
        job();
    }
}

void ThreadPool::queueJob(const std::function<void()>& job)
{
    {
        std::unique_lock<std::mutex> lock(queuedMutex);
        jobs.push(job);
    }
    mutexCondition.notify_one();
}


bool ThreadPool::busy()
{
    bool poolbusy;
    {
        std::unique_lock<std::mutex> lock(queuedMutex);
        poolbusy = !jobs.empty();
    }
    return poolbusy;
}

void ThreadPool::stop()
{
    {
        std::unique_lock<std::mutex> lock(queuedMutex);
        shouldTerminate = true;
    }
    mutexCondition.notify_all();

    for (std::thread& active_thread : threads) 
    {
        active_thread.join();
    }
    threads.clear();
}