#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>

using namespace std;


const unsigned MAPPED_FILE_REGION_SIZE = 1024;
const char MAPPED_FILE_NAME[] = "sharedfile.txt";


void writeMemoryMappedFile()
{
	// 1. create the file in read-write mode (mmap didnt work with write only mode)
	int fd = open(MAPPED_FILE_NAME, O_CREAT | O_RDWR, 0600);
	if (fd == -1)
		throw "open failed";

	// 2. resize the file to desired size
	ftruncate(fd, MAPPED_FILE_REGION_SIZE);

	// 3. map the file to the memory
	char* data = (char*)mmap(0, MAPPED_FILE_REGION_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0); // last 0 is offset
	if (data == (char*)(-1))
		throw "mmap failed";

	// 4. write some data to the mapped memory region
	const char S[] = "This message goes through memory mapped file!!!";
	strcpy(data, S);	

	// 5. unmap the file
	if (munmap(data, MAPPED_FILE_REGION_SIZE) == -1)
		throw "unmap failed";

	// 6. close the fd
	close(fd);
}

int main()
{
	try {
		cout << "[CHILD] writer spawned, writing to memory mapped file..." << endl;
		writeMemoryMappedFile();
	}
	catch (const char* s) {
		cout << "[CHILD] Exception: " << s << endl;
	}
		
}
