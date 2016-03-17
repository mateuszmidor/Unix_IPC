#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;

// handy function, as only l_type differs in locking/unlocking for file read/write
flock getFlock(int l_type)
{
	flock fl;
	fl.l_type = l_type; 	// F_RDLCK, F_WRLCK, F_UNLCK
	fl.l_whence = SEEK_SET; // SEEK_SET, SEEK_CUR, SEEK_END; can lock part of file
	fl.l_start = 0;		// lock offset (from whence)
	fl.l_len = 0;		// lock length; 0 = EOF
	fl.l_pid = getpid();	// pid of process that manages the lock
	return fl;
}

void fileLockControl(string filename, int l_type, int open_mode)
{
	flock fl = getFlock(l_type);	
	int fd = open(filename.c_str(), open_mode); // open file for reading
	fcntl(fd, F_SETLKW, &fl); // set lock using blocking method
	// F_SETLKW - blocking method
	// F_SETLK - nonblocking; ret -1 if already locked
	// F_GETLK - check if file locked, return in fl.l_type as F_UNLCK

}

void lockReading(string filename)
{
	fileLockControl(filename, F_RDLCK, O_RDONLY);
}

void lockWriting(string filename)
{
	fileLockControl(filename, F_WRLCK, O_WRONLY);
}

void unlockReading(string filename)
{
	fileLockControl(filename, F_UNLCK, O_RDONLY);
}

void unlockWriting(string filename)
{
	fileLockControl(filename, F_UNLCK, O_WRONLY);
}


// file locking example
void filelockExample()
{
	const string FILENAME = "main.cpp"; // lock our sourcecode	
	cout << "[PARENT] Locking the file " << FILENAME << " for writing..." << endl;
	lockWriting(FILENAME);
	cout << "[PARENT] Lock acquired" << endl;

	if (fork() == 0)
	{
		// [CHILD]
		cout << "[CHILD] Trying to lock for reading..." << endl;
		lockReading(FILENAME); // blocking operation
		cout << "[CHILD] Reading lock acquired..." << endl;
		unlockReading(FILENAME);
		cout << "[CHILD] unlocked" << endl;
		return;
	}
	else
	{
		// [PARENT]
		// keep the lock for a number of seconds..
		for (int i = 3; i >= 0; --i)
		{
			sleep(1);
			cout << i << "...\n";
		}
		unlockWriting(FILENAME);
		cout << "[PARENT] unlocked" << endl;
		wait(0);
	}
}

int main()
{
	filelockExample();
}
