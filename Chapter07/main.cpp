/* THIS PROGRAM STARTS THE ENTIRE DEMO */

#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <cstdio>
#include <iostream>

using namespace std;


const char MESSAGE_QUEUE_FILENAME[] ="main.cpp"; // must be an existent file!!!
const int PROJ_ID = 'b'; // a magical number (?)
enum MSG_TYPE {MSG_TEXT = 1, MSG_TERMINATE}; // see Message::mtype

struct Message
{
	long mtype; 	// message type; reader can say what type he wants to rcv
	char text[50];	// actual message payload. Can be anything
};


// create writing process
void spawnWriter()
{
	if (fork() == 0)
		execlp("./writer", "writer", NULL); 
}

// read the messages from the queue
void startReading()
{
	// 1. get the queue unique key 
	key_t key = ftok(MESSAGE_QUEUE_FILENAME, PROJ_ID);
	if (key == -1)
		throw "ftok failed";

	// 2. get/create the queue if not exists yet
	int queue_id = msgget(key, 0666 | IPC_CREAT);
	if (queue_id == -1)
		throw "msgget failed";

	// 3. read the queue until MSG_TERMINATE is received 
	Message m;
	do {
		if (msgrcv(queue_id, &m, sizeof(m.text), 0, 0) == -1)
			throw "msgrcv failed";
		else
			cout << "[PARENT] MSG: " << m.text << endl; 

	} while (m.mtype != MSG_TERMINATE);

	// 4. destroy the queue 
	if (msgctl(queue_id, IPC_RMID, 0) == -1)
		throw "msgctl(IPC_RMID) failed";
}

int main()
{
	try {
		cout << "[PARENT] Spawning writer...\n";
		spawnWriter();
		startReading();
	} 
	catch (const char *s) {
		cout << "[PARENT] Exception: " << s << endl;
	}
}
