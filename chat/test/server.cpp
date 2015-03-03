
#include "head.h"
#include "hashtable.h"
int clnt_cnt = 0;
int UID = 0;
HashTable user_sock;
pthread_mutex_t mutx;

void *handle_clnt(void *arg) {
    int clnt_sock = *((int*)arg);
    struct req request;
    struct usr user;
    list<Entry>::const_iterator itr;
    list<Entry>::const_iterator sender;
    list<Entry>::const_iterator recver;
    while (recv(clnt_sock, &request, sizeof(request), 0) > 0) {
        // handle online
        if (request.code == 1) {
            cout << "after 1 request code : " << request.code << endl;
            // distribute uid
            pthread_mutex_lock(&mutx);
            // request.user.uid online user
            request.user.uid = UID++; 
            // broadcast online user
            struct req broadcast;
            broadcast.code = 5;
            broadcast.user.uid = request.user.uid;
            broadcast.user.username = request.user.username;
            for (int i = 0; i < 10; i++) {
                for (itr = user_sock.containers[i].begin(); itr != user_sock.containers[i].end(); itr++) {
                    if (send(itr->getSocket(), &broadcast, sizeof(broadcast), 0) < 0)
                        error_handling("send failed");
                }
            }
            // insert in hashtable
            user_sock.Insert(Entry(request.user.uid, clnt_sock, request.user.username));
            pthread_mutex_unlock(&mutx);
            cout << "user id:\t" << request.user.uid << endl;
            cout <<  request.user.username << " is online!" << endl;

            // send uid back
            user.uid = request.user.uid;
            user.username = request.user.username;
            if (send(clnt_sock, &request, sizeof(request), 0) < 0) {
                error_handling("send() error");
            }

        } else if (request.code == 2) {
            // get online user
            pthread_mutex_lock(&mutx);
            for (int i = 0; i < 10; i++) {
                for (itr = user_sock.containers[i].begin(); itr != user_sock.containers[i].end(); itr++) {
                    request.user.uid = itr->getKey();
                    request.user.username = itr->getData();
                    cout << request.user.username << " is online!"<< endl;
                    if (send(clnt_sock, &request, sizeof(request), 0) < 0)
                        error_handling("send failed");
                }
            }
            pthread_mutex_unlock(&mutx);

        } else if (request.code == 3) {
            pthread_mutex_lock(&mutx);
            user_sock.Delete(request.user.uid);
            if (user_sock.Find(request.user.uid, itr))
                cout << "delete fail" << endl;
            else
                cout << request.user.username << " is offline!" << endl;
            pthread_mutex_unlock(&mutx);
            if (send(clnt_sock, &request, sizeof(request), 0) < 0) 
                error_handling("send error");
            break;
        } else if (request.code == 4) {
            pthread_mutex_lock(&mutx);
            user_sock.Find(request.recver, recver);
            user_sock.Find(request.sender, sender);
            pthread_mutex_unlock(&mutx);
            request.user.username = sender->getData();
            if (send(recver->getSocket(), &request, sizeof(request), 0) < 0) 
                error_handling("send failed");
        }
        memset(&request, 0, sizeof(request));

    }
    return NULL;

}
int main() {
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    struct usr tmp;
    list<Entry>::const_iterator itr;
    socklen_t clnt_adr_sz;
    pthread_t t_id;

    

    pthread_mutex_init(&mutx, NULL);
    
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

    while (1) {
        clnt_adr_sz = sizeof(clnt_adr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
        cout << "client sock:\t" << clnt_sock << endl;

        // get client user info
        if (clnt_sock == -1) 
            continue;
        else
            cout << "new client connect..." << endl;

        pthread_create(&t_id, NULL, handle_clnt, (void*) &clnt_sock);
        pthread_detach(t_id);
    }
    close(serv_sock);
    return 0;
}


