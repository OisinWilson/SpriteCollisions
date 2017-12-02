#include <iostream>
#include <SFML/Graphics.hpp>
#define TINYC2_IMPL
#include <tinyc2.h>
#include <AnimatedSprite.h>
#include <Player.h>
#include <Input.h>
#include <Debug.h>

#include<Tinyc2Debug.hpp>


// make sure to get rid of the debug before final solution

using namespace std;

int main()
{
	// Create the main window
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");


	Tinyc2Debug g(window);
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
	aabb_mouse.max = c2V(mouse.getPosition().x + mouse.getGlobalBounds().width, mouse.getPosition().y + mouse.getGlobalBounds().height);

	c2Circle circle_mouse;
	circle_mouse.p = { aabb_mouse.min.x + 42, aabb_mouse.min.y + 42 };
	circle_mouse.r = 42;

	c2Ray ray_mouse;
	ray_mouse.p = c2V(aabb_mouse.min.x, aabb_mouse.min.y);
	ray_mouse.d = c2Norm(c2V(1, 1));
	ray_mouse.t = 125;

	// Setup Players Default Animated Sprite
	AnimatedSprite animated_sprite(sprite_sheet);
	animated_sprite.addFrame(sf::IntRect(3, 3, 84, 84));
	animated_sprite.addFrame(sf::IntRect(88, 3, 84, 84));
	animated_sprite.addFrame(sf::IntRect(173, 3, 84, 84));
	animated_sprite.addFrame(sf::IntRect(258, 3, 84, 84));
	animated_sprite.addFrame(sf::IntRect(343, 3, 84, 84));
	animated_sprite.addFrame(sf::IntRect(428, 3, 84, 84));

	AnimatedSprite polyShape(sprite_sheet);
	polyShape.addFrame(sf::IntRect(3, 171, 84, 84));
	polyShape.setPosition(200,102);

	AnimatedSprite circleShape(sprite_sheet);
	circleShape.addFrame(sf::IntRect(3, 87, 84, 84));
	circleShape.setPosition(79, 459);

	AnimatedSprite capsuleShape(sprite_sheet);
	capsuleShape.addFrame(sf::IntRect(3, 255, 84, 168));
	capsuleShape.setRotation(270);
	capsuleShape.setPosition(465,192);

	AnimatedSprite rayShape(sprite_sheet);
	rayShape.addFrame(sf::IntRect(3, 425, 168, 84));
	rayShape.setRotation(-1);
	rayShape.setPosition(500,418);

	// Setup Player
	c2Manifold manifold;
	c2Raycast cast;

	c2AABB aabb_player;
	aabb_player.min = c2V(animated_sprite.getPosition().x, animated_sprite.getPosition().y);
	aabb_player.max = c2V(animated_sprite.getGlobalBounds().width / animated_sprite.getFrames().size(),
		animated_sprite.getGlobalBounds().height / animated_sprite.getFrames().size());

	c2Capsule capsule_player;
	capsule_player.a = c2V(509, 150);
	capsule_player.b = c2V(593, 150);
	capsule_player.r = 40;

	c2Poly polygon_player;
	polygon_player.count = 3;
	polygon_player.verts[0] = c2V(200,100);
	polygon_player.verts[1] = c2V(200,185);
	polygon_player.verts[2] = c2V(285,185);
	c2MakePoly(&polygon_player);
	

	c2Circle circle_player;
	circle_player.r = { 40 };
	circle_player.p = { 120,500 };
	
	c2Ray ray_player;
	ray_player.p = c2V(502, 500);
	ray_player.d = c2Norm(c2V(2, -1));
	ray_player.t = 185;


	// Setup the Player
	Player player(animated_sprite);
	Input input;

	Player playerPoly(polyShape);
	Player playerCircle(circleShape);
	Player playerCapsule(capsuleShape);
	Player PlayerRay(rayShape);

	// Collision result
	int result = 0;
	
	// Start the game loop
	while (window.isOpen())
	{
		// Move Sprite Follow Mouse
		mouse.setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));

		// Update mouse AABB
		aabb_mouse.min = c2V(mouse.getPosition().x, mouse.getPosition().y);
		aabb_mouse.max = c2V(mouse.getPosition().x + mouse.getGlobalBounds().width, mouse.getPosition().y + mouse.getGlobalBounds().height);

		circle_mouse.p = { aabb_mouse.min.x + 42, aabb_mouse.min.y + 42 };
		ray_mouse.p = { aabb_mouse.min.x, aabb_mouse.min.y };


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
		playerPoly.update();
		playerCircle.update();




		// Check for collisions
		if (mouse_changer[0])	// AABB
		{
			result = c2AABBtoAABB(aabb_mouse, aabb_player);
			cout << ((result != 0) ? ("Collision AABB - AABB") : "") << endl;
			if (result) {
				player.getAnimatedSprite().setColor(sf::Color(255, 0, 0));
			}
			else {
				player.getAnimatedSprite().setColor(sf::Color(0, 255, 0));
			}

			result = c2AABBtoCapsule(aabb_mouse, capsule_player);
			cout << ((result != 0) ? ("Collision AABB - Capsule") : "") << endl;
			if (result) {
				playerCapsule.getAnimatedSprite().setColor(sf::Color(255, 0, 0));
			}
			else {
				playerCapsule.getAnimatedSprite().setColor(sf::Color(0, 255, 0));
			}

			c2AABBtoPolyManifold(aabb_mouse, &polygon_player, NULL, &manifold);
			cout << ((manifold.count != 0) ? ("Collision AABB - Poly") : "") << endl;
			if (manifold.count !=0) {
				playerPoly.getAnimatedSprite().setColor(sf::Color(255, 0, 0));
			}
			else {
				playerPoly.getAnimatedSprite().setColor(sf::Color(0, 255, 0));
			}

			result = c2RaytoAABB(ray_player, aabb_mouse, &cast);
			cout << ((result != 0) ? ("Collision AABB - Ray") : "") << endl;
			if (result) {
				PlayerRay.getAnimatedSprite().setColor(sf::Color(255, 0, 0));
			}
			else {
				PlayerRay.getAnimatedSprite().setColor(sf::Color(0, 255, 0));
			}

			result = c2CircletoAABB(circle_player, aabb_mouse);
			cout << ((result != 0) ? ("Collision AABB - Circle") : "") << endl;
			if (result) {
				playerCircle.getAnimatedSprite().setColor(sf::Color(255, 0, 0));
			}
			else {
				playerCircle.getAnimatedSprite().setColor(sf::Color(0, 255, 0));
			}
		}
		else if (mouse_changer[1]) // Circle
		{
			result = c2CircletoAABB(circle_mouse, aabb_player);
			cout << ((result != 0) ? ("Collision Circle - AABB") : "") << endl;
			if (result) {
				player.getAnimatedSprite().setColor(sf::Color(255, 0, 0));
			}
			else {
				player.getAnimatedSprite().setColor(sf::Color(0, 255, 0));
			}

			result = c2CircletoCircle(circle_mouse, circle_player);
			cout << ((result != 0) ? ("Collision Circle - Circle") : "") << endl;
			if (result) {
				playerCircle.getAnimatedSprite().setColor(sf::Color(255, 0, 0));
			}
			else {
				playerCircle.getAnimatedSprite().setColor(sf::Color(0, 255, 0));
			}

			result = c2CircletoCapsule(circle_mouse, capsule_player);
			cout << ((result != 0) ? ("Collision Circle - Capsule") : "") << endl;
			if (result) {
				playerCapsule.getAnimatedSprite().setColor(sf::Color(255, 0, 0));
			}
			else {
				playerCapsule.getAnimatedSprite().setColor(sf::Color(0, 255, 0));
			}

			c2CircletoPolyManifold(circle_mouse, &polygon_player, NULL, &manifold);
			cout << ((manifold.count != 0) ? ("Collision Circle - Poly") : "") << endl;
			if (manifold.count !=0) {
				playerPoly.getAnimatedSprite().setColor(sf::Color(255, 0, 0));
			}
			else {
				playerPoly.getAnimatedSprite().setColor(sf::Color(0, 255, 0));
			}

			//issues
			result = c2RaytoCircle(ray_player, circle_mouse, &cast);
			cout << ((result != 0) ? ("Collision Circle - Ray") : "") << endl;
			if (result) {
				PlayerRay.getAnimatedSprite().setColor(sf::Color(255, 0, 0));
			}
			else {
				PlayerRay.getAnimatedSprite().setColor(sf::Color(0, 255, 0));
			}

		}
		else if (mouse_changer[2]) // Ray
		{
			result = c2RaytoAABB(ray_mouse, aabb_player, &cast);
			cout << ((result != 0) ? ("Collision Ray - AABB") : "") << endl;
			if (result) {
				player.getAnimatedSprite().setColor(sf::Color(255, 0, 0));
			}
			else {
				player.getAnimatedSprite().setColor(sf::Color(0, 255, 0));
			}

			result = c2RaytoCircle(ray_mouse, circle_player, &cast);
			cout << ((result != 0) ? ("Collision Ray - Circle") : "") << endl;
			if (result) {
				playerCircle.getAnimatedSprite().setColor(sf::Color(255, 0, 0));
			}
			else {
				playerCircle.getAnimatedSprite().setColor(sf::Color(0, 255, 0));
			}

			result = c2RaytoCapsule(ray_mouse, capsule_player, &cast);
			cout << ((result != 0) ? ("Collision Ray - Capsule") : "") << endl;
			if (result) {
				playerCapsule.getAnimatedSprite().setColor(sf::Color(255, 0, 0));
			}
			else {
				playerCapsule.getAnimatedSprite().setColor(sf::Color(0, 255, 0));
			}

			result = c2RaytoPoly(ray_mouse,&polygon_player,NULL,&cast);
			cout << ((result != 0) ? ("Collision Ray - Poly") : "") << endl;
			if (result) {
				playerPoly.getAnimatedSprite().setColor(sf::Color(255, 0, 0));
			}
			else {
				playerPoly.getAnimatedSprite().setColor(sf::Color(0, 255, 0));
			}
		}
	


		// Clear screen
		window.clear();

		// Draw the Players Current Animated Sprite
		window.draw(player.getAnimatedSprite());
		window.draw(playerPoly.getAnimatedSprite());
		window.draw(playerCircle.getAnimatedSprite());
		window.draw(playerCapsule.getAnimatedSprite());

		if (!mouse_changer[2]) {
			window.draw(PlayerRay.getAnimatedSprite());
		}

		window.draw(mouse);

		//Remove later
		g.draw(aabb_mouse, sf::Color::Cyan);
		g.draw(circle_mouse, sf::Color::Cyan);
		g.draw(ray_mouse, sf::Color::Cyan);

		// Update the window
		window.display();
	}

	return EXIT_SUCCESS;
};