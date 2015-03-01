#include "head.h"

int main(int argc, char *argv[]) {
    int sock;
    pid_t pid;
    char buf[1024];
    if (argc != 2) {
        error_handling("Usage error : <name>");
        exit(1);
    }
    struct sockaddr_in serv_addr;
    struct usr user;

    // Initialize
    memset(&user, 0, sizeof(user));
    user.username = argv[1];

    cout << user.username << endl;
    
    // create a socket
    sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(9190);

    // connect
    if (connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1) 
        error_handling("connect() error");

    // interchange data
    struct req online;
    memset(&online, 0, sizeof(online));
    //cout << "online code:\t" << online.code << endl; 
    //cout << "online sender:\t" << online.sender << endl; 
    //cout << "online recver:\t" << online.recver << endl; 
    //cout << "online msg:\t" << online.msg << endl; 
    online.user = user;
    cout << "online uid:" << online.user.uid << endl; 
    cout << "online username:" << online.user.username << endl; 
    online.code = 1;
    if (send(sock, &online, sizeof(online), 0) < 0)
        error_handling("send failed");

    if (recv(sock, &user, sizeof(user), 0) > 0) {
        cout << "user id:\t" << user.uid << endl;
        cout << "user name:\t" << user.username << endl;
    }

    online.code = 2;
    if (send(sock, &online, sizeof(online), 0) < 0)
        error_handling("send error");

    struct req request;
    pid = fork();
    while (1) {
        if (pid == 0) {
            // parent proc
            cout << "I'm going to input" << endl;
            struct req command;
            memset(&command, 0, sizeof(command));
            while (cin >> command.code) {
                if (command.code == 3) {
                    cout << "I'm in 3 now" << endl;
                    command.user.uid = user.uid;
                    command.user.username = user.username;
                    if (send(sock, &command, sizeof(command), 0) < 0) 
                        error_handling("send error");
                     
                    cout << "Success" << endl;
                    cout << "end Child process" << endl;
                    return 0;
                } else if (command.code == 4) {
                    cout << "I'm in 4 now" << endl;
                }

            }
        } else {
            // child proc
            while (recv(sock, &request, sizeof(request), 0) > 0) {
                if (request.code == 2) {
                    cout << "user id:\t" << request.user.uid << endl;
                    cout << "user name:\t" << request.user.username << endl;
                } else if (request.code == 3) {
                    cout << "I'm going to quit!" << endl;
                    shutdown(sock, SHUT_WR);
                    close(sock);
                    cout << "end parent process" << endl;
                    return 0;
                }

            }
        }
    }
}

