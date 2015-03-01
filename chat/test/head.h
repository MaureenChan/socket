#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <list>
#include <algorithm>


using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::list;
void error_handling(const string msg); 

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
