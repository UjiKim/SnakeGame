#include <random>
#include <iostream>
using namespace std;

#include "Random.h"

int Random::getRandom(int min, int max){
	random_device rd;
	mt19937_64 gen(rd());
	uniform_int_distribution<int> dist(min,nextafter(max, max + 1));
	return dist(gen);
}

double Random::getRandom(double min, double max){
	random_device rd;
	mt19937_64 gen(rd());
	uniform_real_distribution<double> dist(min, nextafter(max, max + 1));
	return dist(gen);
}
