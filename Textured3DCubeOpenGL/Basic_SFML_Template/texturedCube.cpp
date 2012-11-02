
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

#include "SFML/Graphics.hpp"
#include "SFML/OpenGL.hpp"
#include <iostream>




////////////////////////////////////////////////////////////
/// Entry point of application
////////////////////////////////////////////////////////////
int main()
{
    // Create the main window
    sf::RenderWindow App(sf::VideoMode(800, 600, 32), "SFML OpenGL");

    // Create a clock for measuring time elapsed
    sf::Clock Clock;

	glEnable(GL_TEXTURE_2D);
	//load first texture
	sf::Image senna_img;
	sf::String senna_file="../Images/lotus1986.png";
	if (!senna_img.loadFromFile(senna_file))
	{
		std::cout << "Could not load " << senna_file.getData();
		
	}	
	
	GLuint senna_texture;
	glGenTextures(1, &senna_texture);
	glBindTexture(GL_TEXTURE_2D,senna_texture);

	glTexImage2D(
		GL_TEXTURE_2D, 
		0, 
		GL_RGBA,
		senna_img.getSize().x, senna_img.getSize().y,
		0,
		GL_RGBA, 
		GL_UNSIGNED_BYTE, 
		senna_img.getPixelsPtr()
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	

	sf::Image clarke_img;
	sf::String clarke_file="../Images/clarkelotus.jpg";
	if (!clarke_img.loadFromFile(clarke_file))
	{
		std::cout << "Could not load " << clarke_file.getData();
		
	}
	
	GLuint clarke_texture;
	glGenTextures(1, &clarke_texture);
	glBindTexture(GL_TEXTURE_2D,clarke_texture);

	glTexImage2D(
		GL_TEXTURE_2D, 
		0, GL_RGB,
		clarke_img.getSize().x, clarke_img.getSize().y,
		0,
		GL_RGBA, 
		GL_UNSIGNED_BYTE, 
		clarke_img.getPixelsPtr()
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	

    //prepare OpenGL surface for HSR
	glClearDepth(1.f);
    glClearColor(0.3f, 0.3f, 0.3f, 0.f);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

	

    //// Setup a perspective projection & Camera position
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.f, 1.f, 1.f, 300.0f);//fov, aspect, zNear, zFar



    bool rotate=true;
	double anglex;
	double angley;
	
	// Start game loop
	while (App.isOpen())
    {
        // Process events
        sf::Event Event;
        while (App.pollEvent(Event))
        {
            // Close window : exit
            if (Event.type == sf::Event::Closed)
                App.close();

            // Escape key : exit
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
                App.close();

			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::A)){
				rotate=!rotate;
			}
 
		}
        
        //Prepare for drawing
		// Clear color and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Apply some transformations for the cube
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.f, 0.f, -200.f);
		

		if(rotate){
			sf::Vector2i mp = sf::Mouse::getPosition();
			anglex=mp.y/100.0;
			angley=-mp.x/100.0;
		}
		glRotatef(anglex * 50, 1.f, 0.f, 0.f);
		glRotatef(angley * 30, 0.f, 1.f, 0.f);
		
			
		glColor3d(1,1,1);
		 
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,senna_texture);

		//Draw a cube
        glBegin(GL_QUADS);//draw some squares

			glTexCoord2d(0.0,0.0);glVertex3f(-50.f, -50.f, -50.f);
			glTexCoord2d(0.0,1.0);glVertex3f(-50.f,  50.f, -50.f);
			glTexCoord2d(1.0,1.0);glVertex3f( 50.f,  50.f, -50.f);
			glTexCoord2d(1.0,0.0);glVertex3f( 50.f, -50.f, -50.f);

			//glColor3f(0,0,1);
            glTexCoord2d(0.0,0.0);glVertex3f(-50.f, -50.f, 50.f);
            glTexCoord2d(0.0,1.0);glVertex3f(-50.f,  50.f, 50.f);
            glTexCoord2d(1.0,1.0);glVertex3f( 50.f,  50.f, 50.f);
            glTexCoord2d(1.0,0.0);glVertex3f( 50.f, -50.f, 50.f);
		glEnd();


		glBindTexture(GL_TEXTURE_2D,clarke_texture);

		glBegin(GL_QUADS);//draw some squares
	
            glTexCoord2d(0.0,0.0); glVertex3f(-50.f, -50.f, -50.f);
            glTexCoord2d(0.0,1.0);glVertex3f(-50.f,  50.f, -50.f);
            glTexCoord2d(1.0,1.0);glVertex3f(-50.f,  50.f,  50.f);
            glTexCoord2d(1.0,0.0);glVertex3f(-50.f, -50.f,  50.f);

		
            glTexCoord2d(0.0,0.0);glVertex3f(50.f, -50.f, -50.f);
            glTexCoord2d(0.0,1.0);glVertex3f(50.f,  50.f, -50.f);
            glTexCoord2d(1.0,1.0);glVertex3f(50.f,  50.f,  50.f);
            glTexCoord2d(1.0,0.0);glVertex3f(50.f, -50.f,  50.f);
		glEnd();


		glDisable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);//draw some squares
			glColor3f(1,1,0);
            glVertex3f(-50.f, -50.f,  50.f);
            glVertex3f(-50.f, -50.f, -50.f);
            glVertex3f( 50.f, -50.f, -50.f);
            glVertex3f( 50.f, -50.f,  50.f);

			glColor3f(1,0,0);
            glVertex3f(-50.f, 50.f,  50.f);
            glVertex3f(-50.f, 50.f, -50.f);
            glVertex3f( 50.f, 50.f, -50.f);
            glVertex3f( 50.f, 50.f,  50.f);

        glEnd();

        // Finally, display rendered frame on screen
        App.display();
    }

    return EXIT_SUCCESS;
}
