#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#define START_POINT     {0, 0}
#define GREY_RGB        255, 255, 255, 120

const int WINDOW_WIDTH  = 1200;
const int WINDOW_HEIGHT = 816;
const int WINDOW_SIZE   = WINDOW_WIDTH * WINDOW_HEIGHT;

const float BASE_SCALE  = 1;
const int BASE_SHIFT    = 100;

const int ITERATIONS = 1024;

void keyboardHandler(const std::optional<sf::Event> event,
                     sf::RenderWindow*              window,
                     sf::Vector2f*                  center_pos,
                     float*                         scale,
                     int                            shift);

void mouseHandler(const std::optional<sf::Event>    event,
                  sf::Vector2f*                     center_pos,
                  sf::Vector2f*                     clicked_pos,
                  sf::Vector2f*                     released_pos,
                  bool*                             is_clicked,
                  float*                            scale,
                  sf::RectangleShape*               mouse_box);

void closeHandler(const std::optional<sf::Event>    event,
                  sf::RenderWindow*                 window);

void runApp();

#endif
