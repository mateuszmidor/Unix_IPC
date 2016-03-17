#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>

using namespace std;


const char SHARED_MEMORY_FILENAME[] ="main.cpp"; // must be an existent file!!!
const int PROJ_ID = 'b';
const unsigned SHARED_MEMORY_SIZE = 1024;


void writeSharedMemory()
{
	// 1. get a unique key and the already created shared memory kernel object 
	key_t key = ftok(SHARED_MEMORY_FILENAME, PROJ_ID);
	if (key == -1)
		throw "ftok failed";

	int shared_memory_id = shmget(key, SHARED_MEMORY_SIZE, 0666);
	if (shared_memory_id == -1)
		throw "shmget failed";

	// 2. attach to shared memory
	char* data = (char*)shmat(shared_memory_id, 0, 0); // last 0 means read/write access
	if (data == (char*)(-1))
		throw "shmat failed";

	// 3. write some data to shared memory
	const char S[] = "This message goes through shared memory!!!";
	strcpy(data, S);	

	// 4. detach from shared memory
	if (shmdt(data) == -1)
		throw "shmdt failed";
}

int main()
{
	try {
		cout << "[CHILD] writer spawned, writing to shared memory..." << endl;
		writeSharedMemory();
	}
	catch (const char* s) {
		cout << "[CHILD] Exception: " << s << endl;
	}
		
}
