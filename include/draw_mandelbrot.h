#ifndef DRAW_MANDELBROT_H
#define DRAW_MANDELBROT_H

#define _PARALLEL_COEF 16
typedef double dot_type;

const int PARALLEL_COEF = _PARALLEL_COEF;

void drawWithIntrinsics(sf::Vertex* video_memory,
                        sf::Vector2f center_pos,
                        float scale);
void drawNoOp(sf::Vertex* video_memory,
              sf::Vector2f center_pos,
              float scale);
void drawOp(sf::Vertex* video_memory,
            sf::Vector2f center_pos,
            float scale);

sf::Vertex mandelbrotSetPoint(sf::Vector2f center_pos,
                              float i,
                              float j,
                              double scale);

void mandelbrotSetPointARMIntrinces(sf::Vector2f center_pos,
                                    float i,
                                    float j,
                                    float scale,
                                    sf::Color colors[4]);

void mandelbrotSetPointParallel(sf::Vector2f center_pos,
                                dot_type i,
                                dot_type j,
                                dot_type scale,
                                sf::Color colors[PARALLEL_COEF]);

sf::Color setColor(int crit);

#define REAL(inx)           (i - center_pos.x) / (scale * 300)
#define IMAG(inx)           (j + inx - center_pos.y) / (scale * 300)

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

#define _REAL_8             {REAL(0), REAL(0), REAL(0), REAL(0), REAL(0), REAL(0), REAL(0), REAL(0)}
#define _IMAG_8             {IMAG(0), IMAG(1), IMAG(2), IMAG(3), IMAG(4), IMAG(5), IMAG(6), IMAG(7)}
#define _RO_8               {RO(0), RO(1), RO(2), RO(3), RO(4), RO(5), RO(6), RO(7)}
#define _TETA_8             {TETA(0), TETA(1), TETA(2), TETA(3), TETA(4), TETA(5), TETA(6), TETA(7)}
#define _RO_MAX_8           {RO_MAX(0), RO_MAX(1), RO_MAX(2), RO_MAX(3), RO_MAX(4), RO_MAX(5), RO_MAX(6), RO_MAX(7)}
#define _CHECK_RO_8         CHECK_RO(0) && CHECK_RO(1) && CHECK_RO(2) && CHECK_RO(3) && CHECK_RO(4) && CHECK_RO(5) && CHECK_RO(6) && CHECK_RO(7)
#define _BLACK_RET_8        {BLACK(0); BLACK(1); BLACK(2); BLACK(3); BLACK(4); BLACK(5); BLACK(6); BLACK(7); return;}
#define _CRIT_ASSIGN_8      CRIT_ASSIGN(0); CRIT_ASSIGN(1); CRIT_ASSIGN(2); CRIT_ASSIGN(3); CRIT_ASSIGN(4); CRIT_ASSIGN(5); CRIT_ASSIGN(6); CRIT_ASSIGN(7)
#define _CHECK_CRIT_BREAK_8 if (crit[0] != 0 && crit[1] != 0 && crit[2] != 0 && crit[3] != 0 && crit[4] != 0 && crit[5] != 0 && crit[6] != 0 && crit[7] != 0) break;
#define _SET_COLOR_8        SET_COLOR(0); SET_COLOR(1); SET_COLOR(2); SET_COLOR(3); SET_COLOR(4); SET_COLOR(5); SET_COLOR(6); SET_COLOR(7)
#define _VERTEX_8           VERTEX(0); VERTEX(1); VERTEX(2); VERTEX(3); VERTEX(4); VERTEX(5); VERTEX(6); VERTEX(7)
#define _VERTEX_COLOR_8     VERTEX_COLOR(0); VERTEX_COLOR(1); VERTEX_COLOR(2); VERTEX_COLOR(3); VERTEX_COLOR(4); VERTEX_COLOR(5); VERTEX_COLOR(6); VERTEX_COLOR(7)
#define _VERTEX_MEM_8       VERTEX_MEM(0); VERTEX_MEM(1); VERTEX_MEM(2); VERTEX_MEM(3); VERTEX_MEM(4); VERTEX_MEM(5); VERTEX_MEM(6); VERTEX_MEM(7)

