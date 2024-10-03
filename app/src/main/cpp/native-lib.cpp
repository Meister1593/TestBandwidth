#include <arpa/inet.h>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <jni.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sstream>
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

extern "C" JNIEXPORT jstring JNICALL
Java_com_plyshka_testbandwidth_MainActivity_meow(
        JNIEnv *env,
        jobject /* this */) {

    char *serverIp = "127.0.0.1";
    int port = 9757;

    sockaddr_in sendSockAddr;
    memset(&sendSockAddr, 0, sizeof(sendSockAddr));

    sendSockAddr.sin_family = AF_INET;
    sendSockAddr.sin_addr.s_addr =
            inet_addr(serverIp);
    sendSockAddr.sin_port = htons(port);

    int clientSd = socket(AF_INET, SOCK_STREAM, 0);

    if (clientSd < 0) {
        std::stringstream s;
        s << "Error creating socket!" << strerror(errno);

        return (env)->NewStringUTF(s.str().c_str());
    }

    if (connect(clientSd,
                (sockaddr *) &sendSockAddr, sizeof(sendSockAddr)) < 0) {
        std::stringstream s;
        s << "Error connecting to socket! " << strerror(errno);

        return (env)->NewStringUTF(s.str().c_str());
    }

    cout << "Connected to the server!" << endl;
    int bytesRead = 0;
    struct timeval start1, end1;
    byte msg[1440];

    gettimeofday(&start1, NULL);
    while (true) {
        memset(&msg, 0, sizeof(msg));
        int bytesReadCurrent = recv(clientSd, &msg, sizeof(msg), 0);
        if (bytesReadCurrent == 100) {
            break;
        }
        bytesRead += bytesReadCurrent;
    }
    gettimeofday(&end1, NULL);
    close(clientSd);
    stringstream ss;

    time_t elapsed_time = (end1.tv_sec - start1.tv_sec);
    ss << "Bytes read: " << bytesRead << endl;
    ss << "Elapsed time: " << elapsed_time
       << " secs" << endl;
    ss << "Total bandwidth: " << (bytesRead / 1024 / 1024 * 8) / elapsed_time << " MBits/s" << endl;
    return (env)->NewStringUTF(ss.str().c_str());
}