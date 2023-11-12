#pragma once
#ifndef MAIN_H
#define MAIN_H

#define MAXIMUM_MAP_WIDTH 256
#define MAXIMUM_MAP_HEIGHT 256

#include "raincloud/raincloud.h"

const int WINDOW_WIDTH = 1600;
const int WINDOW_HEIGHT = 900;

class AutoTile {
private:
	std::string filePath = "";
	uint32_t textureID = NULL;

	const uint32_t hFrames = 12;
	const uint32_t vFrames = 4;

	rc::vec2 cellSize = { 32, 32 };
	rc::vec2 imgSize = { 0, 0 };
	std::vector<rc::vec2> locations;

	bool mapInfo[MAXIMUM_MAP_HEIGHT][MAXIMUM_MAP_WIDTH] = { false };
public:
	AutoTile(std::string _filePath, rc::vec2 _cellSize);
	~AutoTile();

	void put(int _hPos, int _vPos);
	void clear();

	void draw();
};

AutoTile::AutoTile(std::string _filePath, rc::vec2 _cellSize)
	: filePath(_filePath), cellSize(_cellSize) 
{
	int width, height, channels;
	unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
	this->imgSize.x = width;
	this->imgSize.y = height;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	if (channels > 0 && channels <= 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	else if (channels == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	else {
		std::cout << "Cannot Load Image File : " << filePath;
	}
}

AutoTile::~AutoTile() {
	if (textureID != 0) glDeleteTextures(1, &textureID);
}

void AutoTile::put(int _hPos, int _vPos) {
	if (0 > _hPos || 0 > _vPos || _hPos >= MAXIMUM_MAP_WIDTH || _vPos >= MAXIMUM_MAP_HEIGHT) {
		std::cout << "Cannot Put Tile On : " << _hPos << ", " << _vPos << "\n";
		return;
	}
	if (mapInfo[_vPos][_hPos] == true) {
		return;
	}
	locations.push_back({ _hPos, _vPos });
	mapInfo[_vPos][_hPos] = true;
}

void AutoTile::clear() {
	for (size_t i = 0; i < MAXIMUM_MAP_HEIGHT; i++) {
		for (size_t j = 0; j < MAXIMUM_MAP_WIDTH; j++) {
			mapInfo[i][j] = false;
		}
	}
	locations.clear();
}

void AutoTile::draw() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glPushMatrix();

	std::vector<rc::rect> animGrid{};
	double stepX = imgSize.x / double(hFrames), stepY = imgSize.y / double(vFrames);

	const double epsilon = 1e-9;
	for (double indexY = 0.0; indexY + stepY <= double(imgSize.y) + epsilon; indexY += stepY) {
		for (double indexX = 0.0; indexX + stepX <= double(imgSize.x) + epsilon; indexX += stepX) {
			rc::rect r(indexX / imgSize.x, indexY / imgSize.y, stepX / imgSize.x, stepY / imgSize.y);
			animGrid.push_back(r);
		}
	}

	for (rc::vec2 index : locations) {
		rc::vec2 position;
		position.x = index.x * cellSize.x;
		position.y = index.y * cellSize.y;

		int X = int(index.x);
		int Y = int(index.y);
		const int MAX_X = MAXIMUM_MAP_WIDTH;
		const int MAX_Y = MAXIMUM_MAP_HEIGHT;

		BYTE _00 = 0;
		BYTE _01 = 0;
		BYTE _02 = 0;

		BYTE _10 = 0;
		BYTE _12 = 0;

		BYTE _20 = 0;
		BYTE _21 = 0;
		BYTE _22 = 0;

		if (X - 1 >= 0 && Y - 1 >= 0 && mapInfo[Y - 1][X - 1] == true) _00 = 1;
		if (Y - 1 >= 0 && mapInfo[Y - 1][X] == true) _01 = 2;
		if (X + 1 < MAX_X && Y - 1 >= 0 && mapInfo[Y - 1][X + 1] == true) _02 = 4;

		if (X - 1 >= 0 && mapInfo[Y][X - 1] == true) _10 = 8;
		if (X + 1 < MAX_X && mapInfo[Y][X + 1] == true) _12 = 16;

		if (X - 1 >= 0 && Y + 1 < MAX_Y && mapInfo[Y + 1][X - 1] == true) _20 = 32;
		if (Y + 1 < MAX_Y && mapInfo[Y + 1][X] == true) _21 = 64;
		if (X + 1 < MAX_X && Y + 1 < MAX_Y && mapInfo[Y + 1][X + 1] == true) _22 = 128;

		rc::rect current;
		BYTE flag = _00 | _01 | _02 | _10 | _12 | _20 | _21 | _22;

		switch (flag) {
		case (64): current = animGrid[0]; break;
		case (80): current = animGrid[1]; break;
		case (88): current = animGrid[2]; break;
		case (72): current = animGrid[3]; break;
		case (91): current = animGrid[4]; break;
		case (216): current = animGrid[5]; break;
		case (120): current = animGrid[6]; break;
		case (94): current = animGrid[7]; break;
		case (208): current = animGrid[8]; break;
		case (250): current = animGrid[9]; break;
		case (248): current = animGrid[10]; break;
		case (104): current = animGrid[11]; break;

		case (66): current = animGrid[12]; break;
		case (82): current = animGrid[13]; break;
		case (90): current = animGrid[14]; break;
		case (74): current = animGrid[15]; break;
		case (210): current = animGrid[16]; break;
		case (254): current = animGrid[17]; break;
		case (251): current = animGrid[18]; break;
		case (106): current = animGrid[19]; break;
		case (214): current = animGrid[20]; break;
		case (126): current = animGrid[21]; break;
		// case (): current = animGrid[22]; break;
		case (123): current = animGrid[23]; break;

		case (2): current = animGrid[24]; break;
		case (18): current = animGrid[25]; break;
		case (26): current = animGrid[26]; break;
		case (10): current = animGrid[27]; break;
		case (86): current = animGrid[28]; break;
		case (223): current = animGrid[29]; break;
		case (127): current = animGrid[30]; break;
		case (75): current = animGrid[31]; break;
		case (222): current = animGrid[32]; break;
		case (255): current = animGrid[33]; break;
		case (219): current = animGrid[34]; break;
		case (107): current = animGrid[35]; break;

		case (0): current = animGrid[36]; break;
		case (16): current = animGrid[37]; break;
		case (24): current = animGrid[38]; break;
		case (8): current = animGrid[39]; break;
		case (122): current = animGrid[40]; break;
		case (30): current = animGrid[41]; break;
		case (27): current = animGrid[42]; break;
		case (218): current = animGrid[43]; break;
		case (22): current = animGrid[44]; break;
		case (31): current = animGrid[45]; break;
		case (95): current = animGrid[46]; break;
		case (11): current = animGrid[47]; break;
		default:
			BYTE specialFlag = _01 | _10 | _12 | _21;
			switch (specialFlag) {
			case (0): current = animGrid[36]; break;
			case (2): current = animGrid[24]; break;
			case (16): current = animGrid[37]; break;
			case (64): current = animGrid[0]; break;
			case (8): current = animGrid[39]; break;
			case (24): current = animGrid[38]; break;
			case (66): current = animGrid[12]; break;

			case (18): {
				current = animGrid[_02 != 0 ? 44 : 25]; break;
			}
			case (10): {
				current = animGrid[_00 != 0 ? 47 : 27]; break;
			}
			case (80): {
				current = animGrid[_22 != 0 ? 8 : 1]; break;
			}
			case (72): {
				current = animGrid[_20 != 0 ? 11 : 3]; break;
			}

			case (82): {
				if (_02 == 0 && _22 == 0) current = animGrid[13];
				else if (_02 == 0 && _22 != 0) current = animGrid[16];
				else if (_02 != 0 && _22 == 0) current = animGrid[28];
				else current = animGrid[20];
				break;
			}
			case (74): {
				if (_00 == 0 && _20 == 0) current = animGrid[15];
				else if (_00 == 0 && _20 != 0) current = animGrid[19];
				else if (_00 != 0 && _20 == 0) current = animGrid[31];
				else current = animGrid[35];
				break;
			}
			case (88): {
				if (_20 == 0 && _22 == 0) current = animGrid[2];
				else if (_20 == 0 && _22 != 0) current = animGrid[5];
				else if (_20 != 0 && _22 == 0) current = animGrid[6];
				else current = animGrid[10];
				break;
			}
			case (26): {
				if (_00 == 0 && _02 == 0) current = animGrid[26];
				else if (_00 == 0 && _02 != 0) current = animGrid[41];
				else if (_00 != 0 && _02 == 0) current = animGrid[42];
				else current = animGrid[45];
				break;
			}
			case (90): current = animGrid[14]; break;
			default:
				// std::cout << "Invalid Flag : " << int(flag) << "\n";
				// break;
				current = animGrid[36];
				break;
			}
		}
		glBegin(GL_QUADS);
		glTexCoord2d(current.x, current.y + current.h);
		glVertex2f(position.x, WINDOW_HEIGHT + -(position.y + cellSize.y));
		glTexCoord2d(current.x + current.w, current.y + current.h);
		glVertex2f(position.x + cellSize.x, WINDOW_HEIGHT + -(position.y + cellSize.y));
		glTexCoord2d(current.x + current.w, current.y);
		glVertex2f(position.x + cellSize.x, WINDOW_HEIGHT + -(position.y));
		glTexCoord2d(current.x, current.y);
		glVertex2f(position.x, WINDOW_HEIGHT + -(position.y));
		glEnd();
	}

	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}


#endif