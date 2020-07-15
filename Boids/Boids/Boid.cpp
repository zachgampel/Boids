#include <SFML/Graphics.hpp>
#include <iostream>
#include "VectorUtils.h"
#pragma once
class Boid {
	double maxSpeed = 5;
public:
	sf::Vector2f position;
	sf::Vector2f velocity;
	sf::Vector2f acceleration;
	std::vector<sf::Vector2f> trail;
	int trailSize;
	sf::Color color;

	Boid(int windowWidth, int windowHeight, int tSize, int colorCount) {
		trailSize = tSize;

		position.x = rand() % (windowWidth - 1);
		position.y = rand() % (windowHeight - 1);
		acceleration = sf::Vector2f(0, 0);
		trail.push_back(position);

		updateColor(colorCount);

		velocity = sf::Vector2f(rand() % 10 - 5, rand() % 10 - 5);
	}

	void updateColor(int colorCount) {
		int randNum = rand() % colorCount;
		if (randNum == 0) { this->color = sf::Color::Blue; }
		else if (randNum == 1) { this->color = sf::Color::Red; }
		else if (randNum == 2) { this->color = sf::Color::Green; }
		else if (randNum == 3) { this->color = sf::Color::Magenta; }
		else if (randNum == 4) { this->color = sf::Color(255, 165, 0); }
		else if (randNum == 5) { this->color = sf::Color(255, 192, 203); }
		else if (randNum == 6) { this->color = sf::Color(128, 0, 128); }
		else { this->color = sf::Color::Yellow; }
	}


	void applyForce(sf::Vector2f force) {
		add(acceleration, force);
	}

	void move(int windowWidth, int windowHeight) {
		add(velocity, acceleration);
		normalize(velocity);
		multiply(velocity, maxSpeed);

		add(position, velocity);
		multiply(acceleration, 0);

		this->correctPosition(windowWidth, windowHeight);
	}

	void correctPosition(int windowWidth, int windowHeight) {
		if (position.x >= windowWidth) { position.x = 0; }
		if (position.x < 0) { position.x = windowWidth - 1; }
		if (position.y >= windowHeight) { position.y = 0; }
		if (position.y < 0) { position.y = windowHeight - 1; }
	}

	void updateTrail() {
		trail.push_back(position);
		while (trail.size() > trailSize) {
			trail.erase(trail.begin());
		}
	}

	void show(sf::RenderWindow &window, bool opaque, int stepSize) {
		for (int i = 0; i < trail.size() - stepSize; i += stepSize) {
			if (i + stepSize > trail.size() - 1) { continue; }
			sf::Color c1 = color;
			sf::Color c2 = color;
			if (opaque){
				c1.a = 255 * i / trail.size();
				c2.a = 255 * (i + stepSize) / trail.size();
			}

			sf::Vertex line[] = {
				sf::Vertex(sf::Vector2f(trail[i].x, trail[i].y), c1),
				sf::Vertex(sf::Vector2f(trail[i + stepSize].x, trail[i + stepSize].y), c2)
			};

			sf::Vector2f p1 = trail[i];
			sf::Vector2f p2 = trail[i + stepSize];
			if (abs(p1.x - p2.x) > 50 || abs(p1.y - p2.y) > 50) {
				continue;	//this prevents the trail from stretching across the entire screen if the trail crosses the edge
			}

			window.draw(line, 2, sf::Lines);
		}
	}

	void showCheap(sf::RenderWindow& window) {
		int i = trail.size() - 2;

		sf::Vertex line[] = {
			sf::Vertex(sf::Vector2f(trail[i].x, trail[i].y), color),
			sf::Vertex(sf::Vector2f(trail[i + 1].x, trail[i + 1].y), color)
		};

		sf::Vector2f p1 = trail[i];
		sf::Vector2f p2 = trail[i + 1];
		if (abs(p1.x - p2.x) > 50 || abs(p1.y - p2.y) > 50) {
			return;	//this prevents the trail from stretching across the entire screen if the trail crosses the edge
		}

		window.draw(line, 2, sf::Lines);
	}

	void eraseTrailingSegment(sf::RenderWindow& window) {
		sf::Vertex line[] = {
				sf::Vertex(sf::Vector2f(trail[0].x, trail[0].y), sf::Color::Black),
				sf::Vertex(sf::Vector2f(trail[1].x, trail[1].y), sf::Color::Black)
		};
		window.draw(line, 2, sf::Lines);
	}

	// Steer to the centroid of the neighboring boids
	sf::Vector2f cohesion(std::vector<Boid> boids, double radius) {
		sf::Vector2f cohesion(0, 0);
		int count = 0;

		for (Boid neighbor : boids) {
			double distance = getDistance(this->position, neighbor.position);
			if (distance <= radius && distance != 0 && color == neighbor.color) {
				add(cohesion, neighbor.position);
				count++;
			}
		}
		if (count != 0) {
			divide(cohesion, count); //cohesion is the target location
			multiply(cohesion, -1);
			add(cohesion, position);
			multiply(cohesion, -1);
			normalize(cohesion);
		}

		return cohesion;
	}

	sf::Vector2f alignment(std::vector<Boid> boids, double radius) {
		sf::Vector2f alignment(0, 0);
		int count = 0;
		for (Boid neighbor : boids) {
			double distance = getDistance(this->position, neighbor.position);
			if (distance <= radius && distance != 0 && color == neighbor.color) {
				add(alignment, neighbor.velocity);
				count++;
			}
		}
		if (count != 0) { 
			divide(alignment, count);
			normalize(alignment);
		}
		
		return alignment;
	}

	sf::Vector2f separate(std::vector<Boid> boids, double separation) {
		sf::Vector2f alignment(0, 0);
		int count = 0;
		for (Boid neighbor : boids) {
			double distance = getDistance(this->position, neighbor.position);
			if (distance > 0 && distance < separation && color == neighbor.color) {
				count++;
				sf::Vector2f difference(position.x - neighbor.position.x, position.y - neighbor.position.y);
				double magnitude = getMagnitude(difference);
				divide(difference, magnitude);
				add(alignment, difference);
			}
		}
		if (count != 0) {
			divide(alignment, count);
			normalize(alignment);
		}

		return alignment;
	}

	sf::Vector2f mouseFear(sf::Vector2f mouseLocation, double radius, sf::RenderWindow &window) {
		if (mouseLocation.x >= 0 && mouseLocation.x <= window.getSize().x && mouseLocation.y >= 0 && mouseLocation.y <= window.getSize().y) {
			sf::Vector2f mouseFear = this->position;
			double distance = getDistance(this->position, mouseLocation);
			if (distance <= radius) {
				subtract(mouseFear, mouseLocation);
				normalize(mouseFear);
				return mouseFear;
			}
		}
		return sf::Vector2f(0, 0);
	}

	sf::Vector2f colorFear(std::vector<Boid> boids, double radius) {
		sf::Vector2f colorFear(0, 0);
		int count = 0;
		for (Boid neighbor : boids) {
			double distance = getDistance(this->position, neighbor.position);
			if (distance > 0 && distance < radius && color != neighbor.color) {
				subtract(colorFear, neighbor.position);
				add(colorFear, this->position);
				count += 2;
			}
		}
		if (count != 0) {
			divide(colorFear, count);
			normalize(colorFear);
		}

		return colorFear;
	}
};
