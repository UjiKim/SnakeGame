#ifndef __AI_H__
#define __AI_H__
class Population {
	const int populations = 2000;
	const double delay = 0.1;
	const bool viewAll = false;

	std::vector<Snake> players;
	Snake best;

	double fitnessSum = 0;
	double avgerage = 0;
	double bestFitness = 0;

	int gen = 0;
	int bestscore = 0;
	
	Random random;

	Texture white;
	Sprite background;
	Font font;
public:
	Population();

	bool done();
	int getGenerations();

	void update(double& timer);
	void draw(sf::RenderWindow& window);
	void setBest();

	Snake selectParent();
	Snake getBest();

	void naturalSelection();
	void mutate();
	void calculateFitness();
	void calculateFitnessSum();
	void calculateAverage();
};
#endif