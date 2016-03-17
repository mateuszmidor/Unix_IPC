#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;


const char MESSAGE_QUEUE_FILENAME[] ="main.cpp"; // must be an existent file!!!
const int PROJ_ID = 'b';
enum MSG_TYPE {MSG_TEXT = 1, MSG_TERMINATE}; // see: Message::mtype

struct Message
{
	long mtype; 	// message type; reader can say what type he wants to rcv
	char text[50];	// actual message payload. Can be anything
};


void sendSomeMessages()
{
	// 1. get the queue unique key
	key_t key = ftok(MESSAGE_QUEUE_FILENAME, PROJ_ID);
	if (key == -1)
		throw "ftok failed";

	// 2. get/create the queue
	int queue_id = msgget(key, 0666 | IPC_CREAT);
	if (queue_id == -1)
		throw "msgget failed";


	// 3. prepare the messages
	Message text = { MSG_TEXT, "Message got through a message queue!" };
	Message terminate = { MSG_TERMINATE, "TERMINATE" };
	Message messages[] = { text, text, text, terminate }; // mind "terminate" at the end

	// 4. send a couple of messages	
	for (const Message m: messages)
	{
		if (msgsnd(queue_id, &m, sizeof(m.text), 0) == -1)
			throw "msgsnd failed"; 
		sleep(1);
	}
}

int main()
{
	try {
		cout << "[CHILD] Writer spawned, sending messages..." << endl;
		sendSomeMessages();
	}
	catch (const char* s) {
		cout << "[CHILD] Exception: " << s << endl;
	}
		
}
