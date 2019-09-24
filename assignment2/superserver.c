#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include "myfunction.h"
#include "service.h"
#include "init_services.h"
#include "checkers.h"

//Constants and global variable declaration goes here
#define ff fflush(stdout)
#define LISTEN_BACKLOG 5
service *services;
int services_size;
fd_set sfd_set;

//Service structure definition goes here
    /* See service.h header */

//Function prototype devoted to handle the death of the son process
void handle_signal (int sig);
void execute(service service, char **env);

int main(int argc,char **argv,char **env){ // NOTE: env is the variable to be passed, as last argument, to execle system-call
    // Other variables declaration goes here
    FILE *file = fopen("inetd.conf.txt", "r");
    int max_sfd = -1;
    int sfd;

    int init_services_return_value = init_services(file, &services, &services_size);
    if(init_services_return_value != 0) {
        perror("Service initialization failed");
        exit(init_services_return_value);
    }

    struct sockaddr_in service_addr;
    for(int i = 0; i < services_size; i++) {
        if (is_transport_protocol_udp(services[i].transport_protocol)) {
            sfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
            if(sfd == -1) {
                perror("Socket creation failed");
                exit(1);
            }

            service_addr.sin_family = AF_INET;
            service_addr.sin_port = htons(atoi(services[i].service_port)); // Convert to network byte order
            service_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Bind to any address

            bind(sfd, (struct sockaddr *) &service_addr, sizeof(service_addr));
            
            services[i].socket_file_descriptor = sfd;
        } else if(is_transport_protocol_tcp(services[i].transport_protocol)) {
            sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if(sfd == -1) {
                perror("Socket creation failed");
                exit(1);
            }
            
            service_addr.sin_family = AF_INET;
            service_addr.sin_port = htons(atoi(services[i].service_port)); // Convert to network byte order
            service_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Bind to any address
            
            bind(sfd, (struct sockaddr *) &service_addr, sizeof(service_addr));
            int listen_return_value = listen(sfd, LISTEN_BACKLOG);
            if (listen_return_value < 0) {
                exit(EXIT_FAILURE);
            }
            services[i].socket_file_descriptor = sfd;
        } else {
            fprintf(stderr, "Invalid transport protocol: %s", services[i].transport_protocol);
            exit(EXIT_FAILURE);
        }
        char ip_human_readable[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &service_addr.sin_addr, ip_human_readable, INET_ADDRSTRLEN);
        printf("Opened %s socket on IP: %s and port: %d in %s mode\n", services[i].transport_protocol, ip_human_readable, ntohs(service_addr.sin_port), services[i].service_mode); ff;
        if(services[i].socket_file_descriptor > max_sfd) {
            max_sfd = services[i].socket_file_descriptor;
        }
    }
    signal(SIGCHLD, handle_signal);

    while(1) {
        FD_ZERO(&sfd_set);
        for(int i = 0; i < services_size; i++) {
            if(services[i].PID == 0) { //that's because we have to set all the sockets every time, except those that are in wait mode and have been removed from the set
                FD_SET(services[i].socket_file_descriptor, &sfd_set);
            }
        }
        int select_return_value = select(max_sfd + 1, &sfd_set, NULL, NULL, NULL);
        if(select_return_value == -1) {
            if(errno == 4) { //errno value equals 4 when there has been a SIGCHLD
                continue;
            }
            exit(EXIT_FAILURE);
        }
        for(int i = 0; i < services_size; i++) {
            if(FD_ISSET(services[i].socket_file_descriptor, &sfd_set)) {
                int sfd;
                if(is_transport_protocol_tcp(services[i].transport_protocol)) {
                    struct sockaddr sockaddr;
                    socklen_t size = sizeof(sockaddr);
                    sfd = accept(services[i].socket_file_descriptor, &sockaddr, &size);
                }
                int pid = fork();
                if (pid < 0) {
                    printf("Cannot fork!!\n");
                    exit(1);
                }
                if (pid == 0) { //Son
                    if(is_transport_protocol_tcp(services[i].transport_protocol)) {
                        close(services[i].socket_file_descriptor);
                        services[i].socket_file_descriptor = sfd;
                    }
                    close(0);
                    close(1);
                    close(2);
                    dup(services[i].socket_file_descriptor);
                    dup(services[i].socket_file_descriptor);
                    dup(services[i].socket_file_descriptor);
                    execute(services[i], env);
                } else { //Father
                    printf("Created process %d\n", pid);
                    if(is_transport_protocol_tcp(services[i].transport_protocol)) {
                        close(sfd);
                    }
                    if(is_service_mode_wait(services[i].service_mode)) {
                        services[i].PID = pid;
                        FD_CLR(services[i].socket_file_descriptor, &sfd_set);
                    } else if(!is_service_mode_nowait(services[i].service_mode)) {
                        perror("Invalid service mode");
                        exit(EXIT_FAILURE);
                    }
                    sleep(1);
                }
            }
        }
    }
    return 0;
}

//handle_signal implementation
void handle_signal (int sig){
    // Call to wait system-call goes here
    pid_t pid;
    switch (sig) {
        case SIGCHLD:
            pid = wait(NULL);
            printf("Process %d died.\n", pid); ff;
            for(int i = 0; i < services_size; i++) {
                if(services[i].PID == pid && is_service_mode_wait(services[i].service_mode)) {
                    FD_SET(services[i].socket_file_descriptor, &sfd_set);
                    services[i].PID = 0;
                }
            }
            break;
        default : printf ("Signal not known!\n");
            break;
    }
}

void execute(service service, char **env) {
    execle(service.service_full_path_name, service.service_name, "-l", (char *) 0, env);
    exit(0);
}
