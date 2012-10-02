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


#pragma comment(lib,"fmodex_vc.lib") 
#include "fmod.hpp"

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
	result = FMODsys->createSound("C:/Program Files (x86)/FMOD SoundSystem/FMOD Programmers API Windows/examples/media/wave.mp3", FMOD_LOOP_NORMAL|FMOD_3D, 0, &sound);		// FMOD_DEFAULT uses the defaults.  These are the same as FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE.
	
	FMOD::Channel *channel=0;
	float volume=0.5f;

	FMOD::Reverb	 *reverb;

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
	listenerSprite.setPosition(100,100);
	listenerSprite.setOrigin(53,23); //set origin at her head

	sf::Image speakerImage;
	if (!speakerImage.loadFromFile("../speaker.png"))
	{
		std::cout << "Error loading image\n" ;
	}
	sf::Texture stexture;
	stexture.loadFromImage(speakerImage);
	sf::Sprite sourceSprite;
	sourceSprite.setTexture(stexture);
	sourceSprite.setPosition(200,200);
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
	seaSprite.setPosition(400,200);
	seaSprite.setOrigin(seatexture.getSize().x/2,seatexture.getSize().y/2); //set origin at center
	

	bool attenuation=true;
	sf::Vector2i vel=sf::Vector2i(0,0);
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
				   FMODsys->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
				   result = channel->setVolume(volume);	
				    channel->set3DMinMaxDistance(10,10000);

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
					FMOD_REVERB_PROPERTIES prop1 = FMOD_PRESET_OFF;
					FMODsys->setReverbAmbientProperties(&prop1);
				
					result = FMODsys->createReverb(&reverb);
					FMOD_REVERB_PROPERTIES prop2 = FMOD_PRESET_UNDERWATER;
					reverb->setProperties(&prop2);
				
					FMOD_VECTOR pos = {seaSprite.getPosition().x, 0.0f,seaSprite.getPosition().y };
					float mindist = 100.0f; 
					float maxdist = 150.0f;
					reverb->set3DAttributes(&pos, mindist, maxdist);


					//As the 3D reverb uses the position of the listener in its weighting calculation, we also need to ensure that the location of the listener is set using System::set3dListenerAtrributes. 

					


				}


				if (Event.key.code == sf::Keyboard::A){ 
					attenuation=true;
				}

				if (Event.key.code == sf::Keyboard::M){ 
					=;
				}
				
			}
			
        } 

		// Move the sprite
		float ElapsedTime = Clock.getElapsedTime().asMicroseconds()/1000000.0;
		Clock.restart();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))		listenerSprite.move(-100 * ElapsedTime, 0);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))		listenerSprite.move( 100 * ElapsedTime, 0);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))			listenerSprite.move(0, -100 * ElapsedTime);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))		listenerSprite.move(0,  100 * ElapsedTime);
   
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))		listenerSprite.move(0,  100 * ElapsedTime);

		if(attenuation){
		
		}

		if(moving){
		
		}
		

		FMOD_VECTOR  listenerpos  = { listenerSprite.getPosition().x, 0.0f, listenerSprite.getPosition().y };
		FMOD_VECTOR  listenervel  = { 100, 0.0f, 0 };

		FMODsys->set3DListenerAttributes(0, &listenerpos, &listenervel, 0, 0);
		
		if(channel){
			FMOD_VECTOR  sourcePos  = { sourceSprite.getPosition().x, 0.0f, sourceSprite.getPosition().y };
			channel->set3DAttributes(&sourcePos,0);
		}



		App.clear();
		
		App.draw(seaSprite);
		App.draw(sourceSprite);
		App.draw(listenerSprite);
        
		//update FMD
		FMODsys->update();

          
  
  
        // Finally, display rendered frame on screen 
        App.display(); 
		
    } 
  
    return EXIT_SUCCESS; 
} 