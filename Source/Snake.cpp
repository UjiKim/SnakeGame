#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

#include "Random.h"
#include "Matrix.h"
#include "NeuralNetwork.h"
#include "Food.h"
#include "Snake.h"

Snake::Snake() {
	for (int i = 0; i < length; i++) {
		this->x.push_back(max_x / 2);
		this->y.push_back(max_y / 2 - i);
	}
	if (!blue.loadFromFile("image/blue.png")) {
		exit(0);
	}
	if (!red.loadFromFile("image/red.png")) {
		exit(0);
	}
	if (!gray.loadFromFile("image/gray.png")) {
		exit(0);
	}
	sensor.resize(28);
	decisions.resize(4);
	food.update(random.getRandom(0,max_x - 1), random.getRandom(0,max_y - 1));
	foodList.push_back(food);
}

Snake::Snake(vector<Food> foods) {
	if (!blue.loadFromFile("image/blue.png")) {
		exit(0);
	}
	if (!red.loadFromFile("image/red.png")) {
		exit(0);
	}
	if (!gray.loadFromFile("image/gray.png")) {
		exit(0);
	}
	sensor.resize(28);
	decisions.resize(4);
	foodList = foods;
	for (int i = 0; i < length; i++) {
		this->x.push_back(max_x / 2);
		this->y.push_back(max_y / 2 - i);
	}
	food = foodList[0];
}

void Snake::copyForReplay(const Snake op){
	replay = true;
	dead = false;

	brain = op.brain;

	length = 1;
	life = 200;
	total = 0;
	delx = 0;
	dely = 1;

	foodIndex = 0;
	fitness = 0;
	score = 0;

	foodList.clear();
	x.clear();
	y.clear();
	for (int i = 0; i < length; i++) {
		this->x.push_back(max_x / 2);
		this->y.push_back(max_y / 2 - i);
	}
	foodList = op.foodList;
	food = foodList[foodIndex++];
}

Snake& Snake::operator=(const Snake& op) {
	//reset();
	dead = op.dead;
	replay = op.replay;

	food = op.food;
	brain = op.brain;
	random = op.random;

	score = op.score;
	length = op.length;
	life = op.life;
	total = op.total;
	delx = op.delx;
	dely = op.dely;

	foodIndex = op.foodIndex;

	sensor = op.sensor;
	decisions = op.decisions;

	x = op.x;
	y = op.y;

	foodList = op.foodList;

	return *this;
}

Snake Snake::crossover(Snake partner) {
	Snake child;
	child.brain = brain.crossover(partner.brain);
	return child;
}

NeuralNetwork Snake::getBrain() {
	return brain;
}

vector<double> Snake::search_at_Direction(int delta_x, int delta_y) {
	vector<double> sensor(3,0);

	int head_x = x[0];
	int head_y = y[0];
	double distance = 0;
	bool bodyFound = false;
	bool foodFound = false;
	head_x += delta_x;
	head_y += delta_y;
	distance++;
	while (!wallCollide(head_x, head_y)) {
		if (!foodFound && foodCollide(head_x, head_y)) {
			foodFound = true;
			sensor[0] = 1;
		}
		if (!bodyFound && bodyCollide(head_x, head_y)) {
			bodyFound = true;
			sensor[1] = 1;
		}
		head_x += delta_x;
		head_y += delta_y;
		distance++;
	}
	sensor[2] = 1 / distance;
	return sensor;
}

vector<double> Snake::getSensor() {
	return sensor;
}

vector<double> Snake::getOutput() {
	return decisions;
}

vector<Food> Snake::getFoodList() {
	return foodList;
}

bool Snake::bodyCollide(int x, int y) {
	for (int i = 1; i < length; i++) {
		if (x == this->x[i] && y == this->y[i]) {
			return true;
		}
	}
	return false;
}

bool Snake::foodCollide(int x, int y) {
	if (x == food.getX() && y == food.getY()) {
		return true;
	}
	return false;
}

bool Snake::wallCollide(int x, int y) {
	if (x >= max_x || x < 0 || y < 0 || y >= max_y) {
		return true;
	}
	return false;
}

bool Snake::isDead() {
	return dead;
}

bool Snake::isReplay() {
	return replay;
}

int Snake::getScore() {
	return score;
}

int Snake::getLife() {
	return life;
}

double Snake::getFitness(){
	return fitness;
}

void Snake::draw(sf::RenderWindow& window) {
	if (!isDead()) {
		head.setTexture(blue);
		body.setTexture(red);
	}
	else {
		head.setTexture(gray);
		body.setTexture(gray);
	}
	food.draw(window);
	head.setPosition(Vector2f(x[0] * pixel_size, y[0] * pixel_size));
	window.draw(head);
	for (int i = 1; i < x.size();i++) {
		body.setPosition(Vector2f(x[i] * pixel_size, y[i] * pixel_size));
		window.draw(body);
	}
}

