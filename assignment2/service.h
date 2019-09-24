#define MAX_SERVICE_FULL_PATH_LENGTH 200
#define MAX_SERVICE_NAME_LENGTH 100
#define MAX_TRANSPORT_PROTOCOL_LENGTH 4 //3 + 1 char for end of string
#define MAX_SERVICE_PORT_LENGTH 7 //the same
#define MAX_SERVICE_MODE_LENGTH 7 //the same
/**
 * This header contains the declaration of the service struct. 
 */
typedef struct {
    char service_full_path_name[MAX_SERVICE_FULL_PATH_LENGTH];
    char transport_protocol[MAX_TRANSPORT_PROTOCOL_LENGTH];
    char service_port[MAX_SERVICE_PORT_LENGTH];
    char service_mode[MAX_SERVICE_MODE_LENGTH];
    char service_name[MAX_SERVICE_NAME_LENGTH];
    int socket_file_descriptor;
    int PID;
} service;
