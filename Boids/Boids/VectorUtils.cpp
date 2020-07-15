#include <SFML/Graphics.hpp>
#include "VectorUtils.h"


void add(sf::Vector2f &vector1, sf::Vector2f vector2) {
	vector1.x += vector2.x;
	vector1.y += vector2.y;
}

void subtract(sf::Vector2f& vector1, sf::Vector2f vector2) {
	vector1.x -= vector2.x;
	vector1.y -= vector2.y;
}

void normalize(sf::Vector2f& vector) {
	double magnitude = sqrt(vector.x * vector.x + vector.y * vector.y);
	vector.x /= magnitude;
	vector.y /= magnitude;
}

void multiply(sf::Vector2f& vector, double mult) {
	vector.x *= mult;
	vector.y *= mult;
}

void divide(sf::Vector2f& vector, double div) {
	vector.x /= div;
	vector.y /= div;
}

double getDistance(sf::Vector2f &v1, sf::Vector2f &v2) {
	return sqrt((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y));
}

double getMagnitude(sf::Vector2f& vector) {
	return sqrt(vector.x * vector.x + vector.y * vector.y);
}
