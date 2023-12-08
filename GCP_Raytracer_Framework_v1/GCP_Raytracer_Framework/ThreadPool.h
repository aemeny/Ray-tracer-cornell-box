#pragma once
#include <thread>
#include <mutex>
#include <queue>
#include <functional>
#include <vector>

struct ThreadPool
{
	void ThreadLoop();

	bool shouldTerminate = false;           //tells threas to stop looking for jobs
	std::mutex queuedMutex;				    //prevents data races to the job queue
	std::condition_variable mutexCondition; //allows threads to wait on new jobs or termination
	std::vector<std::thread> threads;
	std::queue<std::function<void()>> jobs;

public:
	void start();
	void queueJob(const std::function<void()>& job);
	void stop();
	bool busy();
};

