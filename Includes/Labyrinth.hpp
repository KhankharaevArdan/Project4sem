#pragma once

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
    void MakeLabyrinth();
};

