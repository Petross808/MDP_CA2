/*
* Written in class, edited by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#pragma once
#include <SFML/Graphics.hpp>

class Utility
{
public:
	static sf::Vector2f Normalise(const sf::Vector2f& source);
	static void CentreOrigin(sf::Sprite& sprite);
	static void CentreOrigin(sf::Text& text);
	static std::string toString(sf::Keyboard::Scancode key);
	static double toRadians(double degrees);
	static double ToDegrees(double angle);
	static int RandomInt(int exclusive_max);
	static int RandomInt(int inclusive_min, int exclusive_max);
	static float Length(sf::Vector2f vector);
	static float SqrLength(sf::Vector2f vector);
	static float DotProduct(sf::Vector2f first, sf::Vector2f second);
	static sf::Vector2f Abs(sf::Vector2f vector);
	static sf::FloatRect GetPolygonBound(std::vector<sf::Vector2f> vertices);
};

