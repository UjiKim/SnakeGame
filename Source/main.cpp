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
#include "Population.h"

int main() {
	//Generate Initial Population
	Population population;

	//variables for SFML
	int highscore = 0;
	Text highscore_txt;
	Font font;
	if (!font.loadFromFile("fonts/IBMPlexMono.ttf")) {
		exit(0);
	}
	highscore_txt.setFont(font);
	highscore_txt.setFillColor(Color::White);
	highscore_txt.setCharacterSize(15);
	highscore_txt.setPosition(Vector2f(200, 420));

	//SFML Window Related
	RenderWindow window(VideoMode(850, 600), L"±è¿ìÁø [AI Snake]", Style::Titlebar | Style::Close);
	Image icon;
	if (!icon.loadFromFile("image/icon.png")) {
		exit(0);
	}
	window.setIcon(32, 32, icon.getPixelsPtr());

	//SFML for Animations
	Clock clock;
	double timer = 0, delay = 0.1;
	while (window.isOpen()) {
		double time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;
		Event game_event;
		while (window.pollEvent(game_event)) {
			switch (game_event.type) {
			case Event::Closed:
				window.close();
				break;
			}
		}
		if (population.done()) {
			highscore = population.getBest().getScore();
			highscore_txt.setString("Highscore : " + to_string(highscore));
			population.calculateFitness();
			population.naturalSelection();
			cout << "Replaying..." << endl;
		}
		else {
			population.update(timer);
			window.clear();
			population.draw(window);
			window.draw(highscore_txt);
			window.display();
		}
		if (population.getGenerations() >= 1000)
			window.close();
	}
}