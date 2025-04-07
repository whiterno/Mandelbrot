#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#define START_POINT     {0, 0}
#define GREY_RGB        255, 255, 255, 120

#ifndef DOT_TYPE
#define DOT_TYPE float
#endif

const int WINDOW_WIDTH  = 1200;
const int WINDOW_HEIGHT = 816;
const int WINDOW_SIZE   = WINDOW_WIDTH * WINDOW_HEIGHT;
const int BASE_CENTER_X = WINDOW_WIDTH / 3 * 2;
const int BASE_CENTER_Y = WINDOW_HEIGHT / 2;

const float BASE_SCALE   = 1;
const int BASE_SHIFT     = 100;
const int DOTS_PER_PIXEL = 300;
const float SCALE_MULT   = 2;

const int ITERATIONS = 1024;

typedef struct Vector{
    DOT_TYPE x;
    DOT_TYPE y;
} Vector;

typedef struct ScaleView{
    float scale;
    int shift;
    Vector center_pos;
} ScaleView;

typedef struct MouseBox{
    bool is_clicked;
    sf::Vector2f clicked_pos;
    sf::Vector2f released_pos;
} MouseBox;

typedef void (*draw_t)(sf::Vertex*, ScaleView*);

void keyboardHandler(const std::optional<sf::Event> event, sf::RenderWindow* window, ScaleView* view);

void mouseHandler(const std::optional<sf::Event> event, ScaleView* view, MouseBox* box, sf::RectangleShape* mouse_box);

void closeHandler(const std::optional<sf::Event> event, sf::RenderWindow* window);

void runApp(draw_t draw);

#endif
