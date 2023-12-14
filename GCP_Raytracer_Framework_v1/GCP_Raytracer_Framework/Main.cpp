#include "Program.h"

int main(int argc, char* argv[])
{
	// Init program
	Program program;

	// Get time at start
	std::chrono::steady_clock::time_point time1 = std::chrono::high_resolution_clock::now();

	// Parameter values: SampleSize, NumOfRays, GlobalIllItr, ShadowItr
	program.init(20, 2, 20, 20, 20); 
	
	// Get time after program has finished and calculate time taken
	std::chrono::steady_clock::time_point time2 = std::chrono::high_resolution_clock::now();

	// Write data collected to file
	program.writeToFile(std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count());
	
	return 0;
}
