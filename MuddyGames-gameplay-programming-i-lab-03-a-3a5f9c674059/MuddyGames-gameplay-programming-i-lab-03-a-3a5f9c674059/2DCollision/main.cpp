#include <iostream>
#include <SFML/Graphics.hpp>
#define TINYC2_IMPL
#include <tinyc2.h>
#include <AnimatedSprite.h>
#include <Player.h>
#include <Input.h>
#include <Debug.h>

using namespace std;

int main()
{
	// Create the main window
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

	// Load a sprite to display
	sf::Texture sprite_sheet;
	if (!sprite_sheet.loadFromFile("assets\\grid.png")) {
		DEBUG_MSG("Failed to load file");
		return EXIT_FAILURE;
	}

	// Load a mouse texture to display
	sf::Texture mouse_texture;
	if (!mouse_texture.loadFromFile("assets\\mouse.png")) {
		DEBUG_MSG("Failed to load file");
		return EXIT_FAILURE;
	}

	// Setup a mouse Sprite
	sf::Sprite mouse;
	mouse.setTexture(mouse_texture);



	bool mouse_changer[3]{ 1,0,0 };

	//Setup mouse
	c2AABB aabb_mouse;
	aabb_mouse.min = c2V(mouse.getPosition().x, mouse.getPosition().y);
	aabb_mouse.max = c2V(mouse.getGlobalBounds().width, mouse.getGlobalBounds().width);

	c2Circle circle_mouse;
	circle_mouse.p = { aabb_mouse.min.x + 42, aabb_mouse.min.y + 42 };
	circle_mouse.r = 42;

	c2Ray ray_mouse;


	// Setup Players Default Animated Sprite
	AnimatedSprite animated_sprite(sprite_sheet);
	animated_sprite.addFrame(sf::IntRect(3, 3, 84, 84));
	animated_sprite.addFrame(sf::IntRect(88, 3, 84, 84));
	animated_sprite.addFrame(sf::IntRect(173, 3, 84, 84));
	animated_sprite.addFrame(sf::IntRect(258, 3, 84, 84));
	animated_sprite.addFrame(sf::IntRect(343, 3, 84, 84));
	animated_sprite.addFrame(sf::IntRect(428, 3, 84, 84));

	// Setup Player
	c2AABB aabb_player;
	aabb_player.min = c2V(animated_sprite.getPosition().x, animated_sprite.getPosition().y);
	aabb_player.max = c2V(animated_sprite.getGlobalBounds().width / animated_sprite.getFrames().size(),
		animated_sprite.getGlobalBounds().height / animated_sprite.getFrames().size());

	c2Capsule capsule_player;

	c2Poly * polygon_player = new c2Poly;

	polygon_player->count = 4;

	polygon_player->verts[0] = { animated_sprite.getGlobalBounds().width / animated_sprite.getFrames().size(),
		animated_sprite.getGlobalBounds().height / animated_sprite.getFrames().size() };
	polygon_player->verts[1] = {0,0};
	polygon_player->verts[2] = {0,0};
	polygon_player->verts[3] = {0,0};


	c2Circle circle_player;

	c2Ray ray_player;


	// Setup the Player
	Player player(animated_sprite);
	Input input;

	// Collision result
	int result = 0;
	
	// Start the game loop
	while (window.isOpen())
	{
		// Move Sprite Follow Mouse
		mouse.setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));

		// Update mouse AABB
		aabb_mouse.min = c2V(mouse.getPosition().x, mouse.getPosition().y);
		aabb_mouse.max = c2V(mouse.getGlobalBounds().width, mouse.getGlobalBounds().width);

		circle_mouse.p = { aabb_mouse.min.x + 42, aabb_mouse.min.y + 42 };


		// Process events
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				// Close window : exit
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				{
					input.setCurrent(Input::Action::LEFT);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				{
					input.setCurrent(Input::Action::RIGHT);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				{
					input.setCurrent(Input::Action::UP);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
				{
					mouse_changer[0] = 1;
					mouse_changer[1] = 0;
					mouse_changer[2] = 0;

				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
				{
					mouse_changer[0] = 0;
					mouse_changer[1] = 1;
					mouse_changer[2] = 0;

				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
				{
					mouse_changer[0] = 0;
					mouse_changer[1] = 0;
					mouse_changer[2] = 1;

				}
				break;
			default:
				input.setCurrent(Input::Action::IDLE);
				break;
			}
		}

		// Handle input to Player
		player.handleInput(input);

		// Update the Player
		player.update();




		// Check for collisions
		if (mouse_changer[0])	// AABB
		{
			//result = c2AABBtoAABB(aabb_mouse, aabb_player);
			//cout << ((result != 0) ? ("Collision AABB - AABB") : "") << endl;

			//result = c2AABBtoCapsule(aabb_mouse, capsule_player);
		//	cout << ((result != 0) ? ("Collision AABB - Capsule") : "") << endl;

			result = c2AABBtoPoly(aabb_mouse, polygon_player, NULL);
			cout << ((result != 0) ? ("Collision AABB - Poly") : "") << endl;

			//ray

		}
		else if (mouse_changer[1]) // Circle
		{
			result = c2CircletoAABB(circle_mouse, aabb_player);
			cout << ((result != 0) ? ("Collision Circle - AABB") : "") << endl;

			//result = c2CircletoCircle(circle_mouse, circle_player);
			//cout << ((result != 0) ? ("Collision Circle - Circle") : "") << endl;

			//result = c2CircletoCapsule(circle_mouse, capsule_player);
			//cout << ((result != 0) ? ("Collision Circle - Capsule") : "") << endl;

		//	result = c2CircletoPoly(circle_mouse, polygon_player, transformation);
		//	cout << ((result != 0) ? ("Collision Circle - Poly") : "") << endl;

			//ray

		}
		else if (mouse_changer[2]) // Ray
		{
			
		}
		
		
		
		

	

		


		// change colour for intersection, 
		if (result){
			player.getAnimatedSprite().setColor(sf::Color(255,0,0));
		}
		else {
			player.getAnimatedSprite().setColor(sf::Color(0, 255, 0));
		}






		// Clear screen
		window.clear();

		// Draw the Players Current Animated Sprite
		window.draw(player.getAnimatedSprite());
		window.draw(mouse);

		// Update the window
		window.display();
	}

	return EXIT_SUCCESS;
};