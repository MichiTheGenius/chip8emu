#include "Chip8.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <fstream>
#include <iostream>
using namespace std;

Chip8 chip8;
const int scale = 10;
const int screen_width = 64 * scale;
const int screen_height = 32 * scale;

void print_video_array()
{
  for (int i = 0; i < 100; i++)
  {
    chip8.emulate_cycle();
  }

  for (int i = 0; i < 64 * 32; i++)
  {
    if (i % 64 == 0 && i != 0)
    {
      printf("\n");
    }
    printf("%d", chip8.video[i]);
  }
}
int main()
{
  sf::RenderWindow window(sf::VideoMode(screen_width, screen_height), "chip8emu");
  window.setFramerateLimit(60);

  chip8.init();

  // create an array that stores all of the pixels of the chip8
  sf::RectangleShape pixels[64 * 32];
  for (int i = 0; i < 64 * 32; i++)
  {
    // initialize all of the pixels to zero
    pixels[i] = sf::RectangleShape(sf::Vector2f(0, 0));
    pixels[i].setPosition(0, 0);
  }

  // print_video_array();

  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
      {
        window.close();
      }
    }

    // update variables here
    chip8.emulate_cycle();
    if (chip8.draw_flag == 1)
    {
      for (int i = 0; i < 64 * 32; i++)
      {
        if (chip8.video[i] == 1)
        {
          int x_coordinate = i % 64;
          int y_coordinate = i / 64;
          sf::RectangleShape pixel(
              sf::Vector2f(scale, scale));
          pixel.setPosition(x_coordinate * scale,
                            y_coordinate * scale);
          pixels[i] = pixel;
        }
      }

      window.clear(sf::Color::Black);
      for (int i = 0; i < 64 * 32; i++)
      {
        window.draw(pixels[i]);
      }
      window.display();
    }
  }

  return 0;
}
