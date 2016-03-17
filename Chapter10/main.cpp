/* THIS PROGRAM STARTS THE ENTIRE DEMO */

#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <cstdio>
#include <iostream>

using namespace std;


const unsigned MAPPED_FILE_REGION_SIZE = 1024;
const char MAPPED_FILE_NAME[] = "sharedfile.txt";

// create writing process
void spawnWriter()
{
	if (fork() == 0)
		execlp("./writer", "writer", NULL); 
}

void memoryMappedFileExample()
{
	// 1. spawn the writer and wait till it creates and writes to memory mapped file
	cout << "[PARENT] spawning the writer..." << endl;
	spawnWriter();
	wait(0);

	// 2. open the already created file to be mapped into memory
	int fd = open(MAPPED_FILE_NAME, O_RDONLY);
	if (fd == -1)
		throw "open failed";

	// 3. map the file to the memory
	char* data = (char*)mmap(0, MAPPED_FILE_REGION_SIZE, PROT_READ, MAP_SHARED, fd, 0); // last 0 is offset
	if (data == (char*)(-1))
		throw "mmap failed";

	// 4. read the memory mapped file region
	cout << "[PARENT] reading memory mapped file region: " << data << endl;

	// 5. unmap the file
	if (munmap(data, MAPPED_FILE_REGION_SIZE) == -1)
		throw "unmap failed";

	// 6. close the file
	close(fd);

	// 7. remove the file
	unlink(MAPPED_FILE_NAME);
}

int main()
{
	try {
		memoryMappedFileExample();
	} 
	catch (const char *s) {
		cout << "[PARENT] Exception: " << s << endl;
	}
}
