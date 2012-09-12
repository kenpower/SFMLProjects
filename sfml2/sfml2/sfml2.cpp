
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "stdafx.h"
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"sfml-graphics-d.lib")
#pragma comment(lib,"sfml-audio-d.lib")
#pragma comment(lib,"sfml-system-d.lib")
#pragma comment(lib,"sfml-window-d.lib")
#pragma comment(lib,"sfml-network-d.lib")
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#include <GL/glew.h>
#include <SFML/Graphics.hpp>
//#include <SFML/Graphics/Image.hpp>



////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code

///
////////////////////////////////////////////////////////////
int main()
{
    // Create the main window
    sf::RenderWindow App(sf::VideoMode(800, 600, 32), "SFML OpenGL");

	//initialize GLEW
	GLenum err=glewInit();
	if(GLEW_OK!=err){
		exit(1);
	}

    // Create a clock for measuring time elapsed
    sf::Clock Clock;

    // Set color and depth clear value
    glClearDepth(1.f);
    glClearColor(0.f, 1.f, 0.f, 0.f);

    // Enable Z-buffer read and write
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    // Setup a perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.f, 1.f, 1.f, 300.0f);

	const float vertexPositions[] = {
		75, 75, 0, 1,
		75, -75, 0, 1,
		-75, -75, 0, 1,
	};

	GLuint positionBufferObject;
	glGenBuffers(1, &positionBufferObject);
    
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glVertexPointer( 4, GL_FLOAT, 0, (char *) NULL );
	glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Start game loop
    bool rotate=true;

	sf::Image Image1(100, 100, sf::Color(0, 255, 0));
	sf::Sprite Sprite;
	Sprite.SetColor(sf::Color(0, 255, 255, 128));
	Sprite.SetX(200.f);
	Sprite.SetY(100.f);
	Sprite.Scale(100/Sprite.GetSize().x,100/Sprite.GetSize().y);
	Sprite.SetPosition(200.f, 100.f);
	Sprite.SetRotation(30.f);
	//Sprite.SetCenter(0, 0);
	//Sprite.SetScaleX(2.f);
	//Sprite.SetScaleY(0.5f);
	//Sprite.SetScale(2.f, 0.5f);
	////Sprite.SetBlendMode(sf::Blend::Multiply);

	Sprite.Move(10, 5);
	Sprite.Rotate(90);
	//Sprite.Scale(1.5f, 1.5f);

	App.PreserveOpenGLStates(true);
	while (App.IsOpened())
    {
        // Process events
        sf::Event Event;
        while (App.GetEvent(Event))
        {
            // Close window : exit
            if (Event.Type == sf::Event::Closed)
                App.Close();

            // Escape key : exit
            if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape))
                App.Close();

            // Resize event : adjust viewport
            if (Event.Type == sf::Event::Resized)
                glViewport(0, 0, Event.Size.Width, Event.Size.Height);

		    if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::A)){
				rotate=!rotate;
			}
 
		}

        // Set the active window before using OpenGL commands
        // It's useless here because active window is always the same,
        // but don't forget it if you use multiple windows or controls
        
		
		App.SetActive();
        
        // Clear color and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Apply some transformations
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.f, 0.f, -200.f);
		if(rotate){
			glRotatef(Clock.GetElapsedTime() * 50, 1.f, 0.f, 0.f);
			glRotatef(Clock.GetElapsedTime() * 30, 0.f, 1.f, 0.f);
			glRotatef(Clock.GetElapsedTime() * 90, 0.f, 0.f, 1.f);
		}
		/* Draw a cube
		//
        glBegin(GL_QUADS);
			glColor3i(0,1,1);
            glVertex3f(-50.f, -50.f, -50.f);
            glVertex3f(-50.f,  50.f, -50.f);
            glVertex3f( 50.f,  50.f, -50.f);
            glVertex3f( 50.f, -50.f, -50.f);
			glColor3f(0,0,1);

            glVertex3f(-50.f, -50.f, 50.f);
            glVertex3f(-50.f,  50.f, 50.f);
            glVertex3f( 50.f,  50.f, 50.f);
            glVertex3f( 50.f, -50.f, 50.f);
			glColor3f(1,0,1);

            glVertex3f(-50.f, -50.f, -50.f);
            glVertex3f(-50.f,  50.f, -50.f);
            glVertex3f(-50.f,  50.f,  50.f);
            glVertex3f(-50.f, -50.f,  50.f);

			glColor3f(0,1,0);

            glVertex3f(50.f, -50.f, -50.f);
            glVertex3f(50.f,  50.f, -50.f);
            glVertex3f(50.f,  50.f,  50.f);
            glVertex3f(50.f, -50.f,  50.f);

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
		*/
		
		glBindBuffer(GL_ARRAY_BUFFER,positionBufferObject);		// select first VAO
		glEnableClientState(GL_VERTEX_ARRAY);

		//glEnableVertexAttribArray(0);
		//glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, 1);	// draw first object

		App.Draw(Sprite);

        // Finally, display rendered frame on screen
        App.Display();
    }

    return EXIT_SUCCESS;
}


