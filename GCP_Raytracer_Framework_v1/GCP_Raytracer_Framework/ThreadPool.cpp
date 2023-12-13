#include "ThreadPool.h"

/*
    Basis for threadPool.h / .cpp
    Reference:
    https://stackoverflow.com/questions/15752659/thread-pooling-in-c11
*/

// Init threads by adding max amount the system can support and start them in a loop
void ThreadPool::start()
{
    const uint32_t numThreads = std::thread::hardware_concurrency(); // the max number of threads the system supports
    for (uint32_t i = 0; i < numThreads; ++i) 
    {
        threads.emplace_back(std::thread(&ThreadPool::ThreadLoop, this));
    }
}

// Loop threads while idle and assign tasks from the jobs queue
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

// Add passed job to a list and lock / unlock a mutex
void ThreadPool::queueJob(const std::function<void()>& job)
{
    { // Use of scope to auto deconstruct mutex
        std::unique_lock<std::mutex> lock(queuedMutex); // Unique lock unlocks mutex on deconstruct
        jobs.push(job);
    }
    mutexCondition.notify_one();
}

// Check if threads are busy doing a task or waiting idle
bool ThreadPool::busy()
{
    bool poolbusy;
    {
        std::unique_lock<std::mutex> lock(queuedMutex);
        poolbusy = !jobs.empty();
    }
    return poolbusy;
}

// Deconstructor for threads. join all current threads and clear threads list
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