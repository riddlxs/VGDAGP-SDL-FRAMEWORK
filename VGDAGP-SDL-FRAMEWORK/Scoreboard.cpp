#include "Scoreboard.h"

void Scoreboard::ClearBoard() {
	for (auto texture : mScore) {
		delete texture;
	}

	mScore.clear();
}

Scoreboard::Scoreboard() : Scoreboard({ 230, 230, 230 }) { }

Scoreboard::Scoreboard(SDL_Color color) {
	mColor = color;
	Score(0);
}