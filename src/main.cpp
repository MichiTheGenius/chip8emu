#include "chip8.h"
#include <fstream>
#include <iostream>

#define COLOR_FG sf::Color::White
#define COLOR_BG sf::Color::Black

#define DELAY 2000
using namespace std;

Chip8 chip8;
const int scale = 10;
const int screen_width = 64 * scale;
const int screen_height = 32 * scale;

void print_video_array()
{
	for (int i = 0; i < 64 * 32; i++)
	{
		if (i % 64 == 0 && i != 0)
		{
			printf("\n");
		}
		printf("%d", chip8.video[i]);
	}
	printf("\n");
}

void run_test_cycles(int n)
{
	for (int i = 0; i < n; i++)
	{
		chip8.emulate_cycle();
	}
	print_video_array();
}

int main(int argc, char const *argv[])
{
	if (argc < 2)
	{
		printf("wrong usage! exiting...\n");
		printf("usage: ./emu.x <ROM>\n");
		return 1;
	}
	sf::RenderWindow window(sf::VideoMode(screen_width, screen_height), "chip8emu");
	// window.setFramerateLimit(60);

	char const *ROM = argv[1];
	chip8.init(ROM);
	// run_test_cycles(500);

	// create an array that stores all of the pixels of the chip8
	sf::RectangleShape pixels[64 * 32];

	for (int i = 0; i < 64 * 32; i++)
	{
		// initialize all of the pixels to zero
		pixels[i] = sf::RectangleShape(sf::Vector2f(scale, scale));
		pixels[i].setPosition(0, 0);
		pixels[i].setFillColor(COLOR_BG);
	}

	// for(int i = 0; i<2050; i++)
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
		chip8.get_keys();
		chip8.emulate_cycle();

		if (chip8.draw_flag == 1)
		{
			window.clear(COLOR_BG);
			for (int i = 0; i < 64 * 32; i++)
			{
				int x_coordinate = i % 64;
				int y_coordinate = i / 64;
				pixels[i].setPosition(x_coordinate * scale, y_coordinate * scale);
				if (chip8.video[i] == 1)
				{
					pixels[i].setFillColor(COLOR_FG);
				}
				else
				{
					pixels[i].setFillColor(COLOR_BG);
				}
				window.draw(pixels[i]);
			}
			window.display();
		}
		usleep(DELAY);
	}

	return 0;
}
