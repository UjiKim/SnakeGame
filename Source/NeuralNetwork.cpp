#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

#include "Matrix.h"
#include "NeuralNetwork.h"

NeuralNetwork::NeuralNetwork(int input, int hidden, int output, int hiddenlayers) {
	this->input_nodes = input;
	this->hidden_nodes = hidden;
	this->output_nodes = output;
	this->hidden_layers = hiddenlayers;

	Matrix itoh(hidden_nodes, input_nodes + 1);	// input -> hidden
	weights.resize(hidden_layers + 1);
	weights[0] = itoh;

	for (int i = 1; i < hidden_layers; i++) {
		Matrix htoh(hidden_nodes,hidden_nodes+1);	//hidden -> hidden
		weights[i] = htoh;
	}

	Matrix htoo(output_nodes,hidden_nodes+1);	//hidden -> output
	weights[weights.size() - 1] = htoo;

	for (Matrix& w : weights) {
		w.randomize(-1.0,1.0);
	}
}

NeuralNetwork& NeuralNetwork::operator=(const NeuralNetwork& op) {
	this->input_nodes = op.input_nodes;
	this->hidden_nodes = op.hidden_nodes;
	this->output_nodes = op.output_nodes;
	this->hidden_layers = op.hidden_layers;

	this->weights = op.weights;

	return *this;
}

NeuralNetwork NeuralNetwork::crossover(NeuralNetwork partner){
	NeuralNetwork child(input_nodes, hidden_nodes, output_nodes, hidden_layers);
	for (unsigned int i = 0; i < weights.size(); i++) {
		child.weights[i] = weights[i].crossover(partner.weights[i]);
	}
	return child;
}

vector<double> NeuralNetwork::output(std::vector<double> input) {
	Matrix inputs = weights[0].singleColumnMatrixFromVector(input);
	Matrix curr_bias = inputs.addBias();

	for (int i = 0; i < hidden_layers; i++) {
		Matrix hidden_input = weights[i].dot(curr_bias);
		Matrix hidden_output = hidden_input.activation();
		curr_bias = hidden_output.addBias();
	}

	Matrix output_input = weights[weights.size() - 1].dot(curr_bias);
	Matrix output = output_input.activation();

	return output.softmax(output.toVector());
}

void NeuralNetwork::mutate(double mutation_rate) {
	for (Matrix& w : weights) {
		w.mutate(mutation_rate);
	}
}

