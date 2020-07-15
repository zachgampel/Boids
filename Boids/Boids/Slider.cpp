#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;

class Slider {
public:
	double value = 0.5;
	bool mousePressed = false;

	sf::RectangleShape sliderBackground;
	sf::RectangleShape sliderScale;
	sf::RectangleShape slider;

	sf::Text description;
	std::string label;

	int sliderBackgroundOffset = 20;
	int sliderScaleHeight = 15;
	int sliderWidth = 15;

public:
	Slider(sf::Vector2f sliderBackgroundPosition, sf::Vector2f sliderBackgroundSize, sf::Font &font, std::string str) {
		sliderBackground.setFillColor(sf::Color(255, 0, 0, 100));
		sliderBackground.setPosition(sliderBackgroundPosition);
		sliderBackground.setSize(sliderBackgroundSize);

		sliderScale.setFillColor(sf::Color(0, 255, 0, 100));

		sf::Vector2f sliderScalePosition = sliderBackground.getPosition();
		sliderScalePosition.x += sliderBackgroundOffset;
		sliderScalePosition.y += sliderBackground.getSize().y / 2 - sliderScaleHeight / 2;
		sliderScale.setPosition(sliderScalePosition);

		sf::Vector2f sliderScaleSize(sliderBackground.getSize().x - 2 * sliderBackgroundOffset, sliderScaleHeight);
		sliderScale.setSize(sliderScaleSize);

		slider.setFillColor(sf::Color(0, 0, 255, 100));
		sf::Vector2f sliderPosition(sliderScale.getPosition().x, sliderScale.getPosition().y);
		sliderPosition.x += sliderScale.getSize().x / 2 - sliderWidth / 2;
		slider.setPosition(sliderPosition);
		slider.setSize(sf::Vector2f(sliderWidth, sliderScale.getSize().y));

		label = str;

		description.setFont(font);
		description.setCharacterSize(24);
		description.setFillColor(sf::Color::White);
		description.setPosition(sliderBackground.getPosition().x, sliderBackground.getPosition().y - 50);
	}

	void show(sf::RenderWindow& window) {
		window.draw(sliderBackground);
		window.draw(sliderScale);
		window.draw(slider);
		window.draw(description);
	}

	double getValue() {
		return value;
	}

	void update(sf::Event& event, sf::Vector2f mouseLocation) {
		description.setString(label + ": " + std::to_string(value));
		if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
			if (mouseLocation.x >= sliderScale.getPosition().x && mouseLocation.x <= sliderScale.getPosition().x + sliderScale.getSize().x) {
				if (mouseLocation.y >= sliderScale.getPosition().y && mouseLocation.y <= sliderScale.getPosition().y + sliderScale.getSize().y) {
					mousePressed = true;
				}
			}
		}
		if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
			mousePressed = false;
		}
		if (mousePressed) {
			sf::Vector2f sliderPosition = slider.getPosition();
			sliderPosition.x = mouseLocation.x - sliderWidth / 2;
			value = (mouseLocation.x - sliderScale.getPosition().x) / sliderScale.getSize().x;
			if (value < 0) {
				value = 0;
				sliderPosition = sliderScale.getPosition();
				sliderPosition.x -= slider.getSize().x / 2;
			}
			if (value > 1) {
				value = 1;
				sliderPosition = Vector2f(sliderScale.getPosition().x + sliderScale.getSize().x - slider.getSize().x / 2, sliderScale.getPosition().y);
			}

			slider.setPosition(sliderPosition);
		}
	}
};