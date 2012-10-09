
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

class Vector{
public:
	static float dot(sf::Vector2f a,sf::Vector2f b){
		return a.x*b.x+a.y*b.y;
	}

	static float length(sf::Vector2f a){
		return sqrt(a.x*a.x+a.y*a.y);
	}

	static sf::Vector2f getNormal(sf::Vector2f a,sf::Vector2f b){
		sf::Vector2f n;
		n=a-b;
		
		n/=Vector::length(n);//normalise
		float x=n.x;
		n.x=n.y;
		n.y=-x;
		return n;
	}
};

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

	void setPos(sf::Vector2f p){pos=p;}
	sf::Vector2f getPos(){return pos;}
	void setVel(sf::Vector2f v){vel=v;}
	
	sf::Vector2f  getVel(){return vel;}
	void setAngVel(float aVel){angVel=aVel;}
	void setSize(float s){size=s; initVerts();}
	float getSize(){return size;}
	void setColour(sf::Vector3f c){col=c;}



	void Draw(){
		glBegin(GL_POINTS);
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

	void GetNormals(sf::Vector2f* v){
		
		for(int i=0;i<3;i++)
			v[i]=Vector::getNormal(verts[i],verts[(i+1)%3]);
		
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

	static bool CheckForCollisionSAT(Triangle& t1,Triangle& t2){
		sf::Vector2f verts1[3];
		sf::Vector2f verts2[3];
		t1.GetVerts(verts1);
		t2.GetVerts(verts2);	

		sf::Vector2f normals1[3];
		sf::Vector2f normals2[3];
		t1.GetNormals(normals1);
		t2.GetNormals(normals2);

		

		for(int i=0;i<3;i++){
			bool sep;
			sep=Separated(normals1[i],verts1,verts2);
			if (sep) return false;//no collision
			sep=Separated(normals2[i],verts1,verts2);
			if (sep) return false;//no collision
		}

		//debug code
		//if(Vector::length(t1.getPos()-t2.getPos())>t1.getSize()+t2.getSize()){
		//	for(int i=0;i<3;i++){
		//	bool sep;
		//	sep=Separated(normals1[i],verts1,verts2);
		//	if (sep) return false;//no collision
		//	sep=Separated(normals2[i],verts1,verts2);
		//	if (sep) return false;//no collision
		//}

		//}

		return true;

	}

	static void Bounce(Triangle& t1,Triangle& t2){
		sf::Vector2f displacement=t1.getPos()-t2.getPos();
		sf::Vector2f closing=t1.getVel()-t2.getVel();

		
		
		if(Vector::dot(displacement,closing)>=0) return;

		
		displacement/=Vector::length(displacement); //normalise

		sf::Vector2f b;
		b=Vector::dot(t1.getVel(),displacement)*displacement;
		b=t1.getVel()-b*2.f;
		t1.setVel(b);
		b=Vector::dot(t2.getVel(),displacement)*displacement;
		t2.setVel(t2.getVel()-b*2.f);

	}

	bool static Separated(sf::Vector2f normal,sf::Vector2f verts1[],sf::Vector2f verts2[]){
		float min1=FLT_MAX;
		float min2=FLT_MAX;
		float max1=-FLT_MAX;
		float max2=-FLT_MAX;
		for(int i=0;i<3;i++){//project t2 on to 1's normals
				float d=Vector::dot(normal,verts2[i]);
				if(d>max2) max2=d;
				if(d<min2) min2=d;
			}
			for(int i=0;i<3;i++){//project t1 on to 1's normals
				float d=Vector::dot(normal,verts1[i]);
				if(d>max1) max1=d;
				if(d<min1) min1=d;
			}
		

		if(max1<min2 || max2 <min1)
			return true;


		//if(max1<min2)
		//	return true;
		return false;
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
	window.setFramerateLimit(60);	


    //// Set color and depth clear value
    glClearDepth(1.f);
    glClearColor(0.3f, 0.3f, 0.3f, 0.f);

    //// Enable Z-buffer read and write
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

	const int NUM_TRIS=500;
	Triangle tri[NUM_TRIS];
	const int MAX_VEL=10;
	for(int i=0;i<NUM_TRIS;i++){
		tri[i].setPos(sf::Vector2f(rand()%400+200,rand()%300+150));
		tri[i].setVel(sf::Vector2f(rand()%MAX_VEL-MAX_VEL/2,rand()%MAX_VEL-MAX_VEL/2));
		tri[i].setAngVel(rand()%20-10);
		tri[i].setSize(rand()%20+5);
		tri[i].setColour(sf::Vector3f( (double)rand()/ RAND_MAX, (double)rand() / RAND_MAX, (double)rand() / RAND_MAX ));
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

			for(int j=i;j<NUM_TRIS;j++){
				if(Vector::length(tri[i].getPos()-tri[j].getPos())>tri[i].getSize()+tri[j].getSize())
					continue;
				bool collide=Collider::CheckForCollisionSAT(tri[i],tri[j]);
				if(collide){
					Collider::Bounce(tri[i],tri[j]);
				}
			}
	   }

        // Finally, display rendered frame on screen
        window.display();
    }

    return EXIT_SUCCESS;
}