void NeuralNetwork::draw(sf::RenderWindow& window, int x, int y, double width, double height, std::vector<double> sensor, std::vector<double> decisions){
	vector<Vector2f> start, finish;	//positions

	CircleShape neuron;
	Color gray(128,128,128);
	VertexArray line(Quads, 4);
	string contents;

	Text text;
	Font font;
	if (!font.loadFromFile("fonts/IBMPlexMono.ttf")) {
		exit(0);
	}

	double linethickness = 1;
	double space = 5;
	double neuron_size = (height - space * (input_nodes - 2)) / input_nodes;
	double radius = neuron_size / 2;
	double neuron_space = (width - weights.size() * neuron_size) / weights.size();
	double hBuff = (height - space * (hidden_nodes - 1) - neuron_size * hidden_nodes) / 2;
	double oBuff = (height - space * (output_nodes - 1) - neuron_size * output_nodes) / 2;

	int max_index = 0;
	for (int i = 1; i < decisions.size(); i++) {
		if (decisions[i] > decisions[max_index])
			max_index = i;
	}

	text.setFont(font);
	text.setFillColor(Color::White);
	text.setCharacterSize(neuron_size);
	switch (max_index) {
	case 0:
		text.setString("U");
		break;
	case 1:
		text.setString("D");
		break;
	case 2:
		text.setString("L");
		break;
	case 3:
		text.setString("R");
		break;
	}

	neuron.setRadius(radius);
	neuron.setOutlineThickness(-linethickness);
	neuron.setOutlineColor(Color::White);
	//Draw Nodes
	for (int i = 0; i < input_nodes; i++) {
		if (sensor[i] != 0)
			neuron.setFillColor(Color::Green);
		else
			neuron.setFillColor(gray);
		neuron.setPosition(Vector2f(x, y + (i * (neuron_size + space))));
		start.push_back(neuron.getPosition());
		window.draw(neuron);
	}
	//Draw hidden nodes & line
	for (int layer = 0; layer < hidden_layers; layer++) {
		for (int i = 0; i < hidden_nodes; i++) {
			neuron.setFillColor(gray);
			neuron.setPosition(Vector2f(x + (1+layer)*neuron_size + (1+layer)*neuron_space, y+hBuff+(i*(neuron_size + space))));
			finish.push_back(neuron.getPosition());
			window.draw(neuron);
			for (int j = 0; j < start.size(); j++) {
				line[0].position = Vector2f(start[j].x + neuron_size, start[j].y + radius - 0.5 * linethickness);
				line[3].position = Vector2f(start[j].x + neuron_size, start[j].y + radius + 0.5 * linethickness);
				for (int k = 0; k < finish.size(); k++) {
					line[1].position = Vector2f(finish[k].x, finish[k].y + radius - 0.5 * linethickness);
					line[2].position = Vector2f(finish[k].x, finish[k].y + radius + 0.5 * linethickness);
					if (weights[layer].getContent(k, j) < 0) {
						line[0].color = sf::Color::Red;
						line[1].color = sf::Color::Red;
						line[2].color = sf::Color::Red;
						line[3].color = sf::Color::Red;
					}
					else {
						line[0].color = sf::Color::Blue;
						line[1].color = sf::Color::Blue;
						line[2].color = sf::Color::Blue;
						line[3].color = sf::Color::Blue;
					}
					window.draw(line);
				}
			}
		}
		start.clear();
		start = finish;
		finish.clear();
	}
	//output
	for (int i = 0; i < output_nodes; i++) {
		if (i == max_index) {
			neuron.setFillColor(Color::Green); 
			neuron.setPosition(Vector2f(x + (3 * neuron_space) + (3 * neuron_size), y + oBuff + i * (neuron_size + space)));
			text.setPosition(neuron.getPosition().x + neuron_size, neuron.getPosition().y);
			window.draw(text);
		}
		else {
			neuron.setFillColor(gray);
			neuron.setPosition(Vector2f(x + (3 * neuron_space) + (3 * neuron_size), y + oBuff + i * (neuron_size + space)));
		}
		finish.push_back(neuron.getPosition());
		window.draw(neuron);
		for (int j = 0; j < start.size(); j++) {
			line[0].position = sf::Vector2f(start[j].x + 2 * radius, start[j].y + radius - 0.5 * linethickness);
			line[3].position = sf::Vector2f(start[j].x + 2 * radius, start[j].y + radius + 0.5 * linethickness);
			for (unsigned int k = 0; k < finish.size(); k++) {
				line[1].position = sf::Vector2f(finish[k].x, finish[k].y + radius - 0.5 * linethickness);
				line[2].position = sf::Vector2f(finish[k].x, finish[k].y + radius + 0.5 * linethickness);
				if (weights[weights.size() - 1].getContent(k, j) < 0) {
					line[0].color = sf::Color::Red;
					line[1].color = sf::Color::Red;
					line[2].color = sf::Color::Red;
					line[3].color = sf::Color::Red;
				}
				else {
					line[0].color = sf::Color::Blue;
					line[1].color = sf::Color::Blue;
					line[2].color = sf::Color::Blue;
					line[3].color = sf::Color::Blue;
				}
				window.draw(line);
			}
		}
	}
}

void NeuralNetwork::display(){
	for (int num = 0; num < weights.size(); num++) {
		weights[num].draw();
	}
}
