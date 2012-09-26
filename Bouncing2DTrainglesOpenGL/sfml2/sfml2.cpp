
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

enum Edges{leftEdge,rightEdge,topEdge, bottomEdge};

class Triangle{
	sf::Vector2f verts[3];
	sf::Vector2f vel;
	sf::Vector2f pos;
	sf::Vector3f col;

	float angVel;
	float size;

	
	void initVerts(){
		verts[0]=sf::Vector2f(0,size);
		verts[1]=sf::Vector2f(-size,-size);
		verts[2]=sf::Vector2f(+size,-size);
	}

	public: 

	Triangle(){}
	Triangle(sf::Vector2f p,sf::Vector2f v,float aVel=0,float s=20){
		pos=p;
		size=s;
		vel=v;
		angVel=aVel;
		initVerts();

	}

	void SetPos(sf::Vector2f p){pos=p;}
	void SetVel(sf::Vector2f v){vel=v;}
	void SetAngVel(float aVel){angVel=aVel;}
	void SetSize(float s){size=s; initVerts();}
	void SetColour(sf::Vector3f c){col=c;}



	void Draw(){
		glBegin(GL_TRIANGLES);
		glColor3f(col.x,col.y,col.z);
		for(int i=0;i<3;i++)
			glVertex2f(pos.x+verts[i].x,pos.y+verts[i].y);
		glEnd();
	}

	void Update(){
		pos+=vel;
		sf::Transform t;
		t.rotate(angVel);
		for(int i=0;i<3;i++)
			verts[i]=t.transformPoint(verts[i]);
	
	
	}

	void CollideWithEdge(Edges edge){
		switch(edge){
		case topEdge:
				vel.y=-abs(vel.y);
				break;
		case bottomEdge:
				vel.y=abs(vel.y);
				break;
		case leftEdge:
				vel.x=abs(vel.x);
				break;
		case rightEdge:
				vel.x=-abs(vel.x);
				break;

		}
		return;
	}

	void GetVerts(sf::Vector2f* v){
		for(int i=0;i<3;i++)
			v[i]=pos+verts[i];
	}
	
};

class Collider{
public:
	static void CheckPointsOutsideWindow(Triangle& tri,int left,int right, int bottom, int top){
		sf::Vector2f verts[3];
		tri.GetVerts(verts);
		for(int i=0;i<3;i++){
			if(verts[i].x<left)		tri.CollideWithEdge(leftEdge);
			if(verts[i].x>right)	tri.CollideWithEdge(rightEdge);
			if(verts[i].y<bottom)	tri.CollideWithEdge(bottomEdge);
			if(verts[i].y>top)		tri.CollideWithEdge(topEdge);
		}
	}
};


int main()
{
////////////////////////////////////////////////////////////
/// Entry point of application
///
////////////////////////////////////////////////////////////
    // Create the main window
    //sf::RenderWindow window(sf::VideoMode::getFullscreenModes()[0], "SFML Window", sf::Style::Fullscreen);
    sf::RenderWindow window(sf::VideoMode(800,600,32), "SFML Window");
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(90.f, 1.f, 1.f, 300.0f);
	glOrtho(0,800,0,600,0,1);



    // Create a clock for measuring time elapsed
    sf::Clock Clock;



    //// Set color and depth clear value
    glClearDepth(1.f);
    glClearColor(0.3f, 0.3f, 0.3f, 0.f);

    //// Enable Z-buffer read and write
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

	const int NUM_TRIS=100;
	Triangle tri[NUM_TRIS];
	for(int i=0;i<NUM_TRIS;i++){
		tri[i].SetPos(sf::Vector2f(rand()%400+200,rand()%300+150));
		tri[i].SetVel(sf::Vector2f(rand()%20-10,rand()%20-10));
		tri[i].SetAngVel(rand()%20-10);
		tri[i].SetSize(rand()%20+5);
		tri[i].SetColour(sf::Vector3f( (double)rand()/ RAND_MAX, (double)rand() / RAND_MAX, (double)rand() / RAND_MAX ));
	}




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

            // Resize event : adjust viewport
			if (Event.type == sf::Event::Resized){
				glViewport(0, 0, Event.size.width, Event.size.height);
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				glOrtho(0,Event.size.width,0,Event.size.height,0,1);

			}
 
		}


       glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	   for(int i=0;i<NUM_TRIS;i++){
			tri[i].Update();
			
			tri[i].Draw();

			Collider::CheckPointsOutsideWindow(tri[i],0,800,0,600);
	   }

        // Finally, display rendered frame on screen
        window.display();
    }

    return EXIT_SUCCESS;
}


