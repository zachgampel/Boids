#ifndef VECTORUTILS_H
#define VECTORUTILS_H
#include <SFML/Graphics.hpp>

void add(sf::Vector2f& vector1, sf::Vector2f vector2);

void subtract(sf::Vector2f& vector1, sf::Vector2f vector2);

void normalize(sf::Vector2f& vector);

void multiply(sf::Vector2f& vector, double mult);

void divide(sf::Vector2f& vector, double div);

double getDistance(sf::Vector2f& v1, sf::Vector2f& v2);

double getMagnitude(sf::Vector2f& vector);

#endif