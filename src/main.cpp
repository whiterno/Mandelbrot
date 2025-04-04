#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <arm_neon.h>
#include <stdalign.h>
#include <assert.h>
#include <getopt.h>

#include "app.h"

int main(int argc, char* argv[]){
    const char* short_options = "";
    const option long_options[] = {
        {"graphics", no_argument, NULL, 'g'},
        {"tests", required_argument, NULL, 't'},
        {NULL, 0, NULL, 0}
    };
    int result;
    int option_index;

    while ((result = getopt_long(argc, argv, short_options, long_options, &option_index)) != -1){
        switch(result){
            case 'g':{
                runApp();

                return 0;
            }
            case 'n':{
                int tests_amount = atoi(optarg);
                // double average_fps = runTests(tests, optimization);
            }
        }
    }

    runApp();

    return 0;
}
