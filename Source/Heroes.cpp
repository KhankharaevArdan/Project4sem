#include <Heroes.hpp>

Hero::Hero() {
    dir_ = Direction::RIGHT;
    x_ = 1;
    y_ = 1;
}

void Hero::Move(Cell** Labyrinth) {
    
    //std::cout << coord.first << " " << coord.second << std::endl;
    switch (dir_) {
    case Direction::UP :
        //if(Labyrinth[coord.first][coord.second].Bottom == Open) 
            ++y_;
        break;
    case Direction::RIGHT :
        //if(Labyrinth[coord.first][coord.second].Right == Open) 
            ++x_;
        break;
    
    case Direction::LEFT :
        //if(Labyrinth[coord.first][coord.second].Left == Open)
            --x_;
        break;
    
    case Direction::DOWN :
        //if(Labyrinth[coord.first][coord.second].Bottom == Open)
            --y_;
        break;
        
    default:
        break;
    }
}

std::pair<int, int> Hero::GetCoord() const {
    return std::pair<int, int>(x_, y_);
}

void Hero::ChangeDirection(Direction dir) {
    dir_ = dir;
}

void Hero::SetPosition(std::pair<int, int> pos) {
    x_ = pos.first;
    y_ = pos.second;
}

Direction Hero::GetDirection() const {
    return dir_;
}