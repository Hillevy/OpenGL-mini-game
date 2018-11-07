#include "enemy.h"

int e = 1;
int speed = 1;

Enemy::Enemy()
{
	lastUpdateTimeSprite = microtime();
	this->position[0] = 1000; this->position[1] = 1000; this->position[2] = 1000;
	this->previousPos[0] = 0; this->previousPos[1] = 0; this->previousPos[2] = 0;
}


Enemy::~Enemy()
{
}

bool CompareVectors(int* a, int* b) {
	for (int i = 0; i < 3; i++) {
		if (a[i] != b[i]) {
			return false;
		}
	}
	return true;
}

///a is copied in b
int* CopyArray(int* a, int* b) {
	for (int i = 0; i < 3; i++) {
		b[i] = a[i];
	}
	return b;
}

void Enemy::InitializeEnemyPosition(int l, int c) {
	if (this->position[0] == 1000 && this->position[1] == 1000 && this->position[2] == 1000) {
		this->SetEnemyPos(l, 0, c);
		this->SetPreviousEnemyPos(l, 0, c);
	}
}

void Enemy::Move(int* origin, float distance, int* direction) {

}
