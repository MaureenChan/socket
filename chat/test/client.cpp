#include "head.h"

struct usr user;
void *send_msg(void *arg) {
    int sock = *((int*) arg);
    struct req command;
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
            command.sender = user.uid;
            cout << "Test now uid: " << user.uid << endl;
            cout << "Enter receiver uid" << endl;
            cin >> command.recver;
            cout << "Enter message" << endl;
            cin.getline(command.msg, sizeof(command.msg));
            cout << "message is : " << command.msg << endl;

            if (send(sock, &command, sizeof(command), 0) < 0) 
                error_handling("send error");

        }
        memset(&command, 0, sizeof(command));
        cout << "I'm going to input" << endl;
    }

}

void *recv_msg(void *arg) {
    int sock = *((int*)arg);
    struct req request;
    while (recv(sock, &request, sizeof(request), 0) > 0) {
        if (request.code == 1) {
            user.uid = request.user.uid;
        } else if (request.code == 2) {
            cout << "user id:\t" << request.user.uid << endl;
            cout << "user name:\t" << request.user.username << endl;
        } else if (request.code == 3) {
            cout << "test request code : " << request.code << endl;
            cout << "I'm going to quit!" << endl;
            shutdown(sock, SHUT_WR);
            close(sock);
            cout << "end parent process" << endl;
            return 0;
        } else if (request.code == 4) {
            cout << "message from uid " << request.sender << " [ " << request.user.username << " ] "<< endl;
            cout << "message: " << request.msg << endl;
        } else if (request.code == 5) {
            cout << "uid: " << request.user.uid << " [ " << request.user.username << " ] is online" << endl;
        }


    }

}
int main(int argc, char *argv[]) {
    int sock;
    pthread_t snd_thread, rcv_thread;
    void *thread_return;
    if (argc != 2) {
        error_handling("Usage error : <name>");
        exit(1);
    }
    struct sockaddr_in serv_addr;

    // Initialize
    memset(&user, 0, sizeof(user));
    user.username = argv[1];

    cout << "hello, I'm " << user.username << endl;
    
    // create a socket
    sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(9190);

    // connect
    if (connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1) 
        error_handling("connect() error");

    pthread_create(&snd_thread, NULL, send_msg, (void*)&sock);
    pthread_create(&rcv_thread, NULL, recv_msg, (void*)&sock);
    pthread_join(snd_thread, &thread_return);
    pthread_join(rcv_thread, &thread_return);
    close(sock);
    return 0;
    // interchange data
}

