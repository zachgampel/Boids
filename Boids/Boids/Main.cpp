#include "VectorUtils.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Boid.cpp"
#include "BoidStorage.cpp"
#include "FPSTracker.cpp"
#include "Slider.cpp"

int main()
{
	int windowWidth = 1200;
	int windowHeight = 800;
	int tileSize = 50; //the dimensions of each "tile" in the BoidStorage

	FPSTracker fpsTracker;
	sf::Font font;
	if (!font.loadFromFile("Aller_BdIt.ttf")) {
		std::cout << "font not found" << std::endl;
		return 0;
	}

	srand(time(NULL));
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Boids");
	window.setFramerateLimit(60);
	sf::Event event;

	Slider cohesionSlider = Slider(sf::Vector2f(50, 750), sf::Vector2f(200, 20), font, "Cohesion");
	Slider alignmentSlider = Slider(sf::Vector2f(200, 650), sf::Vector2f(200, 20), font, "Alignment");
	Slider separateSlider = Slider(sf::Vector2f(350, 750), sf::Vector2f(200, 20), font, "Separation");
	Slider colorFearSlider = Slider(sf::Vector2f(500, 650), sf::Vector2f(200, 20), font, "Color Fear");
	Slider mouseFearSlider = Slider(sf::Vector2f(650, 750), sf::Vector2f(200, 20), font, "Mouse Fear");

	Slider maxNeighborsSlider = Slider(sf::Vector2f(800, 650), sf::Vector2f(200, 20), font, "Max Neighbors");

	int cohesionD = 50;
	int alignmentD = 50;
	int separateD = 30;
	int colorFearD = 50;
	int mouseFearD = 150;

	int stepSize = 2;
	int trailSize = 20;
	int colorCount = 2;
	bool opaque = true;

	std::vector<Boid> boids;
	for (int i = 0; i < 500; i++) {
		Boid b = Boid(windowWidth, windowHeight, trailSize, colorCount);
		boids.push_back(b);
	}

	while (window.isOpen()) {
		sf::Vector2f mouseLocation(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
		while (window.pollEvent(event)) {
			cohesionSlider.update(event, mouseLocation);
			alignmentSlider.update(event, mouseLocation);
			separateSlider.update(event, mouseLocation);
			colorFearSlider.update(event, mouseLocation);
			mouseFearSlider.update(event, mouseLocation);
			maxNeighborsSlider.update(event, mouseLocation);
			if (event.type == sf::Event::Closed) { window.close(); }
		}

		BoidStorage boidGrid(boids, tileSize, windowWidth, windowHeight);
		boidGrid.populate(boids);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			Boid b = Boid(windowWidth, windowHeight, trailSize, colorCount);
			boids.push_back(b);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			if (boids.size() == 1) { continue; }
			boids.pop_back();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			for (int i = 0; i < boids.size(); i++) {
				boids[i].trailSize++;
			}
			trailSize++;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			if (trailSize <= stepSize) { continue; }
			for (int i = 0; i < boids.size(); i++) {
				boids[i].trailSize--;
			}
			trailSize--;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::O)) {
			opaque = !opaque;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add)) {
			if (colorCount <= 7){
				colorCount++;
				for (int i = 0; i < boids.size(); i++) {
					boids[i].updateColor(colorCount);
				}
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract)) {
			if (colorCount >= 2){
				colorCount--;
				for (int i = 0; i < boids.size(); i++) {
					boids[i].updateColor(colorCount);
				}
			}
		}

		for (int i = 0; i < boids.size(); i++) {
			std::vector<Boid> neighbors = boidGrid.getNeighbors(boids[i]);
			
			// to increase performance, decrease the number of neighboring boids according to maxNeighborsSlider
			// randomly remove neighboring boids until between 1 and 101 neighbors remain
			if (maxNeighborsSlider.getValue() != 1 && neighbors.size() > 100 * maxNeighborsSlider.getValue() + 1) {
				while (neighbors.size() > 100 * maxNeighborsSlider.getValue() + 1) {
					std::swap(neighbors[rand() % (neighbors.size() - 1)], neighbors.back());
					neighbors.pop_back();
				}
			}

			//calculate the forces applied on the boid. These are normalized vectors with a magnitude of 1.0
			sf::Vector2f cohesion = boids[i].cohesion(neighbors, cohesionD);
			sf::Vector2f alignment = boids[i].alignment(neighbors, alignmentD);
			sf::Vector2f separate = boids[i].separate(neighbors, separateD);
			sf::Vector2f colorFear = boids[i].colorFear(neighbors, colorFearD);
			sf::Vector2f mouseFear = boids[i].mouseFear(mouseLocation, mouseFearD, window);

			//change the magnitude of each force through the gui
			multiply(cohesion, cohesionSlider.getValue());
			multiply(alignment, alignmentSlider.getValue());
			multiply(separate, separateSlider.getValue());
			multiply(colorFear, colorFearSlider.getValue());
			multiply(mouseFear, mouseFearSlider.getValue());

			//apply the forces on the boid
			boids[i].applyForce(cohesion);
			boids[i].applyForce(alignment);
			boids[i].applyForce(separate);
			boids[i].applyForce(colorFear);
			boids[i].applyForce(mouseFear);
		}

		for (int i = 0; i < boids.size(); i++) {
			boids[i].move(windowWidth, windowHeight);
			boids[i].updateTrail();
			boids[i].show(window, opaque, stepSize);
		}

		int fps = fpsTracker.getFPS();
		int lineCount = boids.size() * (boids[0].trail.size() - 1) / stepSize;

		sf::Text text;
		text.setString("fps: " + std::to_string(fps) + "\tlines drawn: " + std::to_string(lineCount) + "\tboids: " + std::to_string(boids.size()) + "\ttrail size: " + std::to_string(trailSize));
		text.setFont(font);
		text.setCharacterSize(24);
		text.setFillColor(sf::Color::White);
		window.draw(text);

		cohesionSlider.show(window);
		alignmentSlider.show(window);
		separateSlider.show(window);
		colorFearSlider.show(window);
		mouseFearSlider.show(window);
		maxNeighborsSlider.show(window);

		window.display();
		window.clear(sf::Color::Black);
	}

	return 0;
}