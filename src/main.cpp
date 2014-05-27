#define PORT 7777
#define BUFLEN 1024

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <syslog.h>
#include <signal.h>

#include <vector>
#include <cstring>
#include <fstream>
#include <iostream>

#include <libpq-fe.h>
#include <math.h>
#include <sstream>
#include <cstdlib>

#include "aisserver.h"

using std::stringstream;
using namespace std;

string str();

int main(int argc, char *argv[])
{
    aisserver d;
    d.exec();
    return 0;
}

string str()
{
	string out_s;
    ifstream f("/usr/local/www/apache22/data/message/message.txt");

    while(!f.eof()) 
	{
		getline(f, out_s);		
    }	
	return out_s;
}
