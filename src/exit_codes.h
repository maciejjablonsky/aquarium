

#ifndef AQUARIUM_EXIT_CODES_H
#define AQUARIUM_EXIT_CODES_H
#include <stdlib.h>
typedef enum exit_codes {EXIT_OK, EXIT_NO_MEMORY, EXIT_FREE_MEMORY_ERROR, EXIT_XML_PARSING_ERROR,
                         EXIT_IMAGE_NOT_FOUND, EXIT_NO_CONFIG_FILE}
exit_codes_t;

#endif //AQUARIUM_EXIT_CODES_H
