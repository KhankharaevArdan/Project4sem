#include <Heroes.hpp>

Hero::Hero() {
    dir_ = Direction::RIGHT;
    x_ = 0;
    y_ = 0;
}

void Hero::Move() {
    switch (dir_)
    {
    case Direction::UP :
        ++y_;
        break;
    
    case Direction::RIGHT :
        ++x_;
        break;
    
    case Direction::LEFT :
        --x_;
        break;
    
    case Direction::DOWN :
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