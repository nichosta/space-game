#include "Header.h"
float toRadians(float degrees)
{
	return degrees / 180 * 3.14;
}
float toDegrees(float radians)
{
	return radians / 3.14 * 180;
}
bool collision(sf::RectangleShape rect, sf::CircleShape coin)
{
	return rect.getGlobalBounds().intersects(coin.getGlobalBounds());
}
bool collisionUFO(sf::RectangleShape rect, sf::Sprite spr)
{
	return rect.getGlobalBounds().intersects(spr.getGlobalBounds());
}
int main()
{
	// Window
	sf::RenderWindow window;
	window.create(sf::VideoMode::getFullscreenModes()[0], "Spaceship", sf::Style::Fullscreen);
	// Level
	int level = 1;
	// UFO Textures
	sf::Texture ufoTex;
	ufoTex.loadFromFile("C:\\Users\\student\\Desktop\\Nicholas C\\ufo.png");
	vector<sf::Sprite> ufos = {};
	vector<int> ufoTimers = { 0, 100 };
	vector<int> ufoRotations = { rand() % 359, rand() % 359 };
	for (int i = 0; i < 2; i++)
	{
		ufos.push_back(sf::Sprite());
		ufos[i].setTexture(ufoTex);
		ufos[i].setScale(6, 6);
		ufos[i].setPosition(sf::Vector2f(rand() % 3900 + 50, rand() % 3900 + 50));
	}
	// Spaceship texture
	sf::Texture tex;
	tex.loadFromFile("spaceship.png");
	sf::Sprite spaceship;
	spaceship.setTexture(tex);
	spaceship.setOrigin(18, 37);
	// Background image
	sf::Texture background;
	background.loadFromFile("background.jpg");
	sf::Sprite backSprite;
	backSprite.setTexture(background);
	backSprite.setPosition(sf::Vector2f(0, 0));
	// Actual game rectangle
	sf::RectangleShape rect(sf::Vector2f(36, 74));
	rect.setPosition(2000, 2000);
	rect.setOrigin(18, 37);
	bool spaceshipVulnerable = false;
	int spaceshipVulnerableTimer = 0;
	bool spaceshipFlashing = false;
	// Coins
	sf::CircleShape coins[10];
	bool liveCoins[10] = { true, true, true, true, true, true, true, true, true, true };
	for (int i = 0; i < 10; i++)
	{
		coins[i] = sf::CircleShape(25);
		coins[i].setPosition(sf::Vector2f(rand() % 3900 + 50, rand() % 3900 + 50));
		coins[i].setFillColor(sf::Color::Yellow);
		coins[i].setOutlineColor(sf::Color::Black);
		coins[i].setOutlineThickness(2);
	}
	// Ship velocity vars
	float shipVeloX = 0;
	float shipVeloY = 0;
	// Score
	int points = 0;
	// Points text
	sf::Font TimesNewRoman;
	TimesNewRoman.loadFromFile("C:\\Windows\\Fonts\\times.ttf");
	sf::Text score("Ponits", TimesNewRoman);
	score.setCharacterSize(100);
	score.setColor(sf::Color::White);
	score.setStyle(sf::Text::Bold);
	// Level text
	sf::Text lvlText("Level: ", TimesNewRoman);
	lvlText.setCharacterSize(120);
	lvlText.setColor(sf::Color::White);
	lvlText.setStyle(sf::Text::Bold);
	// Heart textures
	sf::Texture heartTex;
	heartTex.loadFromFile("heart.png");
	sf::Sprite hearts[3];
	for (int i = 0; i < 3; i++)
	{
		hearts[i].setTexture(heartTex);
	}
	int lives = 3;
	// Game over text
	sf::Text lolNoob;
	lolNoob.setCharacterSize(120);
	lolNoob.setFont(TimesNewRoman);
	lolNoob.setColor(sf::Color::Green);
	lolNoob.setString("Diagnosis: You're dead!");
	// Teleport rectangle
	sf::RectangleShape meter;
	meter.setSize(sf::Vector2f(600, 50));
	meter.setFillColor(sf::Color::Red);
	meter.setOutlineColor(sf::Color::Black);
	meter.setOutlineThickness(3);
	//Teleport charge
	int ultCharge = 30000;
	// Camera position
	window.setView(sf::View(sf::Vector2f(960, 540), sf::Vector2f(1920, 1080)));

	// Rerunnables
	while (window.isOpen())
	{
		// Process events
		// Not sure what these do but I'm afraid of touching them
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Close window: exit
			if (event.type == sf::Event::Closed)
				window.close();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			shipVeloX += -sin(toRadians(rect.getRotation())) * 3;
			shipVeloY += cos(toRadians(rect.getRotation())) * 3;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			rect.rotate(-0.2);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			rect.rotate(0.2);
		}
		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) && ultCharge == 30000)
		{
			rect.setPosition(sf::Vector2f(rand() % 3900 + 50, rand() % 3900 + 50));
			meter.setFillColor(sf::Color::Red);
			ultCharge = 0;
			spaceshipVulnerable = false;
			spaceshipVulnerableTimer = 2000;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			window.close();
		}

		rect.move(shipVeloX / 6000, shipVeloY / 6000);

		int yPos = rect.getPosition().y;
		int xPos = rect.getPosition().x;

		if (rect.getPosition().x > 4000) rect.setPosition(sf::Vector2f(0, rect.getPosition().y));
		if (rect.getPosition().x < 0) rect.setPosition(sf::Vector2f(4000, rect.getPosition().y));
		if (rect.getPosition().y > 4000) rect.setPosition(sf::Vector2f(rect.getPosition().x, 0));
		if (rect.getPosition().y < 0) rect.setPosition(sf::Vector2f(rect.getPosition().x, 4000));

		window.setView(sf::View(sf::Vector2f(min(max(xPos, 960), 3040), min(max(yPos, 540), 3460)), sf::Vector2f(1920, 1080)));

		if (shipVeloX > 1) shipVeloX--;
		else if (shipVeloX < 1 && shipVeloX > 0) shipVeloX = 0;
		if (shipVeloX < -1) shipVeloX++;
		else if (shipVeloX > -1 && shipVeloX < 0) shipVeloX = 0;
		if (shipVeloY > 1) shipVeloY--;
		else if (shipVeloY < 1 && shipVeloY > 0) shipVeloY = 0;
		if (shipVeloY < -1) shipVeloY++;
		else if (shipVeloY > -1 && shipVeloY < 0) shipVeloY = 0;

		spaceship.setPosition(rect.getPosition());
		spaceship.setRotation(rect.getRotation() + 180);

		score.setString("Ponits: " + std::to_string(points));
		lvlText.setString("Level: " + std::to_string(level));
		
		for (int i = 0; i < 10; i++) 
		{
			if (collision(rect, coins[i]))
			{
				coins[i].setPosition(sf::Vector2f(rand() % 3900 + 50, rand() % 3900 + 50));
				points++;
				ultCharge += min(3000, 30000 - ultCharge);
			}
		}

		lvlText.setPosition(sf::Vector2f(window.getView().getCenter().x - 180, window.getView().getCenter().y + 400));
		score.setPosition(sf::Vector2f(window.getView().getCenter().x - 160, window.getView().getCenter().y + 300));
		
		meter.setPosition(sf::Vector2f(window.getView().getCenter().x - 300, window.getView().getCenter().y - 475));
		meter.setSize(sf::Vector2f(ultCharge / 50, 50));

		for (int i = 0; i < 3; i++)
		{
			hearts[i].setPosition(rect.getPosition());
			hearts[i].move(sf::Vector2f(i * 40 - 60, -80));
		}
		
		for (int i = 0; i < level * 2 - 1; i++)
		{
			ufoTimers[i]++;
			if (ufoTimers[i] == 1000)
			{
				ufoRotations[i] = rand() % 359;
				ufoTimers[i] = 0;
			}
		}

		if (!spaceshipVulnerable) spaceshipVulnerableTimer++;
		if (spaceshipVulnerableTimer > 5000) spaceshipVulnerable = true;
		if (!spaceshipVulnerable && !spaceshipFlashing) spaceshipFlashing = true;
		else spaceshipFlashing = false;
		
		for (int i = 0; i < level * 2 - 1; i++)
		{
			ufos[i].move(sf::Vector2f(-sin(ufoRotations[i]) * 0.3, cos(ufoRotations[i]) * 0.3));
			if (ufos[i].getPosition().x > 4000) ufos[i].setPosition(sf::Vector2f(0, ufos[i].getPosition().y));
			if (ufos[i].getPosition().x < 0) ufos[i].setPosition(sf::Vector2f(4000, ufos[i].getPosition().y));
			if (ufos[i].getPosition().y > 4000) ufos[i].setPosition(sf::Vector2f(ufos[i].getPosition().x, 0));
			if (ufos[i].getPosition().y < 0) ufos[i].setPosition(sf::Vector2f(ufos[i].getPosition().x, 4000));
			if (collisionUFO(rect, ufos[i]) && spaceshipVulnerable)
			{
				rect.setPosition(sf::Vector2f(2000, 2000));
				shipVeloX = 0;
				shipVeloY = 0;
				spaceshipVulnerable = false;
				spaceshipVulnerableTimer = 0;
				lives--;
			}
		}
		
		lolNoob.setPosition(sf::Vector2f(window.getView().getCenter().x - 500, window.getView().getCenter().y - 200));

		if (ultCharge < 30000) ultCharge++;
		else if (ultCharge == 30000) meter.setFillColor(sf::Color::Green);
		
		if (points == level)
		{
			level++;
			points = 0;
			for (int i = level * 2 - 3; i < level * 2 - 1; i++)
			{
				ufos.push_back(sf::Sprite());
				ufos[i].setTexture(ufoTex);
				ufos[i].setPosition(sf::Vector2f(rand() % 3900 + 50, rand() % 3900 + 50));
				ufos[i].setScale(6, 6);
				ufoRotations.push_back(rand() % 359);
				ufoTimers.push_back(rand() % 999);
			}
		}

		//Drawing functions
		window.clear();
		window.draw(backSprite);
		if (!spaceshipFlashing) window.draw(spaceship);
		for (int i = 0; i < level * 2 - 1; i++)
		{
			window.draw(ufos[i]);
		}
		for (int i = 0; i < 10; i++) if (liveCoins[i]) window.draw(coins[i]);
		window.draw(score);
		window.draw(meter);
		for (int i = 2; i >= (3 - lives); i--) window.draw(hearts[i]);
		window.draw(lvlText);
		if (lives <= 0)
		{
			window.draw(lolNoob);
			window.display();
			_sleep(1000);
			window.close();
		}
		window.display();
	}
}