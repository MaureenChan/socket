
#include "head.h"
#include "hashtable.h"
int clnt_cnt = 0;
int clnt_socks[10];
int UID = 0;

int main() {
    HashTable user_sock;
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    struct usr user;
    struct usr tmp;
    list<Entry>::const_iterator itr;
    socklen_t clnt_adr_sz;
    
    // create a socket
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(9190);

    // bind
    if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) < 0)
        error_handling("bind() error");

    // listen
    if (listen(serv_sock, SOMAXCONN) == -1) 
        error_handling("listen() error");

    while (clnt_cnt < 10) {
        clnt_adr_sz = sizeof(clnt_adr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
        clnt_socks[clnt_cnt++] = clnt_sock;
        cout << "client sock:\t" << clnt_sock << endl;

        // get client user info
        struct req request;
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
                struct req online_user;
                for (int i = 0; i < 10; i++) {
                    for (itr = user_sock.containers[i].begin(); itr != user_sock.containers[i].end(); itr++) {
                        online_user.user.uid = itr->getKey();
                        online_user.user.username = itr->getData();
                        cout << online_user.user.username << " is online!"<< endl;
                        if (send(clnt_sock, &online_user, sizeof(online_user), 0) < 0)
                            error_handling("send failed");
                    }
                }

            } else if (request.code == 3) {
                user_sock.Delete(request.user.uid);
                cout << "In 3" << endl;
                if (user_sock.Find(request.user.uid, itr))
                    cout << "delete fail" << endl;
                else
                    cout << request.user.username << " is offline!" << endl;
                if (send(clnt_sock, &request, sizeof(request), 0) < 0) 
                    error_handling("send error");
            }

        }



        //// test whether in hash



    }
    close(serv_sock);
    return 0;
}


