//////////////////////////////////////////////////////////// 
// Headers 
//////////////////////////////////////////////////////////// 
#include "stdafx.h" 

//FMOD includes
#pragma comment(lib,"fmodex_vc.lib") 
#include "fmod.hpp"

//sfml & openGL Includes
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
#include "SFML/System/String.hpp" 


//standard c++ includes
#include <iostream> 
#include <string>
  
  
  
  
//////////////////////////////////////////////////////////// 
/// Entry point of application 
//////////////////////////////////////////////////////////// 
int main() 
{ 
    // Create the main window 
    sf::RenderWindow App(sf::VideoMode(800, 600, 32), "SFML OpenGL"); 
  
	// Create a clock for measuring time elapsed     
	sf::Clock Clock; 
   
	//setup FMOD
	FMOD::System *FMODsys; //the FMOD sytem
	FMOD_RESULT result;result = FMOD::System_Create(&FMODsys);		// Create the main system object.
	if (result != FMOD_OK)
	{
		std::cout << "FMOD error! (%d) %s\n" <<result;// << FMOD_ErrorString(result);
		exit(-1);
	}

	result = FMODsys->init(100, FMOD_INIT_NORMAL, 0);	// Initialize FMOD.
		
	if (result != FMOD_OK)
	{
		std::cout << "FMOD error! (%d) %s\n" << result;// << FMOD_ErrorString(result);
		exit(-1);
	}
	
	// create and load a "sound",
	FMOD::Sound *sound;
	result = FMODsys->createSound("../Sleep Away.mp3", FMOD_LOOP_NORMAL|FMOD_3D, 0, &sound);		
	// FMOD_DEFAULT uses the defaults.  These are the same as FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE.
	
	FMOD::Channel *channel=0;
	float volume=0.9f;

	

   //load & create sprites
	sf::Image personImage;
	if (!personImage.loadFromFile("../person.png"))
	{
		std::cout << "Error loading image\n" ;
	}
	sf::Texture texture;
	texture.loadFromImage(personImage);
	sf::Sprite listenerSprite;
	listenerSprite.setTexture(texture);
	listenerSprite.setPosition(100,300);
	listenerSprite.setOrigin(53,23); //set origin at her head
	sf::Vector2i velocity(0,0);

	sf::Image speakerImage;
	if (!speakerImage.loadFromFile("../speaker.png"))
	{
		std::cout << "Error loading image\n" ;
	}
	sf::Texture stexture;
	stexture.loadFromImage(speakerImage);
	sf::Sprite sourceSprite;
	sourceSprite.setTexture(stexture);
	sourceSprite.setPosition(200,500);
	sourceSprite.setOrigin(stexture.getSize().x/2,stexture.getSize().y/2); //set origin at center
	
	sf::Image seaImage;
	if (!seaImage.loadFromFile("../sea.png"))
	{
		std::cout << "Error loading image\n" ;
	}
	sf::Texture seatexture;
	seatexture.loadFromImage(seaImage);
	sf::Sprite seaSprite;
	seaSprite.setTexture(seatexture);
	seaSprite.setPosition(350,500);
	seaSprite.setOrigin(seatexture.getSize().x/2,seatexture.getSize().y/2); //set origin at center
	

	//load a font for later

	// Declare a new font
	sf::Font font;
 
	 // Load it from a file
	 if (!font.loadFromFile("../sansation.ttf"))
	 {
		 std::cout << "Error loading font\n" ;
	 }


	bool soundOn=false;
	bool doppler=false;
	bool reverbOn=false;
	bool spatialSound=false;


	//set up the reverb
	FMOD_REVERB_PROPERTIES prop1 = FMOD_PRESET_OFF;
	FMODsys->setReverbAmbientProperties(&prop1);
	//FMOD_REVERB_PROPERTIES prop2 = {  0,  22, 1.00f, 
	//									-0,  0,  0,  //room 
	//										1.49f,  0.10f, 1.0f, //decay  
	//										-449, 0.15f,//reflections  
	//										-5700, 0.011f, //reverb
	//										1.18f, 0.348f, //modulation
	//										5000.0f, 250.0f, //reference
	//										100.0f, 100.0f, //diffusion & density
	//										0x3f };

	FMOD::Reverb *reverb;
	result = FMODsys->createReverb(&reverb);
	FMOD_REVERB_PROPERTIES prop2 = FMOD_PRESET_UNDERWATER  ;
	reverb->setProperties(&prop2);
				
	FMOD_VECTOR pos = {seaSprite.getPosition().x, 0.0f,seaSprite.getPosition().y };
	float mindist = 100.0f; 
	float maxdist = 150.0f;
	reverb->set3DAttributes(&pos, mindist, maxdist);

	

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
  
            
            if (Event.type == sf::Event::KeyPressed)
			{ //process keyboard events
			
				// Escape key : exit 
				if (Event.key.code == sf::Keyboard::Escape)
					 App.close(); 
            
				//play sound
				if (Event.key.code == sf::Keyboard::S){ 
					if(!channel){
						FMODsys->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
						result = channel->setVolume(volume);	
						//need this for sound fall off
						channel->set3DMinMaxDistance(50,10000);
						
					}
					
					
					
					soundOn=!soundOn;
					channel->setPaused(!soundOn);
				}

				//increase/decrease  volumne
				if (Event.key.code == sf::Keyboard::Equal){ 
					volume+=0.1f;
					result = channel->setVolume(volume);	
				}

				if (Event.key.code == sf::Keyboard::Dash){ 
				   volume-=0.1f;
				   result = channel->setVolume(volume);	
				}

				//set-up reverb
				if ((Event.key.code == sf::Keyboard::R)){ 
					reverbOn=!reverbOn;
				}


				if (Event.key.code == sf::Keyboard::A){ 
					spatialSound=!spatialSound;
				}

				if (Event.key.code == sf::Keyboard::D){ 
					doppler=!doppler;
				}
				
			}
			
        } 

		// Move the listener sprite
		float ElapsedTime = Clock.getElapsedTime().asMicroseconds()/1000000.0;
		Clock.restart();
		velocity=sf::Vector2i(0,0);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))			velocity.x=-50;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))		velocity.x=50;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))			velocity.y=-50;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))			velocity.y=+50;
   
		listenerSprite.move(velocity.x* ElapsedTime, velocity.y*ElapsedTime);



		FMOD_VECTOR  listenervel  = { 0, 0.0f, 0 };
		if(doppler){
			listenervel.x=velocity.x;
			listenervel.z=velocity.y;

		}
		
		if(spatialSound){
			//update position & velocity of listener
			//position of listener needed for spatial & reverb effects
			//velocity of listener needed for dopper effects
			FMOD_VECTOR  listenerpos  = { listenerSprite.getPosition().x, 0.0f, listenerSprite.getPosition().y };
			FMODsys->set3DListenerAttributes(0, &listenerpos, &listenervel, 0, 0);
		
			//update position of sound
			if(channel){
				FMOD_VECTOR  sourcePos  = { sourceSprite.getPosition().x, 0.0f, sourceSprite.getPosition().y };
				channel->set3DAttributes(&sourcePos,0);
				
			}
		}

		else{
			FMOD_VECTOR zero={0,0,0};
			channel->set3DAttributes(&zero,0);
			FMODsys->set3DListenerAttributes(0, &zero,&zero, 0, 0);
		}
		reverb->setActive(reverbOn);

		App.clear();
		
		App.draw(seaSprite);
		App.draw(sourceSprite);
		App.draw(listenerSprite);
        
		sf::Text atext;
		atext.setFont(font);
		atext.setCharacterSize(20);
		atext.setStyle(sf::Text::Bold);
		atext.setColor(sf::Color::Red);
		int textPos=0;
		
		sf::String on="[on]";
		sf::String off="[off]";

		atext.setString(sf::String("press 'S' to enable Sound ")+=soundOn?on:off);
		App.draw(atext);
		

		atext.setString(sf::String("press 'D' to enable Doppler ")+=doppler?on:off);
		atext.setPosition(0,textPos+=22);
		App.draw(atext);

		

		atext.setString(sf::String("press 'R' to enable Reverb ")+=reverbOn?on:off);
		atext.setPosition(0,textPos+=22);
		App.draw(atext);



		atext.setString(sf::String("press 'A' to enable spatial sounds ")+=spatialSound?on:off);
		atext.setPosition(0,textPos+=22);
		App.draw(atext);




		 // Draw it
		
		



		//update FMD
		FMODsys->update();

          
  
  
        // Finally, display rendered frame on screen 
        App.display(); 
		
    } 
  
    return EXIT_SUCCESS; 
} 