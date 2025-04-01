#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>

#include "complex.h"

#define GL_SILENCE_DEPRECATION

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 800;
const int WINDOW_SIZE = WINDOW_WIDTH * WINDOW_HEIGHT;

const int ITERATIONS = 256;

sf::Vertex mandelbrotSetPoint(sf::Vector2i center_pos, float i, float j, double scale);
sf::Color setColor(int crit);

int main()
{
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 0;
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
                            "My window",
                            sf::Style::Default,
                            sf::State::Windowed,
                            settings);
    sf::Clock clock;

    sf::Vector2i center_pos({700, 400});
    sf::Vector2i window_pos = window.getPosition();
    float scale = 1;
    int shift = 100;

    bool is_clicked = false;
    sf::Vector2f clicked_pos = {};
    sf::Vector2f released_pos = {};

    sf::RectangleShape mouse_box({0, 0});
    sf::Color mouse_box_color(255, 255, 255, 120);
    mouse_box.setFillColor(mouse_box_color);

    while (window.isOpen())
    {
        float currentTime = clock.restart().asSeconds();
        int fps = 1.f / currentTime;
        std::string fps_str = std::to_string(fps);

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* pressed = event->getIf<sf::Event::MouseButtonPressed>()){
                clicked_pos.x = pressed->position.x;
                clicked_pos.y = pressed->position.y;
                is_clicked = true;

                mouse_box.setPosition(clicked_pos);
            }
            if (const auto* released = event->getIf<sf::Event::MouseButtonReleased>()){
                released_pos.x = released->position.x;
                released_pos.y = released->position.y;
                is_clicked = false;

                float multiplier = 1000.f / (released_pos.x - clicked_pos.x);
                scale *= multiplier;
                center_pos.x = multiplier * (center_pos.x - clicked_pos.x);
                center_pos.y = multiplier * (center_pos.y - clicked_pos.y);

                mouse_box.setPosition({0.f, 0.f});
                mouse_box.setSize({0.f, 0.f});
            }
            if (const auto* key_pressed = event->getIf<sf::Event::KeyPressed>()){
                if (key_pressed->scancode == sf::Keyboard::Scancode::Right){
                    center_pos.x -= shift;
                }
                if (key_pressed->scancode == sf::Keyboard::Scancode::Up){
                    center_pos.y += shift;
                }
                if (key_pressed->scancode == sf::Keyboard::Scancode::Left){
                    center_pos.x += shift;
                }
                if (key_pressed->scancode == sf::Keyboard::Scancode::Down){
                    center_pos.y -= shift;
                }
                if (key_pressed->scancode == sf::Keyboard::Scancode::I){
                    scale *= 2;
                    center_pos.x = 2 * center_pos.x - 500;
                    center_pos.y = 2 * center_pos.y - 400;
                }
                if (key_pressed->scancode == sf::Keyboard::Scancode::D){
                    scale /= 2;
                    center_pos.x = center_pos.x / 2 + 250;
                    center_pos.y = center_pos.y / 2 + 200;
                }
            }
        }

        if (is_clicked){
            auto [x, y] = sf::Mouse::getPosition(window);

            mouse_box.setSize({x - clicked_pos.x, y - clicked_pos.y});
        }

        window.clear(sf::Color::Black);

        sf::Vertex video_memory[WINDOW_SIZE];

        // for (int i = 0; i < WINDOW_WIDTH; i++){
        //     for (int j = 0; j < WINDOW_HEIGHT; j += 1){
        //         sf::Vertex vertex = mandelbrotSetPoint(center_pos, i, j, scale);

        //         video_memory[i * WINDOW_HEIGHT + j] = vertex;
        //     }
        // }

        for (int i = 0; i < WINDOW_WIDTH; i++){
            for (int j = 0; j < WINDOW_HEIGHT; j += 4){
                sf::Vertex vertex = mandelbrotSetPoint(center_pos, i, j, scale);

                video_memory[i * WINDOW_HEIGHT + j] = vertex;
            }
        }

        sf::Font font("resources/Roboto_Condensed-Black.ttf");
        sf::Text text(font);

        text.setString(fps_str);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::Green);

        window.draw(video_memory, WINDOW_SIZE, sf::PrimitiveType::Points);
        window.draw(text);
        window.draw(mouse_box);

        window.display();
    }
}

sf::Vertex mandelbrotSetPoint(sf::Vector2i center_pos, float i, float j, double scale){
    sf::Vertex vertex{{i, j}, sf::Color::White, {i, j}};

    Complex num = {.real = (i - center_pos.x) / (scale * 300), .imag = (j - center_pos.y) / (scale * 300)};

    double ro    = sqrt((num.real - 1.f/4) * (num.real - 1.f/4) + num.imag * num.imag);
    double teta  = atan2(num.imag, num.real - 1.f/4);
    double ro_c  = 1.f/2 -1.f/2 * cos(teta);

    int crit = 0;
    if (ro <= ro_c){
        vertex.color = sf::Color::Black;
    }
    else {
        Complex seq_mem = num;

        for (int k = 1; k < ITERATIONS; k++){
            seq_mem = addC(mulC(seq_mem, seq_mem), num);

            if (modC(seq_mem) >= 4){
                crit = k;
                break;
            };
        }
       if (modC(seq_mem) < 4) vertex.color = sf::Color::Black;
    }

    vertex.color = setColor(crit);

    return vertex;
}

sf::Color setColor(int crit){
    if (crit == 0) return sf::Color::Black;

    uint8_t r = (6 * crit) % 255;
    uint8_t g = (6 * crit) % 255;
    uint8_t b = (6 * crit) % 255;

    sf::Color color{r, g, b, 255};

    return color;
}
