#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <arm_neon.h>
#include <stdalign.h>
#include <assert.h>

#include "complex_nums.h"
#include "app.h"
#include "draw_mandelbrot.h"

void drawWithIntrinsics(sf::Vertex* video_memory, ScaleView* view){
    for (int i = 0; i < WINDOW_WIDTH; i++){
        for (int j = 0; j < WINDOW_HEIGHT; j += ARM_COEF){
            long long col = i * WINDOW_HEIGHT;
            sf::Color colors[ARM_COEF];
            sf::Vertex vertexes[ARM_COEF];

            for (int k = 0; k < ARM_COEF; k++) vertexes[k] = {{(float)i, (float)j + k}, sf::Color::White, {(float)i, (float)j + k}};

            mandelbrotSetPointARMIntrinsics(view, i, j, colors);

            for (int k = 0; k < ARM_COEF; k++) vertexes[k].color = colors[k];

            for (int k = 0; k < ARM_COEF; k++) video_memory[col + j + k] = vertexes[k];
        }
    }
}

void drawNoOp(sf::Vertex* video_memory, ScaleView* view){
    for (int i = 0; i < WINDOW_WIDTH; i++){
        for (int j = 0; j < WINDOW_HEIGHT; j += 1){
            sf::Vertex vertex1 = mandelbrotSetPoint(view, i, j);

            video_memory[i * WINDOW_HEIGHT + j] = vertex1;
        }
    }
}

void drawOp(sf::Vertex* video_memory, ScaleView* view){
    for (int i = 0; i < WINDOW_WIDTH; i++){
        for (int j = 0; j < WINDOW_HEIGHT; j += UNROLL_COEF){
            long long col = i * WINDOW_HEIGHT;
            sf::Color colors[UNROLL_COEF];
            sf::Vertex vertexes[UNROLL_COEF];

            for (int k = 0; k < UNROLL_COEF; k++) vertexes[k] = {{(float)i, (float)j + k}, sf::Color::White, {(float)i, (float)j + k}};

            mandelbrotSetPointParallel(view, i, j, colors);

            for (int k = 0; k < UNROLL_COEF; k++) vertexes[k].color = colors[k];

            for (int k = 0; k < UNROLL_COEF; k++) video_memory[col + j + k] = vertexes[k];
        }
    }
}

sf::Vertex mandelbrotSetPoint(ScaleView* view, float i, float j){
    sf::Vertex vertex{{i, j}, sf::Color::White, {i, j}};
    Complex_t num = {(i - view->center_pos.x) / (view->scale * DOTS_PER_PIXEL),
                     (j - view->center_pos.y) / (view->scale * DOTS_PER_PIXEL)};

    #if ALGO_OPTIMIZATION == 1

    double ro    = sqrt((num.real - 1.f/4) * (num.real - 1.f/4) + num.imag * num.imag);
    double teta  = atan2(num.imag, num.real - 1.f/4);
    double ro_c  = 1.f/2 -1.f/2 * cos(teta);

    if (ro <= ro_c){
        vertex.color = sf::Color::Black;

        return vertex;
    }

    #endif

    int crit = 0;
    Complex_t seq_mem = num;

    for (int k = 1; k < ITERATIONS; k += 1){
        Complex_t square = mulC(seq_mem, seq_mem);
        seq_mem = addC(square, num);

        if (modC(seq_mem) >= 4){
            crit = k;
            break;
        }
    }

    vertex.color = setColor(crit);

    return vertex;
}

void mandelbrotSetPointARMIntrinsics(ScaleView* view, float i, float j, sf::Color colors[4]){
    alignas(8) float32_t real[ARM_COEF] = {};
    alignas(8) float32_t imag[ARM_COEF] = {};

    for (int k = 0; k < ARM_COEF; k++) real[k] = (i - float(view->center_pos.x)) / (view->scale * DOTS_PER_PIXEL);
    for (int k = 0; k < ARM_COEF; k++) imag[k] = (j + k - float(view->center_pos.y)) / (view->scale * DOTS_PER_PIXEL);

    int crit[4] = {};

    #if ALGO_OPTIMIZATION == 1

    double ro[ARM_COEF]   = {};
    double teta[ARM_COEF] = {};
    double ro_c[ARM_COEF] = {};

    for (int k = 0; k < ARM_COEF; k++) ro[k]   = sqrt((real[k] - 1.f/4) * (real[k] - 1.f/4) + imag[k] * imag[k]);
    for (int k = 0; k < ARM_COEF; k++) teta[k] = atan2(imag[k], real[k] - 1.f/4);
    for (int k = 0; k < ARM_COEF; k++) ro_c[k] = 1.f/2 -1.f/2 * cos(teta[k]);

    bool check_ro = 1;
    for (int k = 0; k < ARM_COEF; k++) check_ro = check_ro && (ro[k] <= ro_c[k]);

    if (check_ro){
        for (int k = 0; k < ARM_COEF; k++) colors[k] = sf::Color::Black;

        return;
    }

    #endif

    alignas(16) float32x4_t real_members = vld1q_f32(real);
    alignas(16) float32x4_t imag_members = vld1q_f32(imag);
    alignas(16) float32x4_t real_first = real_members;
    alignas(16) float32x4_t imag_first = imag_members;

    alignas(16) float32x4_t real_squared = {};
    alignas(16) float32x4_t imag_squared = {};
    alignas(16) float32x4_t check_coords = {};

    for (int k = 0; k < ITERATIONS; k++){
        real_squared = vmulq_f32(real_members, real_members);
        imag_squared = vmulq_f32(imag_members, imag_members);

        check_coords = vaddq_f32(real_squared, imag_squared);

        for (int m = 0; m < ARM_COEF; m++) crit[m] = (check_coords[m] > 4 && crit[m] == 0) ? k : crit[m];

        imag_members = vmulq_f32(real_members, imag_members);
        imag_members = vmlaq_n_f32(imag_first, imag_members, 2.f);

        real_members = vsubq_f32(real_squared, imag_squared);
        real_members = vaddq_f32(real_members, real_first);

        bool crit_flag = 1;
        for (int m = 0; m < ARM_COEF; m++) crit_flag = crit_flag && (crit[m] != 0);
    }

    for (int k = 0; k < ARM_COEF; k++) colors[k] = setColor(crit[k]);
}

