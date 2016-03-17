/* THIS PROGRAM STARTS THE ENTIRE DEMO */

#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <cstdio>
#include <iostream>

using namespace std;


const char SHARED_MEMORY_FILENAME[] ="main.cpp"; // must be an existent file!!!
const int PROJ_ID = 'b'; // a magical number (?)
const unsigned SHARED_MEMORY_SIZE  = 1024;


// create writing process
void spawnWriter()
{
	if (fork() == 0)
		execlp("./writer", "writer", NULL); 
}

void sharedMemoryExample()
{
	// 1. get unique key and create the shared memory
	key_t key = ftok(SHARED_MEMORY_FILENAME, PROJ_ID);
	if (key == -1)
		throw "ftok failed";

	int shared_memory_id = shmget(key, SHARED_MEMORY_SIZE, 0666 | IPC_CREAT);
	if (shared_memory_id == -1)
		throw "shmget failed";


	// 2. spawn the writer and wait till it finishes writing to shared mem
	cout << "[PARENT] spawning the writer..." << endl;
	spawnWriter();
	wait(0);


	// 3. attach to shared memory
	char* data = (char*)shmat(shared_memory_id, 0, SHM_RDONLY); 
	if (data == (char*)(-1))
		throw "shmat failed";
	
	// 4. print the shared memory contents
	cout << "[PARENT] reading shared memory contents: " << data << endl;

	// 5. detach from shared memory
	if (shmdt(data) == -1)
		throw "shmdt failed";


	// 6. destroy the shared memory 
	if (shmctl(shared_memory_id, IPC_RMID, 0) == -1)
		throw "shmctl(IPC_RMID) failed";
}

int main()
{
	try {
		sharedMemoryExample();
	} 
	catch (const char *s) {
		cout << "[PARENT] Exception: " << s << endl;
	}
}
