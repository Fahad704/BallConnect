#pragma once
#define KeyPressHandle(key,button)case key: {\
			input.buttons[button].changed = !input.buttons[button].is_down;\
			input.buttons[button].is_down = true;\
		}break;
#define KeyReleaseHandle(key,button)case key: {\
			input.buttons[button].changed = input.buttons[button].is_down;\
			input.buttons[button].is_down = false;\
		}break;
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include "platform_common.cpp"
sf::RenderWindow window(sf::VideoMode(1080,720), "Ball Connect!",sf::Style::Default);
#include "globals.cpp"
#include "game.cpp"
int main() {
	Input input = {};
	int width=1080, height=720;
	int xOffset=0, yOffset=0;
	while (window.isOpen()) {
		for (int i = 0; i < BUTTON_COUNT; i++) {
			input.buttons[i].changed = false;
		}
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type)
			{
			case sf::Event::Closed: {
				window.close();
			}break;
			case sf::Event::Resized: {
				xOffset = event.size.width - width;
				yOffset = event.size.width - height;
				sf::FloatRect visible_area(0, 0, event.size.width, event.size.height);
				height = window.getSize().y;
				width = window.getSize().x;
				window.setView(sf::View(visible_area));
			}break;
			case sf::Event::MouseButtonPressed: {
				switch (event.key.code)
				{
					KeyPressHandle(sf::Mouse::Left, LEFT_CLICK);
				default:
					break;
				}
			}break;
			case sf::Event::MouseButtonReleased: {
				switch (event.key.code) {
					KeyReleaseHandle(sf::Mouse::Left, LEFT_CLICK);
				}break;
			}break;
			case sf::Event::KeyPressed: {
				switch (event.key.code) {
					KeyPressHandle(sf::Keyboard::A, BUTTON_A);
					KeyPressHandle(sf::Keyboard::B, BUTTON_B);
				}
			}break;
			case sf::Event::KeyReleased: {
				switch (event.key.code) {
					KeyReleaseHandle(sf::Keyboard::A, BUTTON_A);
					KeyReleaseHandle(sf::Keyboard::B, BUTTON_B);
				}
			}break;
			default:
				break;
			}
		}
		window.clear(sf::Color(10,20,50));
		simulate_game(input);
		window.display();

	}
	return 0;
}