void mandelbrotSetPointParallel(ScaleView* view, DOT_TYPE i, DOT_TYPE j, sf::Color colors[UNROLL_COEF]){
    DOT_TYPE real[UNROLL_COEF] = {};
    DOT_TYPE imag[UNROLL_COEF] = {};

    for (int k = 0; k < UNROLL_COEF; k++) real[k] = (i - view->center_pos.x) / (view->scale * DOTS_PER_PIXEL);
    for (int k = 0; k < UNROLL_COEF; k++) imag[k] = (j + k - view->center_pos.y) / (view->scale * DOTS_PER_PIXEL);

    int crit[UNROLL_COEF] = {};

    #if ALGO_OPTIMIZATION == 1

    double ro[UNROLL_COEF]   = {};
    double teta[UNROLL_COEF] = {};
    double ro_c[UNROLL_COEF] = {};

    for (int k = 0; k < UNROLL_COEF; k++) ro[k]   = sqrt((real[k] - 1.f/4) * (real[k] - 1.f/4) + imag[k] * imag[k]);
    for (int k = 0; k < UNROLL_COEF; k++) teta[k] = atan2(imag[k], real[k] - 1.f/4);
    for (int k = 0; k < UNROLL_COEF; k++) ro_c[k] = 1.f/2 -1.f/2 * cos(teta[k]);

    bool check_ro = 1;
    for (int k = 0; k < UNROLL_COEF; k++) check_ro = check_ro && (ro[k] <= ro_c[k]);

    if (check_ro){
        for (int k = 0; k < UNROLL_COEF; k++) colors[k] = sf::Color::Black;

        return;
    }

    #endif

    DOT_TYPE real_first[UNROLL_COEF]   = {};
    DOT_TYPE imag_first[UNROLL_COEF]   = {};
    DOT_TYPE check_coords[UNROLL_COEF] = {};

    DOT_TYPE real_squared[UNROLL_COEF] = {};
    DOT_TYPE imag_squared[UNROLL_COEF] = {};

    for (int k = 0; k < UNROLL_COEF; k++) real_first[k] = (i - view->center_pos.x) / (view->scale * DOTS_PER_PIXEL);
    for (int k = 0; k < UNROLL_COEF; k++) imag_first[k] = (j + k - view->center_pos.y) / (view->scale * DOTS_PER_PIXEL);

    for (int k = 0; k < ITERATIONS; k++){
        for (int m = 0; m < UNROLL_COEF; m++){
            real_squared[m] = real[m] * real[m];
            imag_squared[m] = imag[m] * imag[m];
        }
        for (int m = 0; m < UNROLL_COEF; m++){
            check_coords[m] = real_squared[m] + imag_squared[m];
        }

        for (int m = 0; m < UNROLL_COEF; m++) crit[m] = (check_coords[m] > 4 && crit[m] == 0) ? k : crit[m];


        for (int m = 0; m < UNROLL_COEF; m++){
            imag[m] = 2 * (imag[m] * real[m]) + imag_first[m];
            real[m] = real_squared[m] - imag_squared[m] + real_first[m];
        }

        bool crit_flag = 1;
        for (int m = 0; m < UNROLL_COEF; m++) crit_flag = crit_flag && (crit[m] != 0);

        if (crit_flag) break;
    }

    for (int k = 0; k < UNROLL_COEF; k++) colors[k] = setColor(crit[k]);
}

sf::Color setColor(int crit){
    uint8_t r = (4 * crit) % 255;
    uint8_t g = (6 * crit) % 255;
    uint8_t b = (8 * crit) % 255;

    sf::Color color{r, g, b, 255};

    return color;
}
