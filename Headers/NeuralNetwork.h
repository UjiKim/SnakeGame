#ifndef __NEURALNETWORK_H__
#define __NEURALNETWORK_H__
class NeuralNetwork {
	int input_nodes, hidden_nodes, output_nodes, hidden_layers;
	std::vector<Matrix> weights;
public:
	NeuralNetwork(int input = 28, int hidden = 16, int output = 4, int hiddenlayers = 2);
	NeuralNetwork& operator=(const NeuralNetwork& op);

	NeuralNetwork crossover(NeuralNetwork partner);

	std::vector<double> output(std::vector<double> input);

	void mutate(double mutation_rate);
	void draw(sf::RenderWindow& window, int x, int y, double width, double height, std::vector<double> sensor, std::vector<double> decisions);
	void display();
};
#endif