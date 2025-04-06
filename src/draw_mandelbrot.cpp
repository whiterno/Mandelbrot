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
        for (int j = 0; j < WINDOW_HEIGHT; j += 4){
            long long col = i * WINDOW_HEIGHT;
            sf::Color colors[4];

            _VERTEX_4;

            mandelbrotSetPointARMIntrinsics(view, i, j, colors);

            _VERTEX_COLOR_4;

            _VERTEX_MEM_4;
        }
    }
}

void drawNoOp(sf::Vertex* video_memory, ScaleView* view){
    for (int i = 0; i < WINDOW_WIDTH; i++){
        for (int j = 0; j < WINDOW_HEIGHT; j += 1){
            sf::Vertex vertex1 = mandelbrotSetPoint(view, i, j);

            video_memory[i * WINDOW_HEIGHT + j]     = vertex1;
        }
    }
}

void drawOp(sf::Vertex* video_memory, ScaleView* view){
    for (int i = 0; i < WINDOW_WIDTH; i++){
        for (int j = 0; j < WINDOW_HEIGHT; j += PARALLEL_COEF){
            long long col = i * WINDOW_HEIGHT;
            sf::Color colors[PARALLEL_COEF];

            _VERTEX;

            mandelbrotSetPointParallel(view, i, j, colors);

            _VERTEX_COLOR;

            _VERTEX_MEM;
        }
    }
}

sf::Vertex mandelbrotSetPoint(ScaleView* view, float i, float j){
    sf::Vertex vertex{{i, j}, sf::Color::White, {i, j}};
    Complex_t num = {REAL(0), IMAG(0)};

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
    alignas(8) float32_t real[4] = _REAL_4;
    alignas(8) float32_t imag[4] = _IMAG_4;

    int crit[4] = {};

    #if ALGO_OPTIMIZATION == 1

    double ro[4]   = _RO_4;
    double teta[4] = _TETA_4;
    double ro_c[4] = _RO_MAX_4;

    if (_CHECK_RO_4) _BLACK_RET_4

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
        check_coords = vmovq_n_f32(0);

        check_coords = vaddq_f32(real_squared, imag_squared);

        _CRIT_ASSIGN_4;

        imag_members = vmulq_f32(real_members, imag_members);
        imag_members = vmlaq_n_f32(imag_first, imag_members, 2.f);

        real_members = vsubq_f32(real_squared, imag_squared);
        real_members = vaddq_f32(real_members, real_first);

        _CHECK_CRIT_BREAK_4;
    }

    _SET_COLOR_4;
}

void mandelbrotSetPointParallel(ScaleView* view, dot_type i, dot_type j, sf::Color colors[PARALLEL_COEF]){
    alignas(8) dot_type real[PARALLEL_COEF] = _REAL;
    dot_type imag[PARALLEL_COEF] = _IMAG;

    int crit[PARALLEL_COEF] = {};

    #if ALGO_OPTIMIZATION == 1

    double ro[PARALLEL_COEF]   = _RO;
    double teta[PARALLEL_COEF] = _TETA;
    double ro_c[PARALLEL_COEF] = _RO_MAX;

    if (_CHECK_RO) _BLACK_RET

    #endif

    dot_type real_first[PARALLEL_COEF]   = _REAL;
    dot_type imag_first[PARALLEL_COEF]   = _IMAG;
    dot_type check_coords[PARALLEL_COEF] = {};

    dot_type real_squared[PARALLEL_COEF] = {};
    dot_type imag_squared[PARALLEL_COEF] = {};

    for (int k = 0; k < ITERATIONS; k++){
        for (int m = 0; m < PARALLEL_COEF; m++){
            real_squared[m] = real[m] * real[m];
            imag_squared[m] = imag[m] * imag[m];
        }
        for (int m = 0; m < PARALLEL_COEF; m++){
            check_coords[m] = real_squared[m] + imag_squared[m];
        }

        _CRIT_ASSIGN;

        for (int m = 0; m < PARALLEL_COEF; m++){
            imag[m] = 2 * (imag[m] * real[m]) + imag_first[m];
            real[m] = real_squared[m] - imag_squared[m] + real_first[m];
        }

        _CHECK_CRIT_BREAK;
    }

    _SET_COLOR;
}

sf::Color setColor(int crit){
    uint8_t r = (4 * crit) % 255;
    uint8_t g = (6 * crit) % 255;
    uint8_t b = (8 * crit) % 255;

    sf::Color color{r, g, b, 255};

    return color;
}
