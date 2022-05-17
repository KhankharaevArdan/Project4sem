#include "Game.hpp"
#include "SFML/Graphics.hpp"
#include <iostream>
#include <array>

#include "Heroes.hpp"
#include "Labyrinth.hpp"

const int screen_resolution_x = 1920;
const int screen_resolution_y = 1080;

const size_t boundary_width_y = 18;
const size_t boundary_width_x = 18;
const size_t cell_size = 70;
const size_t width = 38;
const size_t height = 15;

PixelGameEngine::PixelGameEngine() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    window_ = new sf::RenderWindow (sf::VideoMode(screen_resolution_x, screen_resolution_y), "", 
                            sf::Style::Fullscreen, settings);

    window_->setVerticalSyncEnabled(true);

    if(!texture_tank_right_.loadFromFile("Images/tankRight.png")) {
        exit(-1);
    }
    texture_tank_right_.setSmooth(true);

    if(!texture_tank_left_.loadFromFile("Images/tankLeft.png")) {
        exit(-1);
    }
    texture_tank_left_.setSmooth(true);

    if(!texture_tank_up_.loadFromFile("Images/tankDown.png")) {
        exit(-1);
    }
    texture_tank_up_.setSmooth(true);

    if(!texture_tank_down_.loadFromFile("Images/tankUp.png")) {
        exit(-1);
    }
    texture_tank_down_.setSmooth(true);
}

void PixelGameEngine::StartGame(NetWorkClient& Client) {
    Hero tank_me{};
    Hero tank_friend{};
    bool update = true;
    Cell lab;

    while(window_->isOpen()) {
        
        sf::Event event;

        while (window_->pollEvent(event)) {
            
            switch (event.type) {
                case sf::Event::Closed :
                    window_->close();
                    break;

                case sf::Event::KeyPressed :
                    switch (event.key.code) {
                        case sf::Keyboard::Escape :
                            window_->close();
                            break;
                    
                        case sf::Keyboard::W :
                            tank_me.ChangeDirection(Direction::DOWN);
                            tank_me.Move();
                            update = true;
                            break;

                        case sf::Keyboard::D :
                            tank_me.ChangeDirection(Direction::RIGHT);
                            tank_me.Move();
                            update = true;
                            break;

                        case sf::Keyboard::S :
                            tank_me.ChangeDirection(Direction::UP);
                            tank_me.Move();
                            update = true;
                            break;
                        
                        case sf::Keyboard::A :
                            tank_me.ChangeDirection(Direction::LEFT);
                            tank_me.Move();
                            update = true;
                            break;

                        default:
                            break;
                    }


                default:  
                    break;
            }

        }

        //if(update) {
            Client.SendDataToOpponent(tank_me);
            //update = false;
        //}

        Client.ReceiveDataFromOpponent(tank_friend);

        DrawMap(lab.labyrinth);
        DrawHero(tank_me, true);
        DrawHero(tank_friend, false);
        window_->display();
        window_->clear();
    }

}

void PixelGameEngine::DrawMap(Cell** labyrinth) {
    std::array<sf::RectangleShape, 2> boundary;
    boundary.at(0) = sf::RectangleShape(sf::Vector2f(window_->getSize().x, boundary_width_y)); // horizontal
    boundary.at(1) = sf::RectangleShape(sf::Vector2f(boundary_width_x, window_->getSize().y)); //  vertical
    boundary.at(0).setFillColor(sf::Color::White);
    boundary.at(1).setFillColor(sf::Color::White);

    std::array<sf::RectangleShape, 2> wall;
    wall.at(0) = sf::RectangleShape(sf::Vector2f(cell_size, 3));
    wall.at(1) = sf::RectangleShape(sf::Vector2f(3, cell_size));
    wall.at(0).setFillColor(sf::Color::White);
    wall.at(1).setFillColor(sf::Color::White);

    boundary.at(0).setPosition(0, 0);
    boundary.at(1).setPosition(0, 0);
    window_->draw(boundary.at(0));
    window_->draw(boundary.at(1));

    boundary.at(0).setPosition(0, window_->getSize().y - boundary_width_y);
    boundary.at(1).setPosition(window_->getSize().x - boundary_width_x, 0);
    window_->draw(boundary.at(0));
    window_->draw(boundary.at(1));

    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            if(labyrinth[x][y].Top == Close) 
            {
                wall.at(0).setPosition(cell_size * x + boundary_width_x, cell_size * y + boundary_width_y);
                window_->draw(wall.at(0));
            };

            if(labyrinth[x][y].Left == Close)
            {
                wall.at(1).setPosition(cell_size * x + boundary_width_x, cell_size * y + boundary_width_y);
                window_->draw(wall.at(1));
            }

            if(labyrinth[x][y].Right == Close)
            {
                wall.at(1).setPosition(cell_size * x + cell_size + boundary_width_x, cell_size * y + boundary_width_y);
                window_->draw(wall.at(0));
            }

            if(labyrinth[x][y].Bottom == Close)
            {
                wall.at(0).setPosition(cell_size * x + boundary_width_x, cell_size * y + cell_size + boundary_width_y);
                window_->draw(wall.at(1));
            }
        }
    }
}

void PixelGameEngine::DrawHero(const Hero& tank, bool me) {
    sf::RectangleShape rectangle(sf::Vector2f(70, 70));

    switch (tank.GetDirection()) {
            case Direction::UP :
                rectangle.setTexture(&texture_tank_up_);
                break;

            case Direction::DOWN :
                rectangle.setTexture(&texture_tank_down_);
                break;

            case Direction::LEFT :
                rectangle.setTexture(&texture_tank_left_);
                break;

            case Direction::RIGHT :
                rectangle.setTexture(&texture_tank_right_);
                break;
            
            default:
                break;
    }
    auto coord = tank.GetCoord();
    rectangle.setPosition(sf::Vector2f(coord.first * 70, coord.second * 70));
    //if(!me) rectangle.setOutlineColor(sf::Color::Red);
    window_->draw(rectangle);
    
}

PixelGameEngine::~PixelGameEngine() {
    delete window_;
}