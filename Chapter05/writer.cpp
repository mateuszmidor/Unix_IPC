#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <iostream>
#include <string>
using namespace std;

const char FIFO_NAME[] = "named_pipe_example";

void startWriting(const string &s)
{
	// 1. PRECONDITION: the named pipe is already created by reading process

	// 2. open the pipe for writing
	int fd = open(FIFO_NAME, O_WRONLY); // | O_NDELAY would cause open return -1 if no readers are waiting

	// 3. write the string to the pipe for a few times
	for (unsigned i = 0; i < 5; ++i)
	{
		write(fd, s.c_str(), s.size() + 1); // +1 to include '\0'
		sleep(1);
	}
}

// PRECONDITION: a message to be printed is provided as argv[1]
int main(int argc, char* argv[])
{
	if (argc < 2)
		throw "ERROR: please provide text to write";

	cout << "Writer spawned...\n";
	startWriting(argv[1]);
}
