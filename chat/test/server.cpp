
#include "head.h"
#include "hashtable.h"
int clnt_cnt = 0;
int clnt_socks[10];
int UID = 0;
HashTable user_sock;

int main() {
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    struct sigaction act;
    struct usr user;
    struct usr tmp;
    pid_t pid;
    list<Entry>::const_iterator itr;
    socklen_t clnt_adr_sz;
    
    // create a socket
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(9190);

    int state;
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    state = sigaction(SIGCHLD, &act, 0);
    // bind
    if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) < 0)
        error_handling("bind() error");

    // listen
    if (listen(serv_sock, SOMAXCONN) == -1) 
        error_handling("listen() error");

    while (1) {
        clnt_adr_sz = sizeof(clnt_adr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
        clnt_socks[clnt_cnt++] = clnt_sock;
        cout << "client sock:\t" << clnt_sock << endl;

        // get client user info
        struct req request;
        if (clnt_sock == -1) 
            continue;
        else
            cout << "new client connect..." << endl;

        pid = fork();

        if (pid == -1) {
            close(clnt_sock);
            continue;
        } 

        if (pid == 0) {
            close(serv_sock);
            while (recv(clnt_sock, &request, sizeof(request), 0) > 0) {
                // handle online
                if (request.code == 1) {
                    // distribute uid
                    request.user.uid = UID++;
                    // insert in hashtable
                    user_sock.Insert(Entry(request.user.uid, clnt_sock, request.user.username));
                    cout << "user id:\t" << request.user.uid << endl;
                    cout <<  request.user.username << " is online!" << endl;

                    // send uid back
                    user.uid = request.user.uid;
                    user.username = request.user.username;
                    if (send(clnt_sock, &user, sizeof(user), 0) < 0) {
                        error_handling("send() error");
                    }

                } else if (request.code == 2) {
                    // get online user
                    for (int i = 0; i < 10; i++) {
                        for (itr = user_sock.containers[i].begin(); itr != user_sock.containers[i].end(); itr++) {
                            request.user.uid = itr->getKey();
                            request.user.username = itr->getData();
                            cout << request.user.username << " is online!"<< endl;
                            if (send(clnt_sock, &request, sizeof(request), 0) < 0)
                                error_handling("send failed");
                        }
                    }

                } else if (request.code == 3) {
                    user_sock.Delete(request.user.uid);
                    if (user_sock.Find(request.user.uid, itr))
                        cout << "delete fail" << endl;
                    else
                        cout << request.user.username << " is offline!" << endl;
                    if (send(clnt_sock, &request, sizeof(request), 0) < 0) 
                        error_handling("send error");
                }

            }
            close(clnt_sock);
            return 0;
        } else {
            close(clnt_sock);
        }
    }
    close(serv_sock);
    return 0;
}


