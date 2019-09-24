/**
 * Checks if protocol is tcp.
 * It returns:
 * 1 if protocol is tcp (upper or lower case)
 * 0 otherwise
 */
int is_transport_protocol_tcp(char *protocol) {
    return strcmp(protocol, "tcp") == 0 || strcmp(protocol, "TCP") == 0 ? 1 : 0;
}

/**
 * Checks if protocol is udp.
 * It returns:
 * 1 if protocol is udp (upper or lower case)
 * 0 otherwise
 */
int is_transport_protocol_udp(char *protocol) {
    return strcmp(protocol, "udp") == 0 || strcmp(protocol, "UDP") == 0 ? 1 : 0;
}

/**
 * Checks if mode is wait.
 * It returns:
 * 1 if protocol is wait (upper or lower case)
 * 0 otherwise
 */
int is_service_mode_wait(char *mode) {
    return strcmp(mode, "wait") == 0 || strcmp(mode, "WAIT") == 0 ? 1 : 0;
}

/**
 * Checks if mode is nowait.
 * It returns:
 * 1 if protocol is nowait (upper or lower case)
 * 0 otherwise
 */
int is_service_mode_nowait(char *mode) {
    return strcmp(mode, "nowait") == 0 || strcmp(mode, "NOWAIT") == 0 ? 1 : 0;
}