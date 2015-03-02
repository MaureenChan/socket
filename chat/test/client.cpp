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
    struct req request;
    struct req command;
    pid = fork();
    while (1) {
        if (pid == 0) {
            // parent proc
            cout << "I'm going to input" << endl;
            memset(&command, 0, sizeof(command));
            while (cin >> command.code) {
                if (command.code == 1) {
                    // sign in
                    command.user.username = user.username;
                    if (send(sock, &command, sizeof(command), 0) < 0) 
                        error_handling("send error");
                } else if (command.code == 2) {
                    // get online user
                    if (send(sock, &command, sizeof(command), 0) < 0)
                        error_handling("send error");
                } else if (command.code == 3) {
                    // offline
                    command.user.uid = user.uid;
                    command.user.username = user.username;
                    if (send(sock, &command, sizeof(command), 0) < 0) 
                        error_handling("send error");
                    return 0;
                } else if (command.code == 4) {
                    // send message
                    cout << "I'm in 4 now" << endl;
                }
            }
        } else {
            // child proc
            while (recv(sock, &request, sizeof(request), 0) > 0) {
                if (request.code == 1) {
                    user.uid = request.user.uid;
                } else if (request.code == 2) {
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

