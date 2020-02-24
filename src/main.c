#include <time.h>
#include <SDL2/SDL.h>
#include "aquarium.h"
#include "graphics_paths.h"
#include "object.h"
#include "exit_codes.h"

int main(int argc, char ** argv) {
    if (argc < 2) {
        fprintf(stdout, "Example usage: ./aquarium config.xml\n");
        exit(EXIT_NO_CONFIG_FILE);
    }
    srand(time(NULL));
    config_parser_t *config_parser = new_config_parser(argv[1]);
    aquarium_t *aquarium = new_aquarium_from_config(config_parser);
    if (is_created(aquarium)) {
        run_aquarium(aquarium);
    }
    delete_aquarium(aquarium);
}
