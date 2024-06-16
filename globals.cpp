#pragma once
#include "main.cpp"
enum GridData{
	EMPTY,
	RED,
	BLUE,
	PINK,
	GREEN
};
struct Square {
	int index;
	bool isFilled;
	GridData color;
	bool isDrawn;
};
struct Link {
	Square first;
	Square second;
};
struct LinkThread {
	std::vector<Link> links;
	GridData color;
};
struct Ball {
	sf::CircleShape circle;
	int x;
	int y;
	int index;
	bool isConnected;
};
struct Level {
	int grid;
	std::vector<std::vector<GridData>> level_map;
	std::vector<Ball> circles;
};