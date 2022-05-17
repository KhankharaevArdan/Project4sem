#pragma once
#include "SFML/Graphics.hpp"
#include "Heroes.hpp"
#include "Labyrinth.hpp"
#include "NetworkClient.hpp"
#include "NetworkServer.hpp"

class PixelGameEngine {
   public:
      PixelGameEngine();
      void StartGame(NetWorkClient& Client);
      ~PixelGameEngine();
   private:
      void DrawMap(Cell** labyrinth);
      void DrawHero(const Hero& tank, bool me);

      sf::Texture texture_tank_right_;
      sf::Texture texture_tank_up_;
      sf::Texture texture_tank_down_;
      sf::Texture texture_tank_left_;

    sf::RenderWindow* window_;

};


