#include <signal.h>
#include <iostream>

using namespace std;

void interupt(int)
{
	cout << "no interrupting!\n";
}

void usr1(int)
{
	cout << "Hello!\n";
}

void usr2(int)
{
	cout << "Goodbye!\n";
}

int main()
{
	// 1. register signal handlers
	// signal (SIGINT, interupt); // this would prevent ^C from stopping the app
	signal (SIGUSR1, usr1);
	signal (SIGUSR2, usr2);
	

	// 2. wait for SIGUSR1/2 signals. They are handled async from the while loop
	cout << "Waiting for SIGUSR1 or SIGUSR2...\n";
	while (true) { } 

	// now, from the terminal:
	// >kill SIGUSR1 <pid>
	// >kill SIGUSR2 <pid>
}
