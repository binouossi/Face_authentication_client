#include "client.h"
#include <iostream>

//#include "pam_face_authentication.h"
#include "pam_face_defines.h"
#include "webcamImagePaint.h"
#include "opencvWebcam.h"
#include "detector.h"
//#include "verifier.h"




client::client(char* user)
{
    this->user=user;

     /* if((this->sock = socket(AF_INET, SOCK_STREAM, 0))< 0)
        {
          printf("\n Error : Could not create socket \n");
        }*/
      const char* addr;

      addr=this->getaddr().c_str();

      this->serv_addr.sin_family = AF_INET;
      this->serv_addr.sin_port = htons(5000);
     // serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

      this->serv_addr.sin_addr.s_addr = inet_addr(addr);
      /*
      if(connect(this->sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
        {
          printf("\n Error : Connect Failed \n");
          exit(0);
        }*/
}


std::string client::getaddr()
{
    std::ifstream monFlux("/etc/Face_client");
    if(monFlux)
    {
        std::string ligne;
        std::getline(monFlux, ligne);
        return ligne;
    }
    else
    {
        std::cout<<"ERREUR: Impossible d'ouvrir le fichier de configuration." <<std::endl;
        exit(0);
    }
}


int client::communicator()
{
    this->lire();

    char msg[strlen(this->lu)];

    memset(msg,'0',sizeof(msg));

    std::cout<<this->lu<<std::strlen(this->lu)<<std::endl;

    strcpy(msg,this->lu/*,strlen(this->lu)*/);

//    std::cout<<msg<<std::strlen(msg)<<std::endl;

    this->stringsender(this->user);

    IplImage* ji=cvCreateImage( cvSize(IMAGE_WIDTH,IMAGE_HEIGHT),IPL_DEPTH_8U,3);
    cvZero(ji);
    //this->get_im();

    this->IplImagesender(ji);

    int result=NULL;

    this->receive_int(&result);

    std::cout<<result<<std::endl;

    return result;
}


int client::communicator(IplImage *il)
{   
    if((this->sock = socket(AF_INET, SOCK_STREAM, 0))< 0)
      {
        printf("\n Error : Could not create socket \n");
      }
    if(connect(this->sock, (struct sockaddr *)&this->serv_addr, sizeof(this->serv_addr))<0)
      {
        printf("\n Error : Connect Failed \n");
        exit(0);
      } 
   // this->lire();

  //  char msg[strlen(this->lu)];

 //   memset(msg,'0',sizeof(msg));

 //   strcpy(msg,this->lu);

 //   std::cout<<msg<<std::endl;

    this->stringsender(this->user);

    this->IplImagesender(il);

    int result=NULL;



    this->receive_int(&result);

    close(this->sock);

    return result;
}


void client::lire()
{
    int n = 0;

    this->receive_int(&n);
//    std::cout<<n<<std::endl;

    if(n==0)
    {
        this->lu[0]='\0';
        return;
    }

    if(this->lu==NULL)
    {
        this->lu=(char*) malloc(n*sizeof(char));
    }

    this->lu=(char*) realloc(this->lu,(n*sizeof(char)));
    if(this->lu==NULL)
    {
        std::cout<<"Allocation error"<<std::endl;
        return;
    }

    memset(this->lu, '0' ,sizeof(this->lu));

this->lu[0]='\0';
 /*   while((n = read(this->sock, recvBuff, sizeof(recvBuff)-1)) > 0)
      {
        recvBuff[n]='\0';    // explicit null termination: updated based on comments
        std::strcat(this->lu,recvBuff) ;
       recvBuff[0]='\0';        // clear the buffer : I am 99% sure this is not needed now

        a=a+n;
        break;
      }*/

    this->readLine(this->lu,n);
    this->lu[n]='\0';

 //   std::cout<<this->lu<<std::strlen(this->lu)<<std::endl;


      if( n < 0)
      {
        printf("\n Read Error \n");
      }

}

int client::stringsender(char fi[])
{

    int a=strlen(fi);

    this->send_int(a);

    int n=write(this->sock,fi,std::strlen(fi));

    if(n<0)
        std::cout<<"error while sending"<<std::endl;
    return n;
}

int client::IplImagesender(IplImage* im)
{
    this->send_int(im->height);

    this->send_int(im->width);

    int bytes=0;
    cv::Mat frame=cv::Mat(im,true);


    frame = (frame.reshape(0,1)); // to make it continuous

    int  imgSize = frame.total()*frame.elemSize();

    // Send data here
    bytes = send(this->sock, frame.data, imgSize, 0);

    return bytes;
}


client::~client()
{
    if(close(this->sock)==0)
    {
        std::cout<<"connection ended"<<std::endl;
    }

    free(this->lu);
}

IplImage* client::get_im()
{
    IplImage* hi;
  //  cvNamedWindow("Window", CV_WINDOW_AUTOSIZE);
        CvCapture* capture =cvCreateCameraCapture(-1);
        if (!capture){
            printf("Error. Cannot capture.");
        }
        else{
      //      cvNamedWindow("Window", CV_WINDOW_AUTOSIZE);

          //  while (1){
                hi = cvQueryFrame(capture);
                if(!hi){
                    printf("Error. Cannot get the frame.");
                 //   break;
                }
        //   cvShowImage("Window",hi);
          //  cvWaitKey(520);

      //      }
        //    cvReleaseCapture(&capture);
       //     cvDestroyWindow("Window");
        }

        return hi;
}

int client::receive_int(int *num)
{/*

    int32_t ret;
    char *data = (char*)&ret;
    int left = sizeof(ret);
    int rc;
    do {
        rc = read(this->sock, data, left);
        if (ret <= 0) {
            if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                // use select() or epoll() to wait for the socket to be readable again
            }
            else if (errno != EINTR) {
                return -1;
            }
        }
        else {
            data += rc;
            left -= rc;
        }
    }
    while (left > 0);
 //   std::cout<<"ico"<<std::endl;

    *num = ntohl(ret);*/


    char buf[10] = "";

    recv( sock , buf , sizeof buf , 0 );

    sscanf( buf , "%d" , num );

    return 0;
}

int client::send_int(int num)
{/*
    int32_t conv = htonl(num);
    char *data = (char*)&conv;
    int left = sizeof(conv);
    int rc;
    do {
        rc = write(this->sock, data, left);

        //rc=this->stringsender(data);

        if (rc < 0) {
            if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                // use select() or epoll() to wait for the socket to be writable again
            }
            else if (errno != EINTR) {
                return -1;
            }
        }
        else {
            data += rc;
            left -= rc;
        }
    }
    while (left > 0);*/


    char buf[10] = "";

    sprintf( buf , "%d" , num );

    send( this->sock , buf , sizeof buf , 0 );


    return 0;
}

int client::readLine(char data[],int maxlen)
{
   size_t len = 0;
   while (len < maxlen)
   {
      char c;
      int ret = read(this->sock, &c, 1);
      if (ret < 0)
      {
          data[len] = 0;
          return len; // EOF reached
      }
      if (c == '\n')
      {
          data[len] = 0;
          return len; // EOF reached
      }
      data[len++] = c;
   }
}