#define _REAL_12            {REAL(0), REAL(0), REAL(0), REAL(0), REAL(0), REAL(0), REAL(0), REAL(0), REAL(0), REAL(0), REAL(0), REAL(0)}
#define _IMAG_12            {IMAG(0), IMAG(1), IMAG(2), IMAG(3), IMAG(4), IMAG(5), IMAG(6), IMAG(7), IMAG(8), IMAG(9), IMAG(10), IMAG(11)}
#define _RO_12              {RO(0), RO(1), RO(2), RO(3), RO(4), RO(5), RO(6), RO(7), RO(8), RO(9), RO(10), RO(11)}
#define _TETA_12            {TETA(0), TETA(1), TETA(2), TETA(3), TETA(4), TETA(5), TETA(6), TETA(7), TETA(8), TETA(9), TETA(10), TETA(11)}
#define _RO_MAX_12          {RO_MAX(0), RO_MAX(1), RO_MAX(2), RO_MAX(3), RO_MAX(4), RO_MAX(5), RO_MAX(6), RO_MAX(7), RO_MAX(8), RO_MAX(9), RO_MAX(10), RO_MAX(11)}
#define _CHECK_RO_12        CHECK_RO(0) && CHECK_RO(1) && CHECK_RO(2) && CHECK_RO(3) && CHECK_RO(4) && CHECK_RO(5) && CHECK_RO(6) && CHECK_RO(7) && CHECK_RO(8) && CHECK_RO(9) && CHECK_RO(10) && CHECK_RO(11)
#define _BLACK_RET_12       {BLACK(0); BLACK(1); BLACK(2); BLACK(3); BLACK(4); BLACK(5); BLACK(6); BLACK(7); BLACK(8); BLACK(9); BLACK(10); BLACK(11); return;}
#define _CRIT_ASSIGN_12     CRIT_ASSIGN(0); CRIT_ASSIGN(1); CRIT_ASSIGN(2); CRIT_ASSIGN(3); CRIT_ASSIGN(4); CRIT_ASSIGN(5); CRIT_ASSIGN(6); CRIT_ASSIGN(7); CRIT_ASSIGN(8); CRIT_ASSIGN(9); CRIT_ASSIGN(10); CRIT_ASSIGN(11)
#define _CHECK_CRIT_BREAK_12 if (crit[0] != 0 && crit[1] != 0 && crit[2] != 0 && crit[3] != 0 && crit[4] != 0 && crit[5] != 0 && crit[6] != 0 && crit[7] != 0 && crit[8] != 0 && crit[9] != 0 && crit[10] != 0 && crit[11] != 0) break;
#define _SET_COLOR_12       SET_COLOR(0); SET_COLOR(1); SET_COLOR(2); SET_COLOR(3); SET_COLOR(4); SET_COLOR(5); SET_COLOR(6); SET_COLOR(7); SET_COLOR(8); SET_COLOR(9); SET_COLOR(10); SET_COLOR(11)
#define _VERTEX_12          VERTEX(0); VERTEX(1); VERTEX(2); VERTEX(3); VERTEX(4); VERTEX(5); VERTEX(6); VERTEX(7); VERTEX(8); VERTEX(9); VERTEX(10); VERTEX(11)
#define _VERTEX_COLOR_12    VERTEX_COLOR(0); VERTEX_COLOR(1); VERTEX_COLOR(2); VERTEX_COLOR(3); VERTEX_COLOR(4); VERTEX_COLOR(5); VERTEX_COLOR(6); VERTEX_COLOR(7); VERTEX_COLOR(8); VERTEX_COLOR(9); VERTEX_COLOR(10); VERTEX_COLOR(11)
#define _VERTEX_MEM_12      VERTEX_MEM(0); VERTEX_MEM(1); VERTEX_MEM(2); VERTEX_MEM(3); VERTEX_MEM(4); VERTEX_MEM(5); VERTEX_MEM(6); VERTEX_MEM(7); VERTEX_MEM(8); VERTEX_MEM(9); VERTEX_MEM(10); VERTEX_MEM(11)

