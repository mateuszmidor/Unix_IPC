/* THIS PROGRAM STARTS THE ENTIRE DEMO */

#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <string>

using namespace std;


const char SOCKET_FILENAME[] = "/tmp/echo_socket"; 
const char SERVER_TERMINATE_CMD[] = "TERMINATE";


// create echo server process
void spawnEchoServer()
{
	if (fork() == 0)
		execlp("./echoserver", "echoserver", NULL); 
}

void runClient()
{
	// 1. get a unix socket filedescriptor
	int socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (socket_fd == -1)
		throw "socket(AF_UNIX, SOCK_STREAM, 0) failed";	


	// 2. connect to the server
	cout << "[CLIENT] connecting to server..." << endl;
	sockaddr_un remote;
	remote.sun_family = AF_UNIX;
	strcpy(remote.sun_path, SOCKET_FILENAME);
	unsigned lenght = strlen(remote.sun_path) + sizeof(remote.sun_family);
	if (connect(socket_fd, (sockaddr*)&remote, lenght) == -1)
		throw "connect failed";

	
	// 3. send-receive some data
	cout << "[CLIENT] connected! Start sending..." << endl;
	const string messages[] = {"This", "demo", "presents",  "UNIX domain sockets.", SERVER_TERMINATE_CMD};
	char buff[256];
	for (string s : messages) {
		// make the interaction visible to user
		sleep(1);

		// send data to echoserver
		if (send(socket_fd, s.c_str(), s.size() + 1, 0) == -1) // +1 for '\0'
			throw "send failed";

		// receive response
		int num_bytes_received = recv(socket_fd, buff, sizeof(buff), 0);
		if (num_bytes_received == -1)
			throw "server closed connection";
		else if (num_bytes_received > 0)
			cout << "[CLIENT] received: " << buff << endl;

	}

	// 4. close the connection
	close(socket_fd);
}

int main()
{
	try {
		// spawn the server first
		spawnEchoServer();
	
		// here some IPC should be used to make sure the server is ready :)
		sleep(1);

		// start talking to the server
		runClient();
	} 
	catch (const char *s) {
		cout << "[CLIENT] Exception: " << s << endl;
	}
}