void Snake::update() {
	life--;
	total++;
	if (foodCollide(x[0], y[0])) {
		eat();
	}
	move();
	if (wallCollide(x[0], y[0])) {
		dead = true;
	}
	else if (bodyCollide(x[0], y[0])) {
		dead = true;
	}
	else if (life <= 0) {
		dead = true;
	}
}

void Snake::eat() {
	length++;
	score++;
	if (life < 500) {
		if (life > 400)
			life = 500;
		else
			life += 100;
	}
	x.push_back(x[x.size() - 1]);
	y.push_back(y[y.size() - 1]);
	if (!replay) {
		food.update(random.getRandom(0, max_x - 1), random.getRandom(0, max_y - 1));
		while (bodyCollide(food.getX(), food.getY())) {
			food.update(random.getRandom(0, max_x - 1), random.getRandom(0, max_y - 1));
		}
		foodList.push_back(food);
	}
	else {
		food = foodList[foodIndex++];
	}
}

void Snake::move() {
	int temp2x, temp2y;
	int tempx = x[0];
	int tempy = y[0];
	x[0] += delx;
	y[0] += dely;
	for (int i = 1; i < length; i++) {
		temp2x = x[i];
		temp2y = y[i];
		x[i] = tempx;
		y[i] = tempy;
		tempx = temp2x;
		tempy = temp2y;
	}
}

void Snake::mutate(){
	brain.mutate(mutaion_rate);
}

void Snake::calculateFitness(){
	fitness = life + pow(2, score) + pow(score, 2.1) * 500 - pow(score, 1.2) * pow(0.25 * life, 1.3);
	//fitness = life + pow(2, score);
}

void Snake::search(){
	vector<double> temp(3,0);
	temp = search_at_Direction(-1,-1);	//NW direction
	sensor[0] = temp[0];
	sensor[1] = temp[1];
	sensor[2] = temp[2];
	temp = search_at_Direction(0, -1);	//N direction
	sensor[3] = temp[0];
	sensor[4] = temp[1];
	sensor[5] = temp[2];
	temp = search_at_Direction(1, -1);	//NE direction
	sensor[6] = temp[0];
	sensor[7] = temp[1];
	sensor[8] = temp[2];
	temp = search_at_Direction(1, 0);	//E direction
	sensor[9] = temp[0];
	sensor[10] = temp[1];
	sensor[11] = temp[2];
	temp = search_at_Direction(1, 1);	//SE direction
	sensor[12] = temp[0];
	sensor[13] = temp[1];
	sensor[14] = temp[2];
	temp = search_at_Direction(0, 1);	//S direction
	sensor[15] = temp[0];
	sensor[16] = temp[1];
	sensor[17] = temp[2];
	temp = search_at_Direction(-1, 1);	//SW direction
	sensor[18] = temp[0];
	sensor[19] = temp[1];
	sensor[20] = temp[2];
	temp = search_at_Direction(-1, 0);	//W direction
	sensor[21] = temp[0];
	sensor[22] = temp[1];
	sensor[23] = temp[2];

	if (delx == 0) {
		if (dely == -1) {
			sensor[24] = 1;
			sensor[25] = 0;
			sensor[26] = 0;
			sensor[27] = 0;
		}
		else {
			sensor[24] = 0;
			sensor[25] = 1;
			sensor[26] = 0;
			sensor[27] = 0;
		}
	}
	if (dely == 0) {
		if (delx == -1) {
			sensor[24] = 0;
			sensor[25] = 0;
			sensor[26] = 1;
			sensor[27] = 0;
		}
		else {
			sensor[24] = 0;
			sensor[25] = 0;
			sensor[26] = 0;
			sensor[27] = 1;
		}
	}
}

void Snake::think(){
	decisions = brain.output(sensor);

	int maxIndex = 0;
	double max = 0;
	for (int i = 0; i < decisions.size(); i++) {
		if (decisions[i] > max) {
			max = decisions[i];
			maxIndex = i;
		}
	}

	switch (maxIndex) {
	case 0:	//move up
		if (dely != 1) {
			delx = 0;
			dely = -1;
		}
		break;
	case 1:	//move down
		if (dely != -1) {
			delx = 0;
			dely = 1;
		}
		break;
	case 2:	//move left
		if (delx != 1) {
			delx = -1;
			dely = 0;
		}
		break;
	case 3:	//move right
		if (delx!= -1) {
			delx = 1;
			dely = 0;
		}
		break;
	}
}

void Snake::reset(){
	dead = false;
	length = 3;
	life = 200;
	total = 0;
	delx = 0;
	dely = 1;
	foodIndex = 0;
	fitness = 0;
	score = 0;

	x.clear();
	y.clear();
	for (int i = 0; i < length; i++) {
		this->x.push_back(max_x / 2);
		this->y.push_back(max_y / 2 - i);
	}
	sensor.clear();

	decisions.clear();
	sensor.resize(28);
	decisions.resize(4);

	foodList.clear();
	food.update(random.getRandom(0, max_x - 1), random.getRandom(0, max_y - 1));
	foodList.push_back(food);
}