#define _REAL_16            {REAL(0), REAL(1), REAL(2), REAL(3), REAL(4), REAL(5), REAL(6), REAL(7), REAL(8), REAL(9), REAL(10), REAL(11), REAL(12), REAL(13), REAL(14), REAL(15)}
#define _IMAG_16            {IMAG(0), IMAG(1), IMAG(2), IMAG(3), IMAG(4), IMAG(5), IMAG(6), IMAG(7), IMAG(8), IMAG(9), IMAG(10), IMAG(11), IMAG(12), IMAG(13), IMAG(14), IMAG(15)}
#define _RO_16              {RO(0), RO(1), RO(2), RO(3), RO(4), RO(5), RO(6), RO(7), RO(8), RO(9), RO(10), RO(11), RO(12), RO(13), RO(14), RO(15)}
#define _TETA_16            {TETA(0), TETA(1), TETA(2), TETA(3), TETA(4), TETA(5), TETA(6), TETA(7), TETA(8), TETA(9), TETA(10), TETA(11), TETA(12), TETA(13), TETA(14), TETA(15)}
#define _RO_MAX_16          {RO_MAX(0), RO_MAX(1), RO_MAX(2), RO_MAX(3), RO_MAX(4), RO_MAX(5), RO_MAX(6), RO_MAX(7), RO_MAX(8), RO_MAX(9), RO_MAX(10), RO_MAX(11), RO_MAX(12), RO_MAX(13), RO_MAX(14), RO_MAX(15)}
#define _CHECK_RO_16        CHECK_RO(0) && CHECK_RO(1) && CHECK_RO(2) && CHECK_RO(3) && CHECK_RO(4) && CHECK_RO(5) && CHECK_RO(6) && CHECK_RO(7) && CHECK_RO(8) && CHECK_RO(9) && CHECK_RO(10) && CHECK_RO(11) && CHECK_RO(12) && CHECK_RO(13) && CHECK_RO(14) && CHECK_RO(15)
#define _BLACK_RET_16       {BLACK(0); BLACK(1); BLACK(2); BLACK(3); BLACK(4); BLACK(5); BLACK(6); BLACK(7); BLACK(8); BLACK(9); BLACK(10); BLACK(11); BLACK(12); BLACK(13); BLACK(14); BLACK(15); return;}
#define _CRIT_ASSIGN_16     CRIT_ASSIGN(0); CRIT_ASSIGN(1); CRIT_ASSIGN(2); CRIT_ASSIGN(3); CRIT_ASSIGN(4); CRIT_ASSIGN(5); CRIT_ASSIGN(6); CRIT_ASSIGN(7); CRIT_ASSIGN(8); CRIT_ASSIGN(9); CRIT_ASSIGN(10); CRIT_ASSIGN(11); CRIT_ASSIGN(12); CRIT_ASSIGN(13); CRIT_ASSIGN(14); CRIT_ASSIGN(15)
#define _CHECK_CRIT_BREAK_16 if (crit[0] != 0 && crit[1] != 0 && crit[2] != 0 && crit[3] != 0 && crit[4] != 0 && crit[5] != 0 && crit[6] != 0 && crit[7] != 0 && crit[8] != 0 && crit[9] != 0 && crit[10] != 0 && crit[11] != 0 && crit[12] != 0 && crit[13] != 0 && crit[14] != 0 && crit[15] != 0) break;
#define _SET_COLOR_16       SET_COLOR(0); SET_COLOR(1); SET_COLOR(2); SET_COLOR(3); SET_COLOR(4); SET_COLOR(5); SET_COLOR(6); SET_COLOR(7); SET_COLOR(8); SET_COLOR(9); SET_COLOR(10); SET_COLOR(11); SET_COLOR(12); SET_COLOR(13); SET_COLOR(14); SET_COLOR(15)
#define _VERTEX_16          VERTEX(0); VERTEX(1); VERTEX(2); VERTEX(3); VERTEX(4); VERTEX(5); VERTEX(6); VERTEX(7); VERTEX(8); VERTEX(9); VERTEX(10); VERTEX(11); VERTEX(12); VERTEX(13); VERTEX(14); VERTEX(15)
#define _VERTEX_COLOR_16    VERTEX_COLOR(0); VERTEX_COLOR(1); VERTEX_COLOR(2); VERTEX_COLOR(3); VERTEX_COLOR(4); VERTEX_COLOR(5); VERTEX_COLOR(6); VERTEX_COLOR(7); VERTEX_COLOR(8); VERTEX_COLOR(9); VERTEX_COLOR(10); VERTEX_COLOR(11); VERTEX_COLOR(12); VERTEX_COLOR(13); VERTEX_COLOR(14); VERTEX_COLOR(15)
#define _VERTEX_MEM_16      VERTEX_MEM(0); VERTEX_MEM(1); VERTEX_MEM(2); VERTEX_MEM(3); VERTEX_MEM(4); VERTEX_MEM(5); VERTEX_MEM(6); VERTEX_MEM(7); VERTEX_MEM(8); VERTEX_MEM(9); VERTEX_MEM(10); VERTEX_MEM(11); VERTEX_MEM(12); VERTEX_MEM(13); VERTEX_MEM(14); VERTEX_MEM(15)

