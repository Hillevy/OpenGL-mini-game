#pragma once

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <sstream>
#include <string.h>
#include "window.h"
#include "ogl.h"
#include "maths_3d.h"
#include "camera.h"

using namespace std;

class Player
{
private:
	int health;
	string healthUI;
	bool hit;
	int keys;
	int totalKeys;
	string score;
	bool key1_;
	bool key2_;
	bool key3_;
	int keyPos1[3];
	int keyPos2[3];
	int keyPos3[3];
	int portal[3];
	bool portalIsActive;

public:
	Player();
	Player(int, string);
	~Player();

	bool Die();
	void Hit(float, float, int*);
	void HitUI();
	void PickUpKey(int, int, int);
	bool ReachPortal(int, int, int);
	void ActivatePortal();
	void IncrementScore();
	void PrintScore();
	void PrintHealth();
	bool CompareVectors(int*, int*);
	void InitializeKeysPosition(int, int, int);
	void InitializePortalPosition(int, int, int);
	void Clamp(float, float, point, float, float);

	int GetHealth() {
		return this->health;
	}
	void SetHealth(int h) {
		this->health = h;
	}

	int GetKeys() {
		return this->keys;
	}
	void SetKeys(int k) {
		this->keys = k;
	}

	int GetTotalKeys() {
		return this->totalKeys;
	}

	bool GetKey1() {
		return this->key1_;
	}
	void SetKey1(bool a) {
		this->key1_ = a;
	}

	bool GetKey2() {
		return this->key2_;
	}
	void SetKey2(bool a) {
		this->key2_ = a;
	}

	bool GetKey3() {
		return this->key3_;
	}
	void SetKey3(bool a) {
		this->key3_ = a;
	}

	int* GetKeyPos1() {
		return this->keyPos1;
	}
	void SetKeyPos1(int x, int y, int z) {
		this->keyPos1[0] = x;
		this->keyPos1[1] = y;
		this->keyPos1[2] = z;
	}
	int* GetKeyPos2() {
		return this->keyPos2;
	}
	void SetKeyPos2(int x, int y, int z) {
		this->keyPos2[0] = x;
		this->keyPos2[1] = y;
		this->keyPos2[2] = z;
	}
	int* GetKeyPos3() {
		return this->keyPos3;
	}
	void SetKeyPos3(int x, int y, int z) {
		this->keyPos3[0] = x;
		this->keyPos3[1] = y;
		this->keyPos3[2] = z;
	}

	int* GetPortalPos() {
		return this->portal;
	}
	void SetPortalPos(int x, int y, int z) {
		this->portal[0] = x;
		this->portal[1] = y;
		this->portal[2] = z;
	}

	bool GetPortalIsActive() {
		return this->portalIsActive;
	}
	void SetPortalIsActive(bool a) {
		this->portalIsActive = a;
	}

};

#endif
