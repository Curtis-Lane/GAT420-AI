#include "GameOfLife.h"

#include "Texture.h"
#include "Random.h"

bool GameOfLife::Initialize() {
	bufferA.resize(size.x * size.y);
	bufferB.resize(size.x * size.y);

	return true;
}

void GameOfLife::Step() {
	frame += (!paused || frameAdvance) ? 1 : 0;

	bool evenFrame = (frame % 2 == 0) ? true : false;
	std::vector<uint8_t>& readBuffer = evenFrame ? bufferA : bufferB;
	std::vector<uint8_t>& writeBuffer = evenFrame ? bufferB : bufferA;

	if(!paused || frameAdvance) {
		// Update buffer
		// Clear write buffer
		std::fill(writeBuffer.begin(), writeBuffer.end(), 0);

		for(int y = 0; y < size.y; y++) {
			for(int x = 0; x < size.x; x++) {
				int weight = 0;

				// Read surrounding 8 cells, accumulate weight
				// XXX
				// X0X
				// XXX
				weight += Read<uint8_t>(readBuffer, x - 1, y - 1);
				weight += Read<uint8_t>(readBuffer, x, y - 1);
				weight += Read<uint8_t>(readBuffer, x + 1, y - 1);
				weight += Read<uint8_t>(readBuffer, x + 1, y);
				weight += Read<uint8_t>(readBuffer, x + 1, y + 1);
				weight += Read<uint8_t>(readBuffer, x, y + 1);
				weight += Read<uint8_t>(readBuffer, x - 1, y + 1);
				weight += Read<uint8_t>(readBuffer, x - 1, y);

				// Game of life rules
				// If cell is alive, update
				if(Read<uint8_t>(readBuffer, x, y)) {
					if(weight < 2) {
						Write<uint8_t>(writeBuffer, x, y, 0);
					} else if(weight > 3) {
						Write<uint8_t>(writeBuffer, x, y, 0);
					} else {
						Write<uint8_t>(writeBuffer, x, y, 1);
					}
				} else {
					// If cell is dead, update
					if(weight == 3) {
						Write<uint8_t>(writeBuffer, x, y, 1);
					}
				}
			}
		}
		frameAdvance = false;
	}

	// "Draw" onto buffer
	if(mouse_button[0]) {
		Write<uint8_t>(writeBuffer, mouse_position.x, mouse_position.y, 1);
	} else if(mouse_button[2]) {
		Write<uint8_t>(writeBuffer, mouse_position.x, mouse_position.y, 0);
	}

	// Convert buffer data format to color buffer
	std::transform((writeBuffer).begin(), (writeBuffer).end(), color_buffer.begin(), [](uint8_t v) {
		{return v ? 0xFFFFFFFF : 0;}
	});

	// Copy color buffer to texture
	texture->Copy(color_buffer);
}

void GameOfLife::KeyDown(SDL_Keycode keycode) {
	switch(keycode) {
		case SDLK_r:
			{
				// Write random 'alive' cells to buffer
				std::vector<uint8_t>& writeBuffer = (frame % 2 == 1) ? bufferA : bufferB;
				for(int i = 0; i < writeBuffer.size(); i++) {
					writeBuffer[i] = (random(0, 10) == 0) ? 1 : 0;
				}
			}
			break;
		case SDLK_e:
			paused = !paused;
			break;
		case SDLK_q:
			frameAdvance = true;
			break;
	}
}