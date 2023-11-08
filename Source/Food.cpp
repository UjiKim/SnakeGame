#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

#include "Food.h"

Food::Food(int x, int y) {
	this->x = x;
	this->y = y;
	if (!green.loadFromFile("image/green.png")) {
		exit(0);
	}
}

Food& Food::operator=(const Food& op) {
	this->x = op.x;
	this->y = op.y;
	food = op.food;
	return *this;
}

void Food::update(int x, int y) {
	this->x = x;
	this->y = y;
}

void Food::draw(sf::RenderWindow& window) {
	food.setTexture(green);
	food.setPosition(Vector2f(this->x * pixel_size, this->y * pixel_size));
	window.draw(food);
}

int Food::getX() {
	return x;
}

int Food::getY() {
	return y;
}