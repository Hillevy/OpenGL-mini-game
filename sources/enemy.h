#pragma once

#include "window.h"
#include "timer.h"
#include "ogl.h"
#include "load.h"
#include "terrain.h"
#include "microtimer.h"
#include <stdlib.h>
#include <time.h>

class Enemy
{
private:
	int position[3];
	int previousPos[3];
	microtimer microtime;
	microtimer::size_type lastUpdateTimeSprite, timeElapsedSinceUpdateSprite;

public:
	Enemy();
	~Enemy();
	void InitializeEnemyPosition(int, int);
	void Move(int*, float, int*);

	int* GetEnemyPos() {
		return this->position;
	}
	void SetEnemyPos(int x, int y, int z) {
		this->position[0] = x;
		this->position[1] = y;
		this->position[2] = z;
	}

	int* GetPreviousEnemyPos() {
		return this->previousPos;
	}
	void SetPreviousEnemyPos(int x, int y, int z) {
		this->previousPos[0] = x;
		this->previousPos[1] = y;
		this->previousPos[2] = z;
	}

};

