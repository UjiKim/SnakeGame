#include <iostream>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

#include "Random.h"
#include "Matrix.h"
#include "NeuralNetwork.h"
#include "Food.h"
#include "Snake.h"
#include "Population.h"

Population::Population(){
	//Random Populations
	cout << "Generating Initial Population...";
	Snake first;
	for (int i = 0; i < populations; i++) {
		Snake tmp(first.getFoodList());
		players.push_back(tmp);
	}
	best = players[0];
	best.replay = true;
	cout << " Done!" << endl;

	//SFML
	if (!font.loadFromFile("fonts/IBMPlexMono.ttf"))
		exit(0);
	if (!white.loadFromFile("image/white.png"))
		exit(0);
	background.setTexture(white);
}

bool Population::done(){
	if (!best.isDead()) {
		return false;
	}
	for (int i = 0; i < players.size(); i++) {
		if (!players[i].isDead()) {
			return false;
		}
	}
	cout << "Generation " << gen << " is finished!" << endl;
	cout << "Start Calculatings..." << endl;
	return true;
}

int Population::getGenerations() {
	return gen;
}

void Population::update(double& timer){
	if (!best.isDead()) {
		if (timer > delay) {
			timer = 0;
			best.search();
			best.think();
			best.update();
		}
		if (best.isDead()) {
			system("cls");
			cout << "Replay Finished!" << endl;
			cout << "Updating Remaining Snakes..." << endl;
		}
	}
	else {
		if (!viewAll) {
			for (int i = 0; i < players.size(); i++) {
				if (!players[i].isDead()) {
					players[i].search();
					players[i].think();
					players[i].update();
				}
			}
		}
		else {
			if (timer > delay) {
				timer = 0;
				for (int i = 0; i < players.size(); i++) {
					if (!players[i].isDead()) {
						players[i].search();
						players[i].think();
						players[i].update();
					}
				}
			}
		}
	}
}

void Population::draw(sf::RenderWindow& window){
	Text bestscore_txt, gen_txt, bestFitness_txt, life_left, average;
	bestscore_txt.setFont(font);
	bestscore_txt.setCharacterSize(15);
	bestscore_txt.setFillColor(Color::White);
	bestscore_txt.setPosition(Vector2f(0, 420));
	bestscore_txt.setString("Score : " + to_string(best.getScore()));
	gen_txt.setFont(font);
	gen_txt.setCharacterSize(15);
	gen_txt.setFillColor(Color::White);
	gen_txt.setPosition(Vector2f(0, 400));
	gen_txt.setString("Generation : " + to_string(gen));
	bestFitness_txt.setFont(font);
	bestFitness_txt.setCharacterSize(15);
	bestFitness_txt.setFillColor(Color::White);
	bestFitness_txt.setPosition(Vector2f(0, 440));
	bestFitness_txt.setString("Best Fitness : " + to_string(bestFitness));
	life_left.setFont(font);
	life_left.setCharacterSize(15);
	life_left.setFillColor(Color::White);
	life_left.setPosition(Vector2f(200,400));
	life_left.setString("Life left : " + to_string(best.getLife()));
	average.setFont(font);
	average.setCharacterSize(15);
	average.setFillColor(Color::White);
	average.setPosition(Vector2f(0, 460));
	average.setString("Avg. Fitness : " + to_string(this->avgerage));

	window.draw(bestscore_txt);
	window.draw(gen_txt);
	window.draw(bestFitness_txt);
	window.draw(life_left);
	window.draw(average);
	for (int i = 0; i < 25; i++) {
		for (int j = 0; j < 25; j++) {
			background.setPosition(Vector2f(i * 16, j * 16));
			window.draw(background);
		}
	}
	if (!viewAll) {
		best.draw(window);
		best.getBrain().draw(window,425,0,400,580,best.getSensor(), best.getOutput());
	}
	else {
		for (int i = 0; i < players.size(); i++) {
			players[i].draw(window);
		}
	}
}

void Population::setBest(){
	cout << "Setting Best Snakes...";
	double max = 0;
	int max_index = 0;
	for (int i = 0; i < players.size(); i++) {
		if (players[i].getFitness() > max) {
			max = players[i].getFitness();
			max_index = i;
		}
	}
	if (max > bestFitness) {
		bestFitness = max;
		best.copyForReplay(players[max_index]);
		bestscore = players[max_index].getScore();
		cout << "Best Changed!" << endl;;
	}
	else {
		best.copyForReplay(best);
		cout << "Best Didn't Changed..." << endl;
	}
}

Snake Population::selectParent(){
	double rand = random.getRandom(0.0, fitnessSum);
	double cumulative = 0;
	for (int i = 0; i < players.size(); i++) {
		cumulative += players[i].getFitness();
		if (cumulative > rand) {
			return players[i];
		}
	}
	return players[0];
}

Snake Population::getBest(){
	return best;
}

void Population::naturalSelection(){
	vector<Snake> nextPlayers;

	setBest();
	calculateFitnessSum();
	calculateAverage();

	//nextPlayers.push_back(best);	//the first player is the best player of previous generation
	cout << "Crossovering & Mutating Child...";
	for (int i = 0; i < players.size(); i++) {
		Snake child = selectParent().crossover(selectParent());
		child.mutate();
		nextPlayers.push_back(child);
	}
	players = nextPlayers;
	cout << "Done!" << endl;
	gen++;
}

void Population::mutate() {
	for (int i = 1; i < players.size(); i++) {
		players[i].mutate();
	}
}

void Population::calculateFitness() {
	cout << "Calculating Fitness of Each Snakes...";
	for (int i = 0; i < players.size(); i++) {
		players[i].calculateFitness();
	}
	cout << "Done!" << endl;
}

void Population::calculateFitnessSum() {
	cout << "Calculating FitnessSum...";
	fitnessSum = 0;
	for (int i = 0; i < players.size(); i++) {
		fitnessSum += players[i].getFitness();
	}
	cout << "Done!" << endl;
}

void Population::calculateAverage() {
	avgerage = fitnessSum / populations;
}