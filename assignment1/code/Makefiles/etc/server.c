// Socket programming :server side C/C++ program

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080

int new_socket, valread, server_fd;
struct sockaddr_in address;
int opt = 1;
int addrlength = sizeof(address);
char buffer[1024] = {0};
char *hello = "Server says hello";

pid_t childpid;

int function_socket(){
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("socket got failed!");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    if(server_fd==-1){
        perror("socket");
    }

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0){
        perror("bind got failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0){
        perror("listen");
        exit(EXIT_FAILURE);
    }
    return 0;
}

int function_data(){
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlength))<0){
        perror("accept");
        exit(EXIT_FAILURE);
    }
      if(childpid = fork() == 0){
        close(server_fd);
        int a= setuid(getuid());

        if(setuid(-1)== 0){
            exit(EXIT_FAILURE);
	}
        valread = read( new_socket , buffer, 1024);
        printf("%s\n",buffer );
        send(new_socket , hello , strlen(hello) , 0 );
        printf("Hello message is sent\n");

    }
    close(new_socket);

    if(childpid<0){
        perror("accept");
        exit(EXIT_FAILURE);
    }

    return 0;
}

int main(int argc, char const *argv[]){
    function_socket();
    function_data();
    return 0;
}
