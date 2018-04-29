/*
===========================================================================
blockattack - Block Attack - Rise of the Blocks
Copyright (C) 2005-2018 Poul Sander

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses/

Source information and contacts persons can be found at
https://blockattack.net
===========================================================================
*/

#include "HelpHowtoState.hpp"
#include "global.hpp"
#include "common.h"
#include "MenuSystem.h"

const int xsize = 1024;
const int ysize = 768;
const int buttonOffset = 160;
extern sago::SagoSprite bExit;
extern sago::SagoSprite bricks[7];

/**
 * Draws bricks with a string like:
 * "aab" for two identical one and another
 * "aaB" the third one will have a bomb
 * The any char not in 'a' to 'g' or 'A' to 'G' the behavior is undefined. 
 * @param target Target to draw to
 * @param bricks description on what to draw as a string
 * @param x
 * @param y
 */
static void RenderRowOfBricks(SDL_Renderer* target, const std::string& brickStr, int x, int y) {
	Uint32 tick = SDL_GetTicks();
	for (size_t i = 0; i < brickStr.size(); ++i) {
		bool bomb = false;
		char brickChar = brickStr[i];
		if (brickChar >= 'A' && brickChar <= 'G') {
			bomb = true;
			brickChar = brickChar + 'a' - 'A';
		}
		if (brickChar >= 'a' &&  brickChar <= 'g') {
			bricks[brickChar - 'a'].Draw(target, tick, x+i*50, y);
			if (bomb) {
				globalData.spriteHolder->GetSprite("block_bomb").Draw(target, tick, x+i*50, y);
			}
		}
	}
}

class HorizontalSwitchAnimation {
public:
	std::string brickStr = "abc";
	int cursorPos = 0;
	int state = 0; //0=move left, 1 = switch, 2 = move right, 3 = switch
	Uint32 lastTick = 0;
	Uint32 animationSpeed = 2000;
	
	void Update (Uint32 tick) {
		if (tick > lastTick + animationSpeed) {
			lastTick = tick;
			switch (state) {
				case 0:
					cursorPos = 1;
					break;
				case 1:  //fallthough
				case 3:
					std::swap(brickStr[cursorPos], brickStr[cursorPos + 1]);
					break;
				case 2:
					cursorPos = 0;
					break;
			}
			++state;
			if (state > 3) {
				state = 0;
			}
		}
	}
};

class MultiLineBlocks {
private:
	std::vector<std::string> lines;
public:
	MultiLineBlocks& addLine(const std::string& line) {
		lines.push_back(line);
		return *this;
	}
	
	void Render(SDL_Renderer* target, int x, int y) {
		for (size_t i = 0; i < lines.size(); ++i) {
			RenderRowOfBricks(target, lines[i], x, y+i*50);
		}
	}
	
};

HorizontalSwitchAnimation switchAnimation;
sago::SagoTextField switchAnimationField;
sago::SagoTextField clearRowfield;
sago::SagoTextField comboField;
sago::SagoTextField dropField;
sago::SagoTextField chainField;

static void InitTextField(sago::SagoTextField& field, const char* text) {
	field.SetHolder(&globalData.spriteHolder->GetDataHolder());
	field.SetFontSize(30);
	field.SetOutline(2, {0,0,0,255});
	field.SetText(text);
}

HelpHowtoState::HelpHowtoState() {
	InitTextField(switchAnimationField, _("Switch block horizontally"));
	InitTextField(clearRowfield, _("Match 3 to clear"));
	InitTextField(comboField, _("Create combos!"));
	InitTextField(dropField, _("Drop blocks!"));
	InitTextField(chainField, _("Create a chain effect"));
}

HelpHowtoState::~HelpHowtoState() {
}

bool HelpHowtoState::IsActive() {
	return isActive;
}

void HelpHowtoState::ProcessInput(const SDL_Event& event, bool& processed) {

	UpdateMouseCoordinates(event, globalData.mousex, globalData.mousey);

	if (isConfirmEvent(event) || isEscapeEvent(event)) {
		isActive = false;
		processed = true;
	}
}

void HelpHowtoState::Draw(SDL_Renderer* target) {
	DrawBackground(target);
	RenderRowOfBricks(target, switchAnimation.brickStr, 50, 50);
	globalData.spriteHolder->GetSprite("cursor").Draw(target, SDL_GetTicks(), 50+switchAnimation.cursorPos*50, 50);
	switchAnimationField.Draw(target, 50 +150+30, 50+25, sago::SagoTextField::Alignment::left, sago::SagoTextField::VerticalAlignment::center);
	RenderRowOfBricks(target, "adaa", 50, 150);
	globalData.spriteHolder->GetSprite("cursor").Draw(target, SDL_GetTicks(), 50, 150);
	RenderRowOfBricks(target, "dAAA", 50+300, 150);
	clearRowfield.Draw(target, 600, 150+25, sago::SagoTextField::Alignment::left, sago::SagoTextField::VerticalAlignment::center);
	comboField.Draw(target, 50+175, 410, sago::SagoTextField::Alignment::center);
	MultiLineBlocks().addLine("ab").addLine("ba").addLine("ab").Render(target, 50, 250);
	globalData.spriteHolder->GetSprite("cursor").Draw(target, SDL_GetTicks(), 50, 250+50);
	MultiLineBlocks().addLine("AB").addLine("AB").addLine("AB").Render(target, 50+200, 250);
	MultiLineBlocks().addLine("a").addLine("b").addLine("e").Render(target, 50+400, 250);
	globalData.spriteHolder->GetSprite("cursor").Draw(target, SDL_GetTicks(), 50+400, 250);
	MultiLineBlocks().addLine(" ").addLine("b").addLine("ea").Render(target, 50+400+200, 250);
	dropField.Draw(target, 50+400+150, 410, sago::SagoTextField::Alignment::center);
	MultiLineBlocks().addLine(" d").addLine(" f").addLine(" f").addLine("fdd").Render(target, 50, 500);
	MultiLineBlocks().addLine(" d").addLine(" F").addLine(" F").addLine("dFd").Render(target, 50+200, 500);
	MultiLineBlocks().addLine(" d").addLine("  ").addLine("  ").addLine("d d").Render(target, 50+200*2, 500);
	MultiLineBlocks().addLine("  ").addLine("  ").addLine("  ").addLine("DDD").Render(target, 50+200*3, 500);
	chainField.Draw(target, 400, 710, sago::SagoTextField::Alignment::center);
	bExit.Draw(globalData.screen, SDL_GetTicks(), xsize-buttonOffset, ysize-buttonOffset);
}

void HelpHowtoState::Update() {
	// If the mouse button is released, make bMouseUp equal true
	if ( !(SDL_GetMouseState(nullptr, nullptr)&SDL_BUTTON(1)) ) {
		bMouseUp=true;
	}

	if (SDL_GetMouseState(nullptr,nullptr)&SDL_BUTTON(1) && bMouseUp) {
		bMouseUp = false;

		//The Score button:
		if ((globalData.mousex>xsize-buttonOffset) && (globalData.mousex<xsize-buttonOffset+bExit.GetWidth()) 
				&& (globalData.mousey>ysize-buttonOffset) && (globalData.mousey<ysize-buttonOffset+bExit.GetHeight())) {
			isActive = false;
		}

	}
	switchAnimation.Update(SDL_GetTicks());
}