#ifndef __FOOD_H__
#define __FOOD_H__
class Food {
	int x, y;
	//SFML
	const int pixel_size = 16;

	sf::Texture green;
	sf::Sprite food;
public:
	Food(int x = 0, int y = 0);
	Food& operator=(const Food& op);

	void draw(sf::RenderWindow& window);
	void update(int x, int y);

	int getX();
	int getY();
};
#endif