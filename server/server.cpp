#include <arpa/inet.h>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;
bool stop = false;

void stopLoop(int sig) {
    stop = true;
}

//Server side
int main(int argc, char *argv[]) {
    signal(SIGINT, &stopLoop);

    int port = 9757;

    sockaddr_in servAddr;
    bzero((char *) &servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(port);

    int serverSd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSd < 0) {
        cerr << "Error establishing the server socket" << endl;
        exit(1);
    }

    int bindStatus = bind(serverSd, (struct sockaddr *) &servAddr,
                          sizeof(servAddr));
    if (bindStatus < 0) {
        cerr << "Error binding socket to local address" << endl;
        exit(1);
    }
    cout << "Waiting for a client to connect..." << endl;

    listen(serverSd, 1);
    sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);

    int newSd = accept(serverSd, (sockaddr *) &newSockAddr, &newSockAddrSize);
    if (newSd < 0) {
        cerr << "Error accepting request from client!" << endl;
        exit(1);
    }
    cout << "Connected!" << endl;
    long bytesWritten = 0;

    struct timeval start1, end1;
    byte msg[1440];
    memset(&msg, 0, sizeof(msg));
    int fd = open("/dev/urandom", O_RDONLY);
    read(fd, msg, 1440);
    gettimeofday(&start1, NULL);
    while (true) {
        if (stop) {
            read(fd, msg, 100);
            send(newSd, &msg, sizeof(msg), 0);
            break;
        }
        bytesWritten += send(newSd, &msg, sizeof(msg), 0);
    }
    gettimeofday(&end1, NULL);
    close(newSd);
    close(serverSd);
    close(fd);

    time_t elapsed_time = (end1.tv_sec - start1.tv_sec);

    cout << "Bytes written: " << bytesWritten << endl;
    cout << "Elapsed time: " << elapsed_time << " secs" << endl;
    cout << "Total bandwidth: " << (bytesWritten / 1024 / 1024 * 8) / elapsed_time << " MBits/s" << endl;
    cout << "Connection closed..." << endl;
    return 0;
}
