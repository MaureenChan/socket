#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <unistd.h>
#include <string>

using std::string;
using std::cout;
using std::endl;
void error_handling(const string msg); 

#pragma pack(1)
struct usr {
    int uid;
    string username;
};
#pragma pack(0)

void error_handling(const string msg) {
    cout << msg << '\n' << endl;
    exit(1);
}
