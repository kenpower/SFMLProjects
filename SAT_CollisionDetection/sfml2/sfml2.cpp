
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
#include <sstream>


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

	void GetNormals(sf::Vector2f* v){
		
		for(int i=0;i<3;i++)
			v[i]=Vector::getNormal(verts[i],verts[(i+1)%3]);
		
	}

	
	
};

class Collider{
public:
	static void CheckPointsOutsideWindow(Triangle* tri,int left,int right, int bottom, int top){
		sf::Vector2f verts[3];
		tri->GetVerts(verts);
		for(int i=0;i<3;i++){
			if(verts[i].x<left)		tri->CollideWithEdge(leftEdge);
			if(verts[i].x>right)	tri->CollideWithEdge(rightEdge);
			if(verts[i].y<bottom)	tri->CollideWithEdge(bottomEdge);
			if(verts[i].y>top)		tri->CollideWithEdge(topEdge);
		}
	}

	static bool CheckForCollisionSAT(Triangle* t1,Triangle* t2){
		sf::Vector2f verts1[3];
		sf::Vector2f verts2[3];
		t1->GetVerts(verts1);
		t2->GetVerts(verts2);	

		sf::Vector2f normals1[3];
		sf::Vector2f normals2[3];
		t1->GetNormals(normals1);
		t2->GetNormals(normals2);

		

		for(int i=0;i<3;i++){
			bool sep;
			sep=Separated(normals1[i],verts1,verts2);
			if (sep) return false;//no collision
			sep=Separated(normals2[i],verts1,verts2);
			if (sep) return false;//no collision
		}

		return true;

	}

	static void Bounce(Triangle* t1,Triangle* t2){
		sf::Vector2f displacement=t1->getPos()-t2->getPos();
		sf::Vector2f closing=t1->getVel()-t2->getVel();

		
		
		if(Vector::dot(displacement,closing)>=0) return;

		
		displacement/=Vector::length(displacement); //normalise

		sf::Vector2f b;
		b=Vector::dot(t1->getVel(),displacement)*displacement;
		b=t1->getVel()-b*2.f;
		t1->setVel(b);
		b=Vector::dot(t2->getVel(),displacement)*displacement;
		t2->setVel(t2->getVel()-b*2.f);

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
		

		if(max1<min2 || max2 <min1) //check for overlap
			return true;

		return false;
	}
	static void CollideAllInVector(std::vector<Triangle*>& tris, bool circleCollision){
			std::vector<Triangle*>::iterator outerIt;	
			std::vector<Triangle*>::iterator innerIt;	
			for(outerIt=tris.begin();outerIt!=tris.end();outerIt++){ 
				for(innerIt=outerIt+1;innerIt!=tris.end();innerIt++){
					//circle Collision
					if(circleCollision){
						if(Vector::length((*outerIt)->getPos()-(*innerIt)->getPos())>(*outerIt)->getSize()+(*innerIt)->getSize())
							continue;
					}
					bool collide=Collider::CheckForCollisionSAT(*outerIt,*innerIt);
					if(collide){
						Collider::Bounce(*outerIt,*innerIt);
					}
				}
			}
	}
};

class FPS
{ 
public:
	/// @brief Constructor with initialization.
	///
	FPS() : mFrame(0), mFps(0) {}

	/// @brief Update the frame count.
	/// 


	/// @brief Get the current FPS count.
	/// @return FPS count.
	const unsigned int getFPS() const { return mFps; }

private:
	unsigned int mFrame;
	unsigned int mFps;
	sf::Clock mClock;

public:
	void update()
	{
		if(mClock.getElapsedTime().asSeconds() >= 1.f)
		{
			mFps = mFrame;
			mFrame = 0;
			mClock.restart();
		}
 
		++mFrame;
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
    sf::RectangleShape viewport(sf::Vector2f(800,600));
	sf::RenderWindow window(sf::VideoMode(viewport.getSize().x,viewport.getSize().y,32), "SFML Window");
	
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(90.f, 1.f, 1.f, 300.0f);
	glOrtho(0,viewport.getSize().x,0,viewport.getSize().y,0,1);



    // Create a clock for measuring time elapsed
    sf::Clock Clock;
	//window.setFramerateLimit(60);	


    //// Set color and depth clear value
    glClearDepth(1.f);
    glClearColor(0.3f, 0.3f, 0.3f, 0.f);

    //// Enable Z-buffer read and write
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

	const int NUM_TRIS=500;
	std::vector<Triangle*> tris;
	const int MAX_VEL=10;
	for(int i=0;i<NUM_TRIS;i++){
		Triangle* tri=new Triangle();
		tri->setPos(sf::Vector2f(rand()%400+200,rand()%300+150));
		tri->setVel(sf::Vector2f(rand()%MAX_VEL-MAX_VEL/2,rand()%MAX_VEL-MAX_VEL/2));
		tri->setAngVel(rand()%20-10);
		tri->setSize(rand()%15+2);
		tri->setColour(sf::Vector3f( (double)rand()/ RAND_MAX, (double)rand() / RAND_MAX, (double)rand() / RAND_MAX ));
		tris.push_back(tri);
	}


	bool broadPhase=false;
	bool collisions=false;
	bool octtree=false;

	FPS fps;
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
				viewport=sf::RectangleShape(sf::Vector2f((float)Event.size.width,(float)Event.size.height));
				glOrtho(0,viewport.getSize().x,0,viewport.getSize().y,0,1);

			}

			 // 
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Space))
                broadPhase=!broadPhase;
            
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::C))
                collisions=!collisions;

			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::O))
                octtree=!octtree;

		}



		typedef std::vector<Triangle*> Vec;
		Vec a,b,c,d;

       
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		std::vector<Triangle*>::iterator triIt;
		for(triIt=tris.begin();triIt!=tris.end();triIt++){
			(*triIt)->Update();
			
			(*triIt)->Draw();

			Collider::CheckPointsOutsideWindow(*triIt,0,viewport.getSize().x,0,viewport.getSize().y);

			int midx=viewport.getSize().x/2;
			int midy=viewport.getSize().y/2;
			if(octtree){ //subdivide all tris into quads
				int x=(*triIt)->getPos().x;
				int y=(*triIt)->getPos().y;

				if(x<midx){
					if(y<midy) a.push_back(*triIt);
				
					else c.push_back(*triIt);
				}
				else{
					if(y<midy) b.push_back(*triIt);
				
					else d.push_back(*triIt);
				}
			}

	   }


		if(collisions){
			if(octtree){
				Collider::CollideAllInVector(a,broadPhase);
				Collider::CollideAllInVector(b,broadPhase);
				Collider::CollideAllInVector(c,broadPhase);
				Collider::CollideAllInVector(d,broadPhase);
			}
			else{
				Collider::CollideAllInVector(tris,broadPhase);
			}

		}

        // Finally, display rendered frame on screen
	   fps.update();
	   std::ostringstream ss;
	   ss << fps.getFPS();
		
	   window.setTitle(ss.str());
       window.display();
    }

    return EXIT_SUCCESS;
}



