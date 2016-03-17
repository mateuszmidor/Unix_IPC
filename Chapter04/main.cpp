#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <sys/wait.h>
#include <string>

using namespace std;

// read string from input pipe
string readPipe(int pipe_fd)
{
	char buff[500] = {'\0'};
	read(pipe_fd, buff, sizeof(buff));
	return string (buff);
}

// write string to output pipe
void writePipe(int pipe_fd, string s)
{
	write(pipe_fd, s.c_str(), s.size() + 1); // +1 to include '\0'
}


// read child output example
void readChildOutputExample()
{
	// 1. create the pipe
	int pipe_fd[2];
	if (pipe(pipe_fd) == -1)
		throw "Pipe could not be created";

	// 2. child - write to pipe
	//    parent - read from pipe
	if (fork())
	{
		// [PARENT]
		close(pipe_fd[1]); // close writing end of the pipe. IMPORTANT!
		cout << "Child->Parent pipe communication. The child says: ";
		cout << readPipe(pipe_fd[0]) << endl; // read the reading end of the pipe
		wait(0); // wait for the child to finish
	}
	else
	{
		// [CHILD]
		close(pipe_fd[0]); // close reading end of the pipe. IMPORTANT!
		writePipe(pipe_fd[1], "Hello!"); // write to the writing end of the pipe 
		exit(0); // exit child process
	}

	cout << endl;
}


// read the command's output example
void readCommandOutputExample()
{
	// 1. create the pipe
	int pipe_fd[2];
	if (pipe(pipe_fd) == -1)
		throw "Pipe could not be created";

	// 2. child - redirect stdout to pipe and run 'cal' app
	//    parent - read the pipe 
	if (fork())
	{
		// [PARENT]
		close(pipe_fd[1]); // close writing end of the pipe. IMPORTANT!
		cout << "Calendar written by 'cal' to the pipe: \n";
		cout << readPipe(pipe_fd[0]) << endl;
		wait(0);	
	}
	else
	{
		// [CHILD]
		// close filedescriptor 1 for our process (std out)
		close(1); 

		// duplicate our pipe's end to the first available filedescriptor
		// this means fd 1, as we have just released it
		dup(pipe_fd[1]); 

		// ... or do the same in just one line:
		// dup2(pipe_fd[1], 1); // free fd 1, then duplicate pipe to fd 1

		// run the calendar command.
		// (command, image filename as first argument, actual arglist, null terminator)
		execlp("cal", "cal", NULL); 

		// no need to exit, as child process was replaced with 'cal' process
		// that will exit on its own
	}
	
}


// piped commands example.
// call "cmd" with "args"
// read input from "input_pipe"
// return file descriptor for reading the output 
int pipeCmd(const char cmd[], const char args[], int input_pipe)
{
	// create communication pipe
	int pipe_fd[2];
	if (pipe(pipe_fd) == -1)
		throw "Pipe could not be created";
		
	// create child process
	if (fork() == 0)
	{
		// [CHILD]
		dup2(input_pipe, 0); // process stdin  = input_pipe
		dup2(pipe_fd[1], 1); // process stdout = pipe_fd[out]	
		close(pipe_fd[0]);   // close the reading end of the pipe
		execlp(cmd, cmd, args, NULL); // exec cmd with redirected input and output
	}
	// [PARENT]
	close(pipe_fd[1]); // close the writing end of the pipe
	return pipe_fd[0]; // return the reading end of the pipe
}

void pipedExample()
{
	cout << "Piped example: ls / | wc -l | sed 's|.*|[&]|'" << endl;
	int reading_pipe_fd;

	// "ls /", input fd = -1: no input
	reading_pipe_fd = pipeCmd("ls", "/", -1);

	// "wc -l", input fd = output of previous cmd; piping happens HERE
	reading_pipe_fd = pipeCmd("wc", "-l", reading_pipe_fd);

	// "sed s|.*|[&]|", input fd = output of prev cmd, piping happens HERE
	reading_pipe_fd = pipeCmd("sed", "s|.*|[&]|", reading_pipe_fd);

	// print out the final output
	cout << "Result: " << readPipe(reading_pipe_fd) << endl;
}

int main()
{
	readChildOutputExample();
	readCommandOutputExample();
	pipedExample();
}