#if _PARALLEL_COEF == 4
#define _REAL               _REAL_4
#define _IMAG               _IMAG_4
#define _RO                 _RO_4
#define _TETA               _TETA_4
#define _RO_MAX             _RO_MAX_4
#define _CHECK_RO           _CHECK_RO_4
#define _BLACK_RET          _BLACK_RET_4
#define _CRIT_ASSIGN        _CRIT_ASSIGN_4
#define _CHECK_CRIT_BREAK   _CHECK_CRIT_BREAK_4
#define _SET_COLOR          _SET_COLOR_4
#define _VERTEX             _VERTEX_4
#define _VERTEX_COLOR       _VERTEX_COLOR_4
#define _VERTEX_MEM         _VERTEX_MEM_4
#endif

#if _PARALLEL_COEF == 8
#define _REAL               _REAL_8
#define _IMAG               _IMAG_8
#define _RO                 _RO_8
#define _TETA               _TETA_8
#define _RO_MAX             _RO_MAX_8
#define _CHECK_RO           _CHECK_RO_8
#define _BLACK_RET          _BLACK_RET_8
#define _CRIT_ASSIGN        _CRIT_ASSIGN_8
#define _CHECK_CRIT_BREAK   _CHECK_CRIT_BREAK_8
#define _SET_COLOR          _SET_COLOR_8
#define _VERTEX             _VERTEX_8
#define _VERTEX_COLOR       _VERTEX_COLOR_8
#define _VERTEX_MEM         _VERTEX_MEM_8
#endif

#if _PARALLEL_COEF == 12
#define _REAL               _REAL_12
#define _IMAG               _IMAG_12
#define _RO                 _RO_12
#define _TETA               _TETA_12
#define _RO_MAX             _RO_MAX_12
#define _CHECK_RO           _CHECK_RO_12
#define _BLACK_RET          _BLACK_RET_12
#define _CRIT_ASSIGN        _CRIT_ASSIGN_12
#define _CHECK_CRIT_BREAK   _CHECK_CRIT_BREAK_12
#define _SET_COLOR          _SET_COLOR_12
#define _VERTEX             _VERTEX_12
#define _VERTEX_COLOR       _VERTEX_COLOR_12
#define _VERTEX_MEM         _VERTEX_MEM_12
#endif

#if _PARALLEL_COEF == 16
#define _REAL               _REAL_16
#define _IMAG               _IMAG_16
#define _RO                 _RO_16
#define _TETA               _TETA_16
#define _RO_MAX             _RO_MAX_16
#define _CHECK_RO           _CHECK_RO_16
#define _BLACK_RET          _BLACK_RET_16
#define _CRIT_ASSIGN        _CRIT_ASSIGN_16
#define _CHECK_CRIT_BREAK   _CHECK_CRIT_BREAK_16
#define _SET_COLOR          _SET_COLOR_16
#define _VERTEX             _VERTEX_16
#define _VERTEX_COLOR       _VERTEX_COLOR_16
#define _VERTEX_MEM         _VERTEX_MEM_16
#endif

#endif
