#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <arm_neon.h>
#include <stdalign.h>
#include <assert.h>
#include <getopt.h>

#include "app.h"
#include "tests.h"
#include "draw_mandelbrot.h"

void getFormat(int argc, char* argv[], int* draw_type, int* tests_amount);

int main(int argc, char* argv[]){
    draw_t draw_array[3] = {drawNoOp, drawWithIntrinsics, drawOp};
    int draw_type        = 0;
    int tests_amount     = 0;

    getFormat(argc, argv, &draw_type, &tests_amount);

    if (draw_type < 3){
        runApp(draw_array[draw_type]);
    }
    else{
        double average_time = runTests(tests_amount, draw_type - 3);
    }

    return 0;
}

void getFormat(int argc, char* argv[], int* draw_type, int* tests_amount){
    const char* short_options = "";
    const option long_options[] = {
        {"graphics-no-op", no_argument, NULL, '0'},
        {"graphics-arm", no_argument, NULL, '1'},
        {"graphics-op", no_argument, NULL, '2'},
        {"no-graphics-no-op", no_argument, NULL, '3'},
        {"no-graphics-arm", no_argument, NULL, '4'},
        {"no-graphics-op", no_argument, NULL, '5'},
        {"tests", required_argument, NULL, 't'},
        {NULL, 0, NULL, 0}
    };

    int result           = 0;
    int option_index     = 0;

    while ((result = getopt_long(argc, argv, short_options, long_options, &option_index)) != -1){
        switch(result){
            case '0': *draw_type = 0; break;
            case '1': *draw_type = 1; break;
            case '2': *draw_type = 2; break;
            case '3': *draw_type = 3; break;
            case '4': *draw_type = 4; break;
            case '5': *draw_type = 5; break;
            case 't': *tests_amount = atoi(optarg);
        }
    }
}
