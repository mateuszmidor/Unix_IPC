/* THIS PROGRAM STARTS THE ENTIRE DEMO */

#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <cstdio>
#include <iostream>

using namespace std;

// named pipe needs a name :)
const char FIFO_NAME[] = "named_pipe_example";

// spawn process that will write to the named pipe
void spawnWriter(const char speech[])
{
	if (fork() == 0)
		execlp("./writer", "writer", speech, NULL); 
}

// named pipe IPC
void namedPipeExample()
{
	// 1. create named pipe
	mknod(FIFO_NAME, S_IFIFO | 0666, 0);

	// spawn writers 
	cout << "Spawning writers...\n";
	spawnWriter("wrier 1 speaks!");
	spawnWriter("wrier 2 speaks!");

	// 2. open the pipe for reading
	int fd = open(FIFO_NAME, O_RDONLY); // | O_NDELAY would cause read to return 0 if no data to read

	// 3. read from the pipe 
	int num_bytes_read;
	char buff[256];
	do {
		num_bytes_read = read(fd, buff, sizeof(buff));
		cout << buff << endl;
	} while (num_bytes_read > 0);

	// 4. remove the named pipe
	unlink(FIFO_NAME); 
}

int main()
{
	namedPipeExample();
}
	
