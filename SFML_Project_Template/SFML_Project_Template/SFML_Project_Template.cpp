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
  
  
 //comment
  
//////////////////////////////////////////////////////////// 
/// Entry point of application 
//////////////////////////////////////////////////////////// 
int main() 
{ 
    // Create the main window 
    sf::RenderWindow App(sf::VideoMode(800, 600, 32), "SFML OpenGL"); 
  
	// Create a clock for measuring time elapsed     
	sf::Clock Clock; 
     
    //prepare OpenGL surface for HSR 
    glClearDepth(1.f); 
    glClearColor(0.3f, 0.3f, 0.3f, 0.f); //background colour
    glEnable(GL_DEPTH_TEST); 
    glDepthMask(GL_TRUE); 
  
    //// Setup a perspective projection & Camera position 
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity(); 
    
	//set up a 3D Perspective View volume
	//gluPerspective(90.f, 1.f, 1.f, 300.0f);//fov, aspect, zNear, zFar 

	//set up a  orthographic projection same size as window
	//this mease the vertex coordinates are in pixel space
	glOrtho(0,800,0,600,0,1); // use pixel coordinates
  
  
  
      
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
            
   
        } 
          
        //Prepare for drawing 
        // Clear color and depth buffer 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
  
        // Apply some transformations 
        glMatrixMode(GL_MODELVIEW); 
        glLoadIdentity(); 
		
		double angle=Clock.getElapsedTime().asMilliseconds();
		glTranslated(+400,+300,0); //shift to original position
		glRotated(angle/10, 0, 0, 1); // rotate
		glTranslated(-400,-300,0);// shift centre to origin
		
       
          
           

        glBegin(GL_TRIANGLES);//draw a Triangle
            glColor3d(0,1,1);//cyan
			glVertex2d(100,100);
			glColor3d(1,1,0);//yellow
			glVertex2d(550,50);
            glColor3d(1,0,1);//magenta
			glVertex2d(700,500);

        glEnd(); 
  
        // Finally, display rendered frame on screen 
        App.display(); 
    } 
  
    return EXIT_SUCCESS; 
} 