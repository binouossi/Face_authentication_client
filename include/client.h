#ifndef CLIENT_H
#define CLIENT_H

//standar
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>
//#include <boost/asio.hpp>

//network

//#include <QtNetwork>


#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>





//opencv
#include <opencv/cv.h>
#include <opencv/highgui.h>












class client//: public QObject
{
 //   Q_OBJECT
public:
    client(char* user);
    ~client();
    std::string getaddr();
    int IplImagesender(IplImage* im);
    //int sender(auto fi);
    int stringsender(char *fi);
    int communicator(IplImage* il);
    int communicator();
    IplImage* get_im();


private:
    void lire();
    void viderBuffer();
    int sock=NULL;
    char IplsendBuff[sizeof(IplImage)];
    char recvBuff[1024];
    char* IplImageencode(IplImage *ni,char * img);
    int receive_int(int *num);
    int readLine(char data[],int maxlen);
    char* user;
    struct sockaddr_in serv_addr;


    char* lu;

    int send_int(int num);

};

#endif // CLIENT_H
