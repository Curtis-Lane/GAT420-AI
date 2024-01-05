#include <glm/glm.hpp>
#include <iostream>
#include <memory>

// Framerate cap
#include <thread>
#include <chrono>

#include "GameOfLife.h"
#include "Renderer.h"
#include "Texture.h"

const int FRAMERATE_CAP = 60;

int main(int argc, char* argv[]) {
	glm::ivec2 screen_size{800, 600};
	glm::ivec2 env_size{200, 150};

	int framerate = (FRAMERATE_CAP == 0) ? 0 : (1.0f / FRAMERATE_CAP) * 1000;

	// create renderer
	Renderer renderer;
	renderer.Initialize();
	renderer.CreateWindow("AI", screen_size.x, screen_size.y);

	// create render texture
	std::shared_ptr<Texture> texture = std::make_unique<Texture>(env_size.x, env_size.y, renderer);

	// create environment
	std::unique_ptr<Environment> env = std::make_unique<GameOfLife>(env_size.x, env_size.y, texture);
	env->Initialize();

	bool quit = false;
	while(!quit) {
		// sdl events
		SDL_Event event;
		SDL_PollEvent(&event);
		switch(event.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				if(event.key.keysym.sym == SDLK_ESCAPE) quit = true;
				env->KeyDown(event.key.keysym.sym);
				break;
			case SDL_MOUSEMOTION:
				env->MousePositionNormalized(glm::vec2{event.motion.x, event.motion.y} / (glm::vec2) screen_size);
				break;
			case SDL_MOUSEBUTTONDOWN:
				env->MouseButtonDown(event.button.button - 1);
				break;
			case SDL_MOUSEBUTTONUP:
				env->MouseButtonUp(event.button.button - 1);
				break;
		}

		// update environment
		env->Step();
		texture->Copy(env->GetColorBuffer());

		// Framerate cap
		std::this_thread::sleep_for(std::chrono::milliseconds(framerate));

		renderer.RenderCopy(texture);
		renderer.Present();
	}

	renderer.Shutdown();

	return 0;
}
