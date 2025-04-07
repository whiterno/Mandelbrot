#ifndef DRAW_MANDELBROT_H
#define DRAW_MANDELBROT_H

#ifndef ALGO_OPTIMIZATION
#define ALGO_OPTIMIZATION 0
#endif

#ifndef UNROLL_COEF
#define UNROLL_COEF 4
#endif

const int ARM_COEF = 4;

void drawWithIntrinsics(sf::Vertex* video_memory, ScaleView* view);

void drawNoOp(sf::Vertex* video_memory, ScaleView* view);

void drawOp(sf::Vertex* video_memory, ScaleView* view);

sf::Vertex mandelbrotSetPoint(ScaleView* view, float i, float j);

void mandelbrotSetPointARMIntrinsics(ScaleView* view, float i, float j, sf::Color colors[4]);

void mandelbrotSetPointParallel(ScaleView* view, DOT_TYPE i, DOT_TYPE j, sf::Color colors[UNROLL_COEF]);

sf::Color setColor(int crit);

#define REAL(inx)           (i - view->center_pos.x) / (view->scale * DOTS_PER_PIXEL)
#define IMAG(inx)           (j + inx - view->center_pos.y) / (view->scale * DOTS_PER_PIXEL)

#define RO(inx)             sqrt((real[inx] - 1.f/4) * (real[inx] - 1.f/4) + imag[inx] * imag[inx])
#define TETA(inx)           atan2(imag[inx], real[inx] - 1.f/4)
#define RO_MAX(inx)         1.f/2 -1.f/2 * cos(teta[inx])
#define CHECK_RO(inx)       ro[inx] <= ro_c[inx]

#define BLACK(inx)          colors[inx] = sf::Color::Black
#define SET_COLOR(inx)      colors[inx] = setColor(crit[inx])

#define CRIT_ASSIGN(inx)    crit[inx] = (check_coords[inx] > 4 && crit[inx] == 0) ? k : crit[inx]

#define VERTEX(inx)         sf::Vertex vertex ## inx{{(float)i, (float)j + inx}, sf::Color::White, {(float)i, (float)j + inx}}
#define VERTEX_COLOR(inx)   vertex ## inx.color = colors[inx]
#define VERTEX_MEM(inx)     video_memory[col + j + inx] = vertex ## inx

#define _REAL_4             {REAL(0), REAL(0), REAL(0), REAL(0)}
#define _IMAG_4             {IMAG(0), IMAG(1), IMAG(2), IMAG(3)}
#define _RO_4               {RO(0), RO(1), RO(2), RO(3)}
#define _TETA_4             {TETA(0), TETA(1), TETA(2), TETA(3)}
#define _RO_MAX_4           {RO_MAX(0), RO_MAX(1), RO_MAX(2), RO_MAX(3)}
#define _CHECK_RO_4         CHECK_RO(0) && CHECK_RO(1) && CHECK_RO(2) && CHECK_RO(3)
#define _BLACK_RET_4        {BLACK(0); BLACK(1); BLACK(2); BLACK(3); return;}
#define _CRIT_ASSIGN_4      CRIT_ASSIGN(0); CRIT_ASSIGN(1); CRIT_ASSIGN(2); CRIT_ASSIGN(3)
#define _CHECK_CRIT_BREAK_4 if (crit[0] != 0 && crit[1] != 0 && crit[2] != 0 && crit[3] != 0) break;
#define _SET_COLOR_4        SET_COLOR(0); SET_COLOR(1); SET_COLOR(2); SET_COLOR(3)
#define _VERTEX_4           VERTEX(0); VERTEX(1); VERTEX(2); VERTEX(3)
#define _VERTEX_COLOR_4     VERTEX_COLOR(0); VERTEX_COLOR(1); VERTEX_COLOR(2); VERTEX_COLOR(3)
#define _VERTEX_MEM_4       VERTEX_MEM(0); VERTEX_MEM(1); VERTEX_MEM(2); VERTEX_MEM(3)

#endif
