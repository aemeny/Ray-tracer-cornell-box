#include "Program.h"

int main(int argc, char* argv[])
{
	// Init program
	Program program;

	// Get time at start
	std::chrono::steady_clock::time_point time1 = std::chrono::high_resolution_clock::now();

	// Parameter values: SampleSize, NumOfRays, GlobalIllItr, ShadowItr
	program.init(1, 2, 1, 1, NULL); 
	
	// Get time after program has finished and calculate time taken
	std::chrono::steady_clock::time_point time2 = std::chrono::high_resolution_clock::now();

	// Write data collected to file
	program.writeToFile(std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count());



	// --- TESTS WITH DIFFERENT SET VALUES ---
	// max 12 threads
	time1 = std::chrono::high_resolution_clock::now();
	program.init(5, 2, 5, 5, NULL);
	time2 = std::chrono::high_resolution_clock::now();
	program.writeToFile(std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count());

	time1 = std::chrono::high_resolution_clock::now();
	program.init(10, 2, 10, 10, NULL);
	time2 = std::chrono::high_resolution_clock::now();
	program.writeToFile(std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count());

	time1 = std::chrono::high_resolution_clock::now();
	program.init(50, 2, 50, 50, NULL);
	time2 = std::chrono::high_resolution_clock::now();
	program.writeToFile(std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count());

	time1 = std::chrono::high_resolution_clock::now();
	program.init(50, 2, 1, 1, NULL);
	time2 = std::chrono::high_resolution_clock::now();
	program.writeToFile(std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count());

	time1 = std::chrono::high_resolution_clock::now();
	program.init(1, 2, 50, 1, NULL);
	time2 = std::chrono::high_resolution_clock::now();
	program.writeToFile(std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count());

	time1 = std::chrono::high_resolution_clock::now();
	program.init(1, 2, 1, 50, NULL);
	time2 = std::chrono::high_resolution_clock::now();
	program.writeToFile(std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count());

	//9 threads
	time1 = std::chrono::high_resolution_clock::now();
	program.init(1, 2, 1, 1, 9);
	time2 = std::chrono::high_resolution_clock::now();
	program.writeToFile(std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count());

	time1 = std::chrono::high_resolution_clock::now();
	program.init(5, 2, 5, 5, 9);
	time2 = std::chrono::high_resolution_clock::now();
	program.writeToFile(std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count());

	time1 = std::chrono::high_resolution_clock::now();
	program.init(10, 2, 10, 10, 9);
	time2 = std::chrono::high_resolution_clock::now();
	program.writeToFile(std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count());

	time1 = std::chrono::high_resolution_clock::now();
	program.init(50, 2, 50, 50, 9);
	time2 = std::chrono::high_resolution_clock::now();
	program.writeToFile(std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count());

	// 6 threads
	time1 = std::chrono::high_resolution_clock::now();
	program.init(1, 2, 1, 1, 6);
	time2 = std::chrono::high_resolution_clock::now();
	program.writeToFile(std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count());

	time1 = std::chrono::high_resolution_clock::now();
	program.init(5, 2, 5, 5, 6);
	time2 = std::chrono::high_resolution_clock::now();
	program.writeToFile(std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count());

	time1 = std::chrono::high_resolution_clock::now();
	program.init(10, 2, 10, 10, 6);
	time2 = std::chrono::high_resolution_clock::now();
	program.writeToFile(std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count());

	time1 = std::chrono::high_resolution_clock::now();
	program.init(50, 2, 50, 50, 6);
	time2 = std::chrono::high_resolution_clock::now();
	program.writeToFile(std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count());

	// 3 thread
	time1 = std::chrono::high_resolution_clock::now();
	program.init(1, 2, 1, 1, 3);
	time2 = std::chrono::high_resolution_clock::now();
	program.writeToFile(std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count());

	time1 = std::chrono::high_resolution_clock::now();
	program.init(5, 2, 5, 5, 3);
	time2 = std::chrono::high_resolution_clock::now();
	program.writeToFile(std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count());

	time1 = std::chrono::high_resolution_clock::now();
	program.init(10, 2, 10, 10, 3);
	time2 = std::chrono::high_resolution_clock::now();
	program.writeToFile(std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count());

	time1 = std::chrono::high_resolution_clock::now();
	program.init(50, 2, 50, 50, 3);
	time2 = std::chrono::high_resolution_clock::now();
	program.writeToFile(std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count());

	// 1 thread
	time1 = std::chrono::high_resolution_clock::now();
	program.init(1, 2, 1, 1, 1);
	time2 = std::chrono::high_resolution_clock::now();
	program.writeToFile(std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count());

	time1 = std::chrono::high_resolution_clock::now();
	program.init(5, 2, 5, 5, 1);
	time2 = std::chrono::high_resolution_clock::now();
	program.writeToFile(std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count());

	time1 = std::chrono::high_resolution_clock::now();
	program.init(10, 2, 10, 10, 1);
	time2 = std::chrono::high_resolution_clock::now();
	program.writeToFile(std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count());

	time1 = std::chrono::high_resolution_clock::now();
	program.init(50, 2, 50, 50, 1);
	time2 = std::chrono::high_resolution_clock::now();
	program.writeToFile(std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count());

	return 0;
}
