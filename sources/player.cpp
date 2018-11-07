#include "player.h"


float e = 5.0f;
float key1[3] = { 30,0,-100 };

Player::Player()
{
	this->health = 100;
	this->healthUI = "100";
	this->hit = false;
	this->keys = 0;
	this->totalKeys = 3;
	this->score = "0";
	this->key1_ = true;
	this->key2_ = true;
	this->key3_ = true;
	this->portalIsActive = false;

	this->keyPos1[0] = 1000; this->keyPos1[1] = 1000; this->keyPos1[2] = 1000;
	this->keyPos2[0] = 1000; this->keyPos2[1] = 1000; this->keyPos2[2] = 1000;
	this->keyPos3[0] = 1000; this->keyPos3[1] = 1000; this->keyPos3[2] = 1000;
	this->portal[0] = 1000; this->portal[1] = 1000; this->portal[2] = 1000;
}

Player::Player(int c, string s) {
	this->health = 100;
	this->healthUI = "100";
	this->hit = false;
	this->keys = c;
	this->totalKeys = 3;
	this->score = s;
	this->key1_ = true;
	this->key2_ = true;
	this->key3_ = true;
	this->portalIsActive = false;

	this->keyPos1[0] = 1000; this->keyPos1[1] = 1000; this->keyPos1[2] = 1000;
	this->keyPos2[0] = 1000; this->keyPos2[1] = 1000; this->keyPos2[2] = 1000;
	this->keyPos3[0] = 1000; this->keyPos3[1] = 1000; this->keyPos3[2] = 1000;
	this->portal[0] = 1000; this->portal[1] = 1000; this->portal[2] = 1000;
}

Player::~Player()
{
}

bool Player::Die() {
	if (this->health <= 0) {
		this->health = 100;
		this->healthUI = to_string(100);
		this->SetKeys(0);
		this->score = to_string(GetKeys());
		this->SetPortalIsActive(false);
		return true;
	}
	return false;
}
void Player::IncrementScore() {
	this->SetKeys(this->GetKeys() + 1);
	this->score = to_string(GetKeys());
}

void Player::Hit(float x, float z, int* pos) {
	if (abs(pos[0] - x) <= 5 && abs(pos[2] - z) <= 5) {
		this->SetHealth(this->GetHealth() - 1);
		this->healthUI = to_string(this->GetHealth());
		this->hit = true;
	}
}

void Player::HitUI() {
	if (this->hit) {
		glColor3f(0.6f, 0.0f, 0.0f);
		this->hit = false;
	}
	else {
		glColor3f(255, 255, 255);
	}
}

void Player::PickUpKey(int x, int y, int z) {
	//Key 1
	if (fabs(x - this->GetKeyPos1()[0]) <= e && fabs(y - this->GetKeyPos1()[1]) <= e && fabs(z - this->GetKeyPos1()[2]) <= e && key1_) {
		IncrementScore();
		SetKey1(false);
	} 
	//Key 2
	if (fabs(x - this->GetKeyPos2()[0]) <= e && fabs(y - this->GetKeyPos2()[1]) <= e && fabs(z - this->GetKeyPos2()[2]) <= e && key2_) {
		IncrementScore();
		SetKey2(false);
	}
	//Key 3
	if (fabs(x - this->GetKeyPos3()[0]) <= e && fabs(y - this->GetKeyPos3()[1]) <= e && fabs(z - this->GetKeyPos3()[2]) <= e && key3_) {
		IncrementScore();
		SetKey3(false);
	}
}

bool Player::ReachPortal(int x, int y, int z) {
	if (fabs(x - this->GetPortalPos()[0]) <= e && fabs(y - this->GetPortalPos()[1]) <= e && fabs(z - this->GetPortalPos()[2]) <= e) {
		return true;
	}
	return false;
}

void Player::ActivatePortal() {
	if (this->GetKeys() == this->GetTotalKeys()) {
		this->SetPortalIsActive(true);
	}
}

void Player::PrintScore() {
	int const& n = this->score.length() + 1;
	char* printScore = new char[n];
	strcpy(printScore, this->score.c_str());
	glLoadIdentity();
	glTranslatef(-7, 5, -10);
	write_2_screen("Keys ");
	glTranslatef(1, 0, 0);
	write_2_screen(printScore);
}

void Player::PrintHealth() {
	int const& n = this->healthUI.length() + 1;
	char* printHealth = new char[n];
	strcpy(printHealth, this->healthUI.c_str());
	glLoadIdentity();
	glTranslatef(-7,4.5f, -10);
	write_2_screen("Life ");
	glTranslatef(1, 0, 0);
	write_2_screen(printHealth);
}

bool Player::CompareVectors(int* a, int* b) {
	for (int i = 0; i < 3; i++) {
		if (a[i] != b[i]) {
			return false;
		}
	}
	return true;
}

void Player::InitializeKeysPosition(int x, int y, int z) {
	int zero[3] = { 1000,1000,1000 };
	if (CompareVectors(this->GetKeyPos1(), zero)) {
		this->SetKeyPos1(x, y, z);
	}
	else if (CompareVectors(this->GetKeyPos2(), zero)) {
		this->SetKeyPos2(x, y, z);
	}
	else if (CompareVectors(this->GetKeyPos3(), zero)) {
		this->SetKeyPos3(x, y, z);
	}
}

void Player::InitializePortalPosition(int x, int y, int z) {
	int zero[3] = { 1000,1000,1000 };
	if (CompareVectors(this->GetPortalPos(), zero)) {
		this->SetPortalPos(x, y, z);
	}
}

void Player::Clamp(float x, float z, point camera, float posX, float posZ) {
	float distance = 10;
	if (abs(x - camera.x) <= distance && x - camera.x>=0) {
		posX = x - distance;
	}
	else if (abs(x - camera.x) <= distance && x - camera.x < 0)
	{
		posX = x + distance;
	}
	if (abs(z - camera.z) <= distance && z - camera.z >= 0) {
		posZ = z - distance;
	}
	else if (abs(z - camera.z) <= distance && z - camera.z >= 0) {
		posZ = z + distance;
	}
}

