#pragma once
#include <SFML/Graphics.hpp>
#include <utility>

enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Hero {
  public:
    Hero();
    void Move();
    void ChangeDirection(Direction dir);
    std::pair<int, int> GetCoord() const;
    Direction GetDirection() const;
    void SetPosition(std::pair<int, int> pos);
  private:

  Direction dir_;

  int x_;
  int y_;
};