#include "Program.h"

int main(int argc, char* argv[])
{
	Program program = Program();

	if (program.init() == -1)
		return -1;

	program.runProgram();

	return 0;
}
