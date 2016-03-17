#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>

using namespace std;


const char SOCKET_FILENAME[] = "/tmp/echo_socket"; 
const char SERVER_TERMINATE_CMD[] = "TERMINATE";


void runServer()
{
	// 1. get a socket filedescriptor
	int server_socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (server_socket_fd == -1)
		throw "socket(AF_UNIX, SOCK_STREAM, 0) failed";	
	
	// 2. bind socket to address in UNIX domain
	sockaddr_un local;
	local.sun_family = AF_UNIX;
	strcpy(local.sun_path, SOCKET_FILENAME);
	unlink(local.sun_path); // delete socket file if such already exists
	unsigned local_length = strlen(local.sun_path) + sizeof(local.sun_family);
	if (bind(server_socket_fd, (sockaddr*)&local, local_length) == -1)
		throw "bind failed";
	
	// 3. start listening
	cout << "[SERVER] listening for incoming connection..." << endl;
	listen(server_socket_fd, 1); // 1 is max size of waiting connections queue, more connection will be rejected
	sockaddr_un remote;
	unsigned remote_length = sizeof(remote);
	int client_socket_fg = accept(server_socket_fd, (sockaddr*)&remote, &remote_length); // remote is filled with remote conf
	if (client_socket_fg == -1)
		throw "accept failed";
	
	// 4. got client connected. Start echoing
	cout << "[SERVER] client connected!" << endl;
	char buff[256];
	
	int num_bytes_received;
	while ((num_bytes_received = recv(client_socket_fg, buff, sizeof(buff), 0)) > 0) {

		if (!strcmp(buff, SERVER_TERMINATE_CMD))
			break;
		cout << "[SERVER] echoing: " << buff << endl;
		send(client_socket_fg, buff, num_bytes_received, 0); 
	}

	// 5. close the connection
	close(client_socket_fg);
	close(server_socket_fd);
}

int main()
{
	try {
		runServer();
	}
	catch (const char* s) {
		cout << "[SERVER] Exception: " << s << endl;
	}
		
}
