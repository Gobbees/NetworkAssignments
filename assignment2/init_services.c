#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "service.h"
#include "init_services.h"

#define MAX_ROW_LENGTH 2048

int init_services(FILE *input_file, service **services, int *services_length) {
    char row[MAX_ROW_LENGTH],
        *service_full_path_name,
        *transport_protocol,
        *service_port,
        *service_mode,
        service_name[MAX_SERVICE_NAME_LENGTH],
        *tmp_service_name,
        *row_correctness;
        
    if (input_file == NULL) {
        perror("input_file must not be null.\n");
        return 1;
    }

    service *services_list = (service *) calloc(10, sizeof(service));
    if(services == NULL) {
        perror("services initialization failed.");
        return 2;
    }

    int size = 10;
    int row_count = 0;
    while(fgets(row, MAX_ROW_LENGTH, input_file) != NULL) {
        service_full_path_name = strtok(row, " ");
        transport_protocol = strtok(NULL, " ");
        service_port = strtok(NULL, " ");
        service_mode = strtok(NULL, " \n");
        row_correctness = strtok(NULL, " \n");
        
        if(row_correctness != NULL || service_full_path_name == NULL || transport_protocol == NULL || service_port == NULL || service_mode == NULL) {
            fprintf(stderr, "The row %d is not well formatted. ", (row_count + 1));
            return 3;
        }
        
        strcpy(services_list[row_count].service_full_path_name, service_full_path_name);
        strcpy(services_list[row_count].transport_protocol, transport_protocol);
        strcpy(services_list[row_count].service_port, service_port);
        strcpy(services_list[row_count].service_mode, service_mode);
        services_list[row_count].socket_file_descriptor = 0;
        services_list[row_count].PID = 0;

        tmp_service_name = strtok(service_full_path_name, "/");
        while(1) {
            if(tmp_service_name != NULL) {
                strcpy(service_name, tmp_service_name);
                tmp_service_name = strtok(NULL, "/");
            } else {
                break;
            }
        }
        strcpy(services_list[row_count++].service_name, service_name);

        if(row_count == size) {
            size *= 2;
            services_list = (service *)realloc(services, size * sizeof(service));
            if(services == NULL) {
                perror("services initialization failed.");
                return 2;
            }
        }
    }
    fclose(input_file);
    
    *services_length = row_count;
    *services = services_list;
    
    return 0;
}