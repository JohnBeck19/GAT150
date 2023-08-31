#include "Input/InputSystem.h"
#include "Player.h"
#include "Enemy.h"
#include "SpaceGame.h"
#include "Core/Core.h"
#include "Audio/AudioSystem.h"
#include"Renderer/Renderer.h"
#include "Renderer/ModelManager.h"
#include "Renderer/Font.h"
#include "Renderer/Text.h"
#include "Renderer/Texture.h"

#include "Renderer/Particle.h"
#include "Renderer/ParticleSystem.h"
#include "Framework/Framework.h"
#include "Framework/Emitter.h"

#include "Physics/PhysicsSystem.h"

#include <iostream>
#include <chrono>
#include <vector>
#include <thread>
#include <array>
#include <map>


using namespace std;



int main(int argc, char* argv[])
{


	meow::MemoryTracker::Initialize();
	meow::seedRandom((unsigned int)time(nullptr));
	meow::setFilePath("assets");
	

	//input initialization
	meow::g_inputSystem.Initialize();

	//physiscs system
	meow::PhysicsSystem::Instance().Initialize();




	//renderer initilization 
	
	meow::g_renderer.Initialize();
	meow::g_renderer.CreateWindow("SUPER ULTIMATE SPACE GAME", 900, 700);
	
	//tranform initilization 
	
	//audio initilization
	meow::g_audioSystem.Initialize();

	
	unique_ptr<SpaceGame> game = make_unique<SpaceGame>();
	game->Initialize();


	
	//game loop
	bool quit = false;
	while (!quit) {
		meow::g_Time.tick();
		meow::g_inputSystem.Update();

		if (meow::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE) && !meow::g_inputSystem.GetPreviousKeyDown(SDL_SCANCODE_SPACE))
		{
			meow::g_audioSystem.PlayOneShot("audio/Laser");
		}
		if (meow::g_inputSystem.GetKeyDown(SDL_SCANCODE_ESCAPE))
		{
			quit = true;
		}

		//updates
		game->Update(meow::g_Time.GetDeltaTime());
		meow::g_particleSystem.Update(meow::g_Time.GetDeltaTime());
		
		meow::PhysicsSystem::Instance().Update(meow::g_Time.GetDeltaTime());

		meow::g_renderer.SetColor(0,0,0,0);
		meow::g_renderer.BeginFrame();
		meow::g_renderer.SetColor(meow::random(256), meow::random(256), meow::random(256), meow::random(256));

		//draws
		
 		game->Draw(meow::g_renderer);
		meow::g_particleSystem.Draw(meow::g_renderer);

		//draw
		meow::g_renderer.EndFrame();

		this_thread::sleep_for(chrono::milliseconds(10));
	}
	return 0;
}

