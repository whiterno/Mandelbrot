#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <arm_neon.h>
#include <stdalign.h>
#include <assert.h>

#include "complex_nums.h"
#include "app.h"
#include "draw_mandelbrot.h"

void runApp(){
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Mandelbrot", sf::Style::Default);
    sf::Clock clock;

    sf::Vector2f center_pos({WINDOW_WIDTH / 3 * 2, WINDOW_HEIGHT / 2});
    float scale             = BASE_SCALE;
    int shift               = BASE_SHIFT;

    bool is_clicked             = false;
    sf::Vector2f clicked_pos    = {};
    sf::Vector2f released_pos   = {};

    sf::RectangleShape mouse_box(START_POINT);
    sf::Color mouse_box_color(GREY_RGB);
    mouse_box.setFillColor(mouse_box_color);

    sf::Font font("resources/Roboto_Condensed-Black.ttf");
    sf::Text text_fps(font);
    text_fps.setCharacterSize(24);
    text_fps.setFillColor(sf::Color::Green);

    while (window.isOpen()){
        // TODO: перенести расчеты в конец
        float currentTime   = clock.restart().asSeconds();
        int fps             = 1.f / currentTime;
        std::string fps_str = std::to_string(fps);

        text_fps.setString(fps_str);

        while (const std::optional event = window.pollEvent()){
            closeHandler(event, &window);
            mouseHandler(event, &center_pos, &clicked_pos, &released_pos, &is_clicked, &scale, &mouse_box);
            keyboardHandler(event, &window, &center_pos, &scale, shift);
        }
        if (is_clicked){
            auto [x, y] = sf::Mouse::getPosition(window);

            mouse_box.setSize({x - clicked_pos.x, y - clicked_pos.y});
        }

        window.clear(sf::Color::Black);

        sf::Vertex video_memory[WINDOW_SIZE];

        drawWithIntrinsics(video_memory, center_pos, scale);

        window.draw(video_memory, WINDOW_SIZE, sf::PrimitiveType::Points);
        window.draw(text_fps);
        window.draw(mouse_box);

        window.display();
    }
}

void keyboardHandler(const std::optional<sf::Event> event,
                     sf::RenderWindow*              window,
                     sf::Vector2f*                  center_pos,
                     float*                         scale,
                     int                            shift){
    assert(center_pos);
    assert(scale);

    if (const auto* key_pressed = event->getIf<sf::Event::KeyPressed>()){
        if (key_pressed->scancode == sf::Keyboard::Scancode::Escape){
            window->close();
        }
        if (key_pressed->scancode == sf::Keyboard::Scancode::Right){
            center_pos->x -= shift;
        }
        if (key_pressed->scancode == sf::Keyboard::Scancode::Up){
            center_pos->y += shift;
        }
        if (key_pressed->scancode == sf::Keyboard::Scancode::Left){
            center_pos->x += shift;
        }
        if (key_pressed->scancode == sf::Keyboard::Scancode::Down){
            center_pos->y -= shift;
        }
        if (key_pressed->scancode == sf::Keyboard::Scancode::I){
            //TODO: magic number
            *scale *= 2;
            center_pos->x = 2 * center_pos->x - WINDOW_WIDTH / 2;
            center_pos->y = 2 * center_pos->y - WINDOW_HEIGHT / 2;
        }
        if (key_pressed->scancode == sf::Keyboard::Scancode::D){
            *scale /= 2;
            center_pos->x = center_pos->x / 2 + WINDOW_WIDTH / 4;
            center_pos->y = center_pos->y / 2 + WINDOW_HEIGHT / 4;
        }
    }
}

void mouseHandler(const std::optional<sf::Event>    event,
                  sf::Vector2f*                     center_pos,
                  sf::Vector2f*                     clicked_pos,
                  sf::Vector2f*                     released_pos,
                  bool*                             is_clicked,
                  float*                            scale,
                  sf::RectangleShape*               mouse_box){
    assert(center_pos);
    assert(clicked_pos);
    assert(released_pos);
    assert(is_clicked);
    assert(scale);
    assert(mouse_box);

    if (const auto* pressed = event->getIf<sf::Event::MouseButtonPressed>()){
        clicked_pos->x = pressed->position.x;
        clicked_pos->y = pressed->position.y;
        *is_clicked = true;

        mouse_box->setPosition(*clicked_pos);
    }
    if (const auto* released = event->getIf<sf::Event::MouseButtonReleased>()){
        released_pos->x = released->position.x;
        released_pos->y = released->position.y;
        *is_clicked = false;

        float multiplier = 1000.f / (released_pos->x - clicked_pos->x);
        *scale *= multiplier;
        center_pos->x = multiplier * (center_pos->x - clicked_pos->x);
        center_pos->y = multiplier * (center_pos->y - clicked_pos->y);

        mouse_box->setPosition({0.f, 0.f});
        mouse_box->setSize({0.f, 0.f});
    }
}

void closeHandler(const std::optional<sf::Event>    event,
                  sf::RenderWindow*                 window){
    assert(window);

    if (event->is<sf::Event::Closed>()) window->close();
}
