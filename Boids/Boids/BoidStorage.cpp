#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "Boid.cpp"


class BoidStorage {
public:
	int gridLength;
	int width;
	int height;
	std::vector<std::vector<Boid>> storage;

	BoidStorage(std::vector<Boid> boids, int maxRadius, int windowWidth, int windowHeight) {
		gridLength = maxRadius;

		width = ceil(double(windowWidth / maxRadius)) + 1;
		height = ceil(double(windowHeight / maxRadius)) + 1;

		//populate the storage with the number of cells required
		for (int i = 0; i < width * height; i++) {
			std::vector<Boid> grid;
			storage.push_back(grid);
		}
	}

	//insert the boids into the boidstorage
	void populate(std::vector<Boid> boids) {
		for (int i = 0; i < boids.size(); i++) {
			Boid boid = boids.at(i);
			int xCoord = boid.position.x / gridLength;
			int yCoord = boid.position.y / gridLength;

			int index = yCoord * width + xCoord;

			//if (index < 0 || index > boids.size()) { continue; }

			storage[index].push_back(boid);
		}
	}

	std::vector<Boid> getNeighbors(Boid boid) {
		std::vector<Boid> neighbors;

		int xCoord = boid.position.x / gridLength;
		int yCoord = boid.position.y / gridLength;

		for (int i = xCoord - 1; i <= xCoord + 1; i++) {
			for (int j = yCoord - 1; j <= yCoord + 1; j++) {
				if (i >= 0 && i < width && j >= 0 && j < height) {
					int index = i + j * width;
					neighbors.insert(neighbors.end(), storage.at(index).begin(), storage.at(index).end());
				}
			}
		}

		return neighbors;
	}

	int getNumberOfLines() {
		int count = 0;

		for (auto grid : storage) {
			for (Boid boid : grid) {
				count += boid.trail.size() - 1;
			}
		}

		return count;
	}
};