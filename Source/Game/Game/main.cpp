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
	//meow::Factory::Instance().Register<meow::SpriteComponent>("SpriteComponent");


	meow::MemoryTracker::Initialize();
	meow::seedRandom((unsigned int)time(nullptr));
	meow::setFilePath("assets");
	

	//input initialization
	meow::g_inputSystem.Initialize();

	//physiscs system
	meow::PhysicsSystem::Instance().Initialize();


	//seed randomization
	//meow::seedRandom((unsigned int)time(nullptr));
	//file path
	//meow::setFilePath("assets");

	//renderer initilization 
	
	meow::g_renderer.Initialize();
	meow::g_renderer.CreateWindow("MEEEEOOOOWWWWW", 900, 700);
	
	//tranform initilization 
	meow::Transform transform({450,350},0,3);
	float speed = 500;
	constexpr float turnRate = meow::DegToRad(180);
	
	//audio initilization
	meow::g_audioSystem.Initialize();

	
	unique_ptr<SpaceGame> game = make_unique<SpaceGame>();
	game->Initialize();


	//scene.Add(std::move(emitter));
	


	//vector<meow::Vector2> points;
	//vector<Star>stars;
	//for (int i = 0; i < 1000; i++)
	//{
//		meow::Vector2 pos(meow::Vector2(meow::randomf(renderer.GetWidth()), meow::randomf(renderer.GetHeight())));
//		meow::Vector2 vel(meow::randomf(1.0f, 10.0f));

		//stars.push_back(Star(pos,vel));
	//}
	// 
	
	
	//game loop
	bool quit = false;
	while (!quit) {
		meow::g_Time.tick();
		meow::g_inputSystem.Update();

		if (meow::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE) && !meow::g_inputSystem.GetPreviousKeyDown(SDL_SCANCODE_SPACE))
		{
			meow::g_audioSystem.PlayOneShot("Laser");
		}
		if (meow::g_inputSystem.GetKeyDown(SDL_SCANCODE_ESCAPE))
		{
			quit = true;
		}

		//updates
		game->Update(meow::g_Time.GetDeltaTime());
		meow::g_particleSystem.Update(meow::g_Time.GetDeltaTime());
		

		meow::g_renderer.SetColor(0,0,0,0);
		meow::g_renderer.BeginFrame();
		meow::g_renderer.SetColor(meow::random(256), meow::random(256), meow::random(256), meow::random(256));

		//draws
		
		game->Draw(meow::g_renderer);
		meow::g_particleSystem.Draw(meow::g_renderer);
		//text
		//text->Draw(meow::g_renderer, 450, 100);
		
		
		//draw
		//meow::Vector2 vel(1.0f, 0.1f);

		//for (auto& star : stars) {
			//star.Update(renderer.GetWidth(),renderer.GetHeight());

			//renderer.SetColor(meow::random(256), meow::random(256), meow::random(256), meow::random(256));
			//renderer.DrawPoint(star.m_pos.x, star.m_pos.y);
		
		//}


		//for (int i = 0; i < 100; i++)
		//{
		//	meow::Vector2 pos(meow::randomf(renderer.GetWidth()), meow::randomf(renderer.GetHeight()));

		//	renderer.SetColor(meow::random(256), meow::random(256), meow::random(256), meow::random(256));
		//	renderer.DrawPoint(pos.x, pos.y);
		//	//renderer.DrawLine(pos.x, pos.y,10.0,10.0);
		//	//renderer.DrawPoint(meow::random(renderer.GetWidth()), meow::random(renderer.GetHeight()));
		//	//renderer.DrawLine(meow::random(renderer.GetWidth()), meow::random(renderer.GetHeight()),
		//		//meow::random(renderer.GetWidth()), meow::random(renderer.GetHeight()));
		//}

		//draw
		meow::g_renderer.EndFrame();

		this_thread::sleep_for(chrono::milliseconds(10));
	}
	return 0;
}
/*

class Star {

public:
	Star(const meow::Vector2& pos, const meow::Vector2& vel) : m_pos{ pos }, m_vel{ vel } {}

	void Update(int width, int height) {
		m_pos += m_vel;

		if (m_pos.x > width) {
			m_pos.x = 0;
		}
		if (m_pos.y > height) {
			m_pos.y = 0;
		}
	}

public:
	meow::Vector2 m_pos;
	meow::Vector2 m_vel;

};
*/
