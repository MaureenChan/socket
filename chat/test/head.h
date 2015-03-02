#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <unistd.h>
#include <signal.h>
#include <string>
#include <list>
#include <algorithm>


using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::list;
void error_handling(const string msg); 
void read_childproc(int sig);

#pragma pack(1)
struct usr {
    int uid;
    string username;
};

struct req {
    int code;
    int sender;
    int recver;
    struct usr user;
    string msg;
};
#pragma pack(0)

void error_handling(const string msg) {
    cout << msg << '\n' << endl;
    exit(1);
}

void read_childproc(int sig) {
    pid_t pid;
    int status;
    pid = waitpid(-1, &status, WNOHANG);
    cout << "removed proc id :" << pid << endl;
}
