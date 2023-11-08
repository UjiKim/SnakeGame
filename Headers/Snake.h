#ifndef __SNAKE_H__
#define __SNAKE_H__
class Snake {
	Food food;
	NeuralNetwork brain;
	Random random;

	int score = 0;
	int length = 1;	//total snake length (includes head)
	int life = 200;	//moves left
	int total = 0;	//total moves
	int delx = 0, dely = 1;	//moves per cycle;
	int foodIndex = 0;	//index for replay

	const int max_x = 25, max_y = 25;	//max value of x, y axis
	const double mutaion_rate = 0.1;
	const int pixel_size = 16;

	double fitness = 0;

	std::vector<double> sensor;	//snake sensors : total 8 directions
	std::vector<double> decisions;	//snake decisions : where to move

	std::vector<int> x, y;	//snake positions: 0 : head, else : body
	std::vector<Food> foodList;	//food positions list
	//SFML
	sf::Texture blue, red, gray;
	sf::Sprite head, body;
public:
	bool replay = false;
	bool dead = false;

	Snake();
	Snake(std::vector<Food> foods);

	void copyForReplay(const Snake op);
	Snake& operator=(const Snake& op);
	Snake crossover(Snake partner);

	NeuralNetwork getBrain();

	std::vector<double> search_at_Direction(int delta_x, int delta_y);
	std::vector<double> getSensor();
	std::vector<double> getOutput();
	std::vector<Food> getFoodList();

	bool bodyCollide(int x, int y);
	bool foodCollide(int x, int y);
	bool wallCollide(int x, int y);
	bool isDead();
	bool isReplay();

	int getScore();
	int getLife();

	double getFitness();

	void draw(sf::RenderWindow& window);
	void update();	//move
	void eat();
	void move();
	void mutate();
	void calculateFitness();
	void search();
	void think();

	void reset();
};
#endif