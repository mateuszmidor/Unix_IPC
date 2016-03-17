#include <unistd.h> 	// fork()
#include <sys/wait.h>	// wait(0)
#include <iostream>

using namespace std;

int main()
{
	if (pid_t pid = fork())
	{
		cout << "[Parent] Hello from Parent process! Child PID = " << pid << endl;
		wait(0); // wait for child process to finish and sweep the process table
	} else  
	{
		cout << "[Child] Hello from Child process!" << endl;
		return 0;
	}
}	
