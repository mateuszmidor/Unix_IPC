#include <sys/wait.h>
#include <sys/sem.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;


const char SEMAPHORE_ARRAY_FILENAME[] ="main.cpp"; // must be an existent file!!!
const int PROJ_ID = 'b';


void waitForSemaphore()
{
	// 1. get unique key for semapthore array
	key_t key = ftok(SEMAPHORE_ARRAY_FILENAME, PROJ_ID);
	if (key == -1)
		throw "ftok failed";

	// 2. get the already created SYSTEM V semaphore array
	const unsigned NUM_SEMAPHORES = 1;
	int semaphore_array_id = semget(key, NUM_SEMAPHORES, 0666 );
	if (semaphore_array_id == -1)
		throw "semget failed";
	
	// 3. wait for the semaphore
	sembuf sb;
	sb.sem_num = 0; // index of semaphore in 0-indexed array that we want to manipulate
	sb.sem_op = -1; // acquire 1 unit of semaphore resource 
	sb.sem_flg = 0; // IPC_NOWAIT would cause semop return EAGAIN instead of blocking
	const unsigned NUM_OPERATIONS = 1; // number of sembuf's provided to semop
	semop(semaphore_array_id, &sb, NUM_OPERATIONS); // block until semaphore has enough resources
}

int main()
{
	try {
		cout << "[CHILD] waiting for semaphore..." << endl;
		waitForSemaphore();
		cout << "[CHILD] semaphore acquired!" << endl;
	}
	catch (const char* s) {
		cout << "[CHILD] Exception: " << s << endl;
	}
}
