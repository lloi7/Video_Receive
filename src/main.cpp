#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <opencv2/opencv.hpp>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>

using namespace cv;
using namespace std;

int sendall(int fp, uint8_t *data, int len){
	int cnt=0;
	while((cnt+=send(fp,data+cnt,len-cnt,0))<len);
	return cnt;
}

int main(){
	int socketfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(5555);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(socketfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

	listen(socketfd, 64);

	while(1){
		struct sockaddr_in clien_addr;
		socklen_t clien_len = sizeof(clien_addr);
		int conn = accept(socketfd, (struct sockaddr*)&clien_addr, &clien_len);
		if(int pid=fork()){
			close(conn);
		}else{
			printf("Receive connection from: %s:%d\n", inet_ntoa(clien_addr.sin_addr), clien_addr.sin_port);
			auto cap = VideoCapture(0);
			if(!cap.isOpened()){
				printf("Camera open error!\n");
				exit(0);
			}
#define DATA_SIZE (640*480*3)
			while(1){
				Mat image;
				cap >> image;
				sendall(conn, image.data, DATA_SIZE);
				imshow("Src", image);
				if(waitKey(33) == 'q'){
					break;
				}
			}
			exit(0);
		}
	}

	return 0;
}
