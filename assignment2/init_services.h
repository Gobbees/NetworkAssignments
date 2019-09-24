/**
 * Reads all the lines of input_file and creates a list of services.
 * It accepts exactly 4 parameters for each line of the input file.
 * After the function executes:
 * -services parameter contains the list of services.
 * -services_length contains the number of services.
 * 
 * The function returns:
 * 0 if all goes well.
 * 1 if some input parameter is invalid.
 * 2 if some initialization fails.
 * 3 if the input file contains errors.
 */
int init_services(FILE *input_file, service **services, int *services_length);