#pragma once
#include <thread>
#include <mutex>
#include <queue>
#include <functional>
#include <vector>

// Class that controls and manages threads
struct ThreadPool
{
	void ThreadLoop(); // Main loop to assign threads tasks

	bool shouldTerminate = false; // Tells threas to stop looking for jobs
	std::mutex queuedMutex; // Prevents data races to the job queue
	std::condition_variable mutexCondition; // Allows threads to wait on new jobs or termination
	std::vector<std::thread> threads; // List of threads created
	std::queue<std::function<void()>> jobs; // Queue of jobs for threads to complete

public:
	void start(); // Initialise threads
	void queueJob(const std::function<void()>& job); // Add a new job to the queue
	void stop(); // Stop all threads / Decontructor
	bool busy(); // Check if threads are working on a task
};

