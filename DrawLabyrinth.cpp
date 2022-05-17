#include <iostream>
#include <cstdlib>
#include <time.h>
#include <stack>
#include <vector>
#include "SFML/Graphics.hpp"
#include <SFML/Audio.hpp>

enum CellState 
{ 
    Close, 
    Open 
};

class Cell
{
    public:
    int x;
    int y;
    CellState Left;
    CellState Right;
    CellState Top;
    CellState Bottom;
    bool Visited;
};

const size_t boundary_width_y = 18;
const size_t boundary_width_x = 18;
const size_t cell_size = 70;
const size_t width = 38;
const size_t height = 15;


int main()
{
    Cell labyrinth[width][height];

    for(int y = 0; y < height; y++)
        for(int x = 0; x < width; x++)
        {
            labyrinth[x][y].x = x;
            labyrinth[x][y].y = y;
            labyrinth[x][y].Visited = false;
        }

    srand(time(NULL));
    int startX = 0;
    int startY = 0;

    labyrinth[startX][startY].Visited = true;

    std::stack<Cell> path;
    path.push(labyrinth[startX][startY]);

    while(!path.empty())
    {
        Cell _cell = path.top();

        std::vector<Cell> nextStep;
        if(_cell.x > 0 && (labyrinth[_cell.x - 1][_cell.y].Visited == false))
            nextStep.push_back(labyrinth[_cell.x - 1][_cell.y]);
        if(_cell.x < width - 1 && (labyrinth[_cell.x + 1][_cell.y].Visited == false))
            nextStep.push_back(labyrinth[_cell.x + 1][_cell.y]);
        if(_cell.y > 0 && (labyrinth[_cell.x][_cell.y - 1].Visited == false))
            nextStep.push_back(labyrinth[_cell.x][_cell.y - 1]);
        if(_cell.y < height - 1 && (labyrinth[_cell.x][_cell.y + 1].Visited == false))
            nextStep.push_back(labyrinth[_cell.x][_cell.y + 1]);

        if(!nextStep.empty())
        {
            Cell next = nextStep[rand() % nextStep.size()];

            if(next.x != _cell.x)
            {
                if(_cell.x - next.x > 0)
                {
                    labyrinth[_cell.x][_cell.y].Left = Open;
                    labyrinth[next.x][next.y].Right = Open;
                }
                else
                {
                    labyrinth[_cell.x][_cell.y].Right = Open;
                    labyrinth[next.x][next.y].Left = Open;
                }
            }
            if(next.y != _cell.y)
            {
                if(_cell.y - next.y > 0)
                {
                    labyrinth[_cell.x][_cell.y].Top = Open;
                    labyrinth[next.x][next.y].Bottom = Open;
                }
                else
                {
                    labyrinth[_cell.x][_cell.y].Bottom = Open;
                    labyrinth[next.x][next.y].Top = Open;
                }
            }

            labyrinth[next.x][next.y].Visited = true;
            path.push(next);

        }
        else
        {
            path.pop();
        }
    }

    sf::RenderWindow window(sf::VideoMode(1920, 1080), "labirinth");

    std::array<sf::RectangleShape, 2> boundary;
    boundary.at(0) = sf::RectangleShape(sf::Vector2f(window.getSize().x, boundary_width_y)); // horizontal
    boundary.at(1) = sf::RectangleShape(sf::Vector2f(boundary_width_x, window.getSize().y)); //  vertical
    boundary.at(0).setFillColor(sf::Color::White);
    boundary.at(1).setFillColor(sf::Color::White);

    std::array<sf::RectangleShape, 2> wall;
    wall.at(0) = sf::RectangleShape(sf::Vector2f(cell_size, 3));
    wall.at(1) = sf::RectangleShape(sf::Vector2f(3, cell_size));
    wall.at(0).setFillColor(sf::Color::White);
    wall.at(1).setFillColor(sf::Color::White);

    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        boundary.at(0).setPosition(0, 0);
        boundary.at(1).setPosition(0, 0);
        window.draw(boundary.at(0));
        window.draw(boundary.at(1));

        boundary.at(0).setPosition(0, window.getSize().y - boundary_width_y);
        boundary.at(1).setPosition(window.getSize().x - boundary_width_x, 0);
        window.draw(boundary.at(0));
        window.draw(boundary.at(1));

        for(int y = 0; y < height; y++)
        {
            for(int x = 0; x < width; x++)
            {
                if(labyrinth[x][y].Top = Close) 
                {
                    wall.at(0).setPosition(cell_size * x + boundary_width_x, cell_size * y + boundary_width_y);
                    window.draw(wall.at(0));
                };

                if(labyrinth[x][y].Left == Close)
                {
                    wall.at(1).setPosition(cell_size * x + boundary_width_x, cell_size * y + boundary_width_y);
                    window.draw(wall.at(1));
                }

                if(labyrinth[x][y].Right == Close)
                {
                    wall.at(1).setPosition(cell_size * x + cell_size + boundary_width_x, cell_size * y + boundary_width_y);
                    window.draw(wall.at(0));
                }

                if(labyrinth[x][y].Bottom == Close)
                {
                    wall.at(0).setPosition(cell_size * x + boundary_width_x, cell_size * y + cell_size + boundary_width_y);
                    window.draw(wall.at(1));
                }
            }
        }
        window.display();
    }

    return 0;
}



