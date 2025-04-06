#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <arm_neon.h>
#include <stdalign.h>
#include <assert.h>

#include "complex_nums.h"
#include "app.h"
#include "draw_mandelbrot.h"

void runApp(draw_t draw){
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Mandelbrot", sf::Style::Default);
    sf::Clock clock;

    ScaleView view = {.scale      = BASE_SCALE,
                      .shift      = BASE_SHIFT,
                      .center_pos = {WINDOW_WIDTH / 3 * 2, WINDOW_HEIGHT / 2}};

    MouseBox box = {.is_clicked   = false,
                    .clicked_pos  = {},
                    .released_pos = {}};

    sf::RectangleShape mouse_box(START_POINT);
    sf::Color mouse_box_color(GREY_RGB);
    mouse_box.setFillColor(mouse_box_color);

    sf::Font font("resources/Roboto_Condensed-Black.ttf");
    sf::Text text_fps(font);
    text_fps.setCharacterSize(24);
    text_fps.setFillColor(sf::Color::Green);

    sf::Vertex* video_memory = (sf::Vertex*)calloc(WINDOW_SIZE, sizeof(sf::Vertex));

    while (window.isOpen()){
        float currentTime   = clock.restart().asSeconds();

        while (const std::optional event = window.pollEvent()){
            closeHandler(event, &window);
            mouseHandler(event, &view, &box, &mouse_box);
            keyboardHandler(event, &window, &view);
        }
        if (box.is_clicked){
            auto [x, y] = sf::Mouse::getPosition(window);

            mouse_box.setSize({x - box.clicked_pos.x, y - box.clicked_pos.y});
        }

        window.clear(sf::Color::Black);

        draw(video_memory, &view);

        int fps             = 1.f / currentTime;
        std::string fps_str = std::to_string(fps);

        text_fps.setString(fps_str);

        window.draw(video_memory, WINDOW_SIZE, sf::PrimitiveType::Points);
        window.draw(text_fps);
        window.draw(mouse_box);

        window.display();
    }

    free(video_memory);
}

void keyboardHandler(const std::optional<sf::Event> event, sf::RenderWindow* window, ScaleView* view){
    assert(view);
    assert(window);

    if (const auto* key_pressed = event->getIf<sf::Event::KeyPressed>()){
        if (key_pressed->scancode == sf::Keyboard::Scancode::Escape){
            window->close();
        }
        if (key_pressed->scancode == sf::Keyboard::Scancode::Right){
            view->center_pos.x -= view->shift;
        }
        if (key_pressed->scancode == sf::Keyboard::Scancode::Up){
            view->center_pos.y += view->shift;
        }
        if (key_pressed->scancode == sf::Keyboard::Scancode::Left){
            view->center_pos.x += view->shift;
        }
        if (key_pressed->scancode == sf::Keyboard::Scancode::Down){
            view->center_pos.y -= view->shift;
        }
        if (key_pressed->scancode == sf::Keyboard::Scancode::I){
            view->scale *= SCALE_MULT;
            view->center_pos.x = SCALE_MULT * (view->center_pos.x - WINDOW_WIDTH / 4);
            view->center_pos.y = SCALE_MULT * (view->center_pos.y - WINDOW_HEIGHT / 4);
        }
        if (key_pressed->scancode == sf::Keyboard::Scancode::D){
            view->scale /= 2;
            view->center_pos.x = view->center_pos.x / SCALE_MULT + WINDOW_WIDTH / 4;
            view->center_pos.y = view->center_pos.y / SCALE_MULT + WINDOW_HEIGHT / 4;
        }
    }
}

void mouseHandler(const std::optional<sf::Event> event, ScaleView* view, MouseBox* box, sf::RectangleShape* mouse_box){
    assert(box);
    assert(view);
    assert(mouse_box);

    if (const auto* pressed = event->getIf<sf::Event::MouseButtonPressed>()){
        box->clicked_pos.x = pressed->position.x;
        box->clicked_pos.y = pressed->position.y;
        box->is_clicked = true;

        mouse_box->setPosition(box->clicked_pos);
    }
    if (const auto* released = event->getIf<sf::Event::MouseButtonReleased>()){
        box->released_pos.x = released->position.x;
        box->released_pos.y = released->position.y;
        box->is_clicked = false;

        float multiplier = WINDOW_WIDTH / (float)(box->released_pos.x - box->clicked_pos.x);
        view->scale *= multiplier;
        view->center_pos.x = multiplier * (view->center_pos.x - box->clicked_pos.x);
        view->center_pos.y = multiplier * (view->center_pos.y - box->clicked_pos.y);

        mouse_box->setPosition({0.f, 0.f});
        mouse_box->setSize({0.f, 0.f});
    }
}

void closeHandler(const std::optional<sf::Event> event, sf::RenderWindow* window){
    assert(window);

    if (event->is<sf::Event::Closed>()) window->close();
}
