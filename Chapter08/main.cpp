/* THIS PROGRAM STARTS THE ENTIRE DEMO */

#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <cstdio>
#include <iostream>

using namespace std;


const char SEMAPHORE_ARRAY_FILENAME[] ="main.cpp"; // must be an existent file!!!
const int PROJ_ID = 'b'; // a magical number (?)

union semun //  used in calls to semctl() to initialize the semaphore
{                   
	int                 val;
	struct semid_ds *   buf;
    	unsigned short *    array;
};


// create child process
void spawnWaiter()
{
	if (fork() == 0)
		execlp("./waiter", "waiter", NULL); 
}

// setup semaphore, spawn child proc to wait for semaphore, signal semaphore
void semaphoreExample()
{
	// 1. get the unique key for semaphore array 
	cout << "[PARENT] creating and acquiring a semaphore for 3 secs..." << endl;
	key_t key = ftok(SEMAPHORE_ARRAY_FILENAME, PROJ_ID);
	if (key == -1)
		throw "ftok failed";

	// 2. create the SYSTEM V semaphore array
	const unsigned NUM_SEMAPHORES = 1;
	int semaphore_array_id = semget(key, NUM_SEMAPHORES, 0666 | IPC_CREAT);
	if (semaphore_array_id == -1)
		throw "semget failed";

	// 3. initialize the semaphore with 0 resources
	const unsigned SEM_INDEX = 0; // semaphore index in 0-indexed semaphore array 
	semun init_val;
	init_val.val = 0; // semaphore initial resource value is 0	
	if (semctl(semaphore_array_id, SEM_INDEX, SETVAL, init_val) == -1)
		throw "semctl failed";
	
	// 4. spawn child process and let it wait for the semaphore for 3 secs
	spawnWaiter();
	sleep(3);

	// 5. release the semaphore to let the child go on
	cout << "[PARENT] releasing semaphore" << endl;
	sembuf sb;
	sb.sem_num = SEM_INDEX; // the index of semaphore in 0-indexed array that we want to manipulate
	sb.sem_op = 1;	// release 1 unit of semaphore resource to let the CHILD acquire it
	sb.sem_flg = 0; // IPC_NOWAIT would cause semop return EAGAIN instead blocking
	const unsigned NUM_OPERATIONS = 1; // num os sembuf's provided to semop
	semop(semaphore_array_id, &sb, NUM_OPERATIONS); // manipulate the semaphore 

	// wait for child process to finish
	wait(0);

	// 6. destroy the semaphore array 
	if (semctl(semaphore_array_id, 0, IPC_RMID) == -1)
		throw "semctl(IPC_RMID) failed";
}

int main()
{
	try {
		semaphoreExample();
	} 
	catch (const char *s) {
		cout << "[PARENT] Exception: " << s << endl;
	}
}
