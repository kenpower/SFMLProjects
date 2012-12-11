
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "stdafx.h"
#ifdef _DEBUG
#pragma comment(lib,"sfml-graphics-d.lib")
#pragma comment(lib,"sfml-audio-d.lib")
#pragma comment(lib,"sfml-system-d.lib")
#pragma comment(lib,"sfml-window-d.lib")
#pragma comment(lib,"sfml-network-d.lib")
#else
#pragma comment(lib,"sfml-graphics.lib")
#pragma comment(lib,"sfml-audio.lib")
#pragma comment(lib,"sfml-system.lib")
#pragma comment(lib,"sfml-window.lib")
#pragma comment(lib,"sfml-network.lib")
#endif
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")

//#include <GL/glew.h>
#include "SFML/Graphics.hpp"
#include "SFML/OpenGL.hpp"
#include <iostream>

//#include <SFML/Graphics/Image.hpp>



////////////////////////////////////////////////////////////
/// Entry point of application
///
///
////////////////////////////////////////////////////////////
int main()
{
    // Create the main window
    //sf::RenderWindow window(sf::VideoMode::getFullscreenModes()[0], "SFML Window", sf::Style::Fullscreen);
    sf::RenderWindow window(sf::VideoMode(800,600,32), "SFML Window");
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,800,0,600,0,1); // use pixel coordinates


    glClearColor(0.3f, 0.3f, 0.3f, 0.f);


	//cretae a sprite in SFML
	sf::Image Image1;
	Image1.create(128, 128, sf::Color(0, 255, 0));
	
	sf::Vector2u v2= Image1.getSize();
	int sz= sf::Texture::getMaximumSize();
	
	sf::Texture texture;
	bool bl=texture.loadFromImage(Image1);


	sf::Sprite Sprite;
	Sprite.setTexture(texture);
	Sprite.setColor(sf::Color(0, 255, 255, 128));
	Sprite.setPosition(200,100);


	Sprite.scale(100/Sprite.getLocalBounds().width,100/Sprite.getLocalBounds().height);


	Sprite.rotate(45);
	Sprite.scale(1.5f, 1.5f);

	// what version of OPenGL are we using??
	sf::ContextSettings settings = window.getSettings();
	std::cout << "OpenGL version:"<< settings.majorVersion << "." << settings.minorVersion << std::endl;

	
	
	while (window.isOpen())
    {
        // Process events
        sf::Event Event;
        while (window.pollEvent(Event))
        {
            // Close window : exit
            if (Event.type == sf::Event::Closed)
                window.close();

            // Escape key : exit
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
                window.close();


			//resize window
			if (Event.type == sf::Event::Resized){
				glViewport(0, 0, Event.size.width, Event.size.height);
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				glOrtho(0,Event.size.width,0,Event.size.height,0,1);

			}
 
		}

        // Clear color and depth buffer
       glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	   glMatrixMode(GL_MODELVIEW);
	   glLoadIdentity();
	  

	   

		glBegin(GL_TRIANGLES);//draw a triangle
			glColor3d(0,1,1);
            glVertex2d(0,0);
       		glVertex2d(400,0);     
			glVertex2d(800,600);
		glEnd();
	


		window.pushGLStates();
		window.draw(Sprite);
		window.popGLStates();

        // Finally, display rendered frame on screen
        window.display();
    }

    return EXIT_SUCCESS;
}


