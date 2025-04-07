#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <chrono>

#include "app.h"
#include "draw_mandelbrot.h"
#include "tests.h"

void runTests(int tests_amount, draw_t draw){
    sf::Vertex* video_memory = (sf::Vertex*)calloc(WINDOW_SIZE, sizeof(sf::Vertex));
    ScaleView view = {.scale      = BASE_SCALE,
                      .shift      = BASE_SHIFT,
                      .center_pos = {BASE_CENTER_X, BASE_CENTER_Y}};
    double fps = 0;

    reachTempPlatoOp(draw, video_memory, view);

    for (int k = 0; k < tests_amount; k++){
        auto start = std::chrono::steady_clock::now();

        draw(video_memory, &view);

        auto end     = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        fps = 1.f / ( elapsed.count() / 1000000.f);
        printf("%d %f\n", k, fps);
    }

    free(video_memory);
}

void reachTempPlatoOp(draw_t draw, sf::Vertex* video_memory, ScaleView view){
    auto start = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(start - start);
    while (elapsed.count() < PLATO_MICROSEC){
        draw(video_memory, &view);

        auto end     = std::chrono::steady_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    }
}
