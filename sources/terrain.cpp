#include "Terrain.h"


Terrain::Terrain()
{
	this->blockSize = 10;
}

Terrain::Terrain(int i)
{
	this->blockSize = i;
}

Terrain::~Terrain()
{
}

void Terrain::GetBlockPosition(int i, int col, int li)
{
	this->currentColumn = i % col;
	this->currentLine = (i==0) ? 0 : i / col;
}

void Terrain::GenerateTerrain() { //23*15
	ifstream terrainFile;
	const char* path = "GenerateTerrain.txt";
	terrainFile.open(path);
	if (terrainFile.is_open()) {
		char c;
		int countCharacters = 0;
		while (!terrainFile.eof()) {
			terrainFile.get(c);
			GetBlockPosition(countCharacters, 23, 15);
			switch (c)
			{
				case '0':
					glLoadIdentity();
					glTranslatef(0, 0, -10);
					write_2_screen("zero");
					break;
				case '1':
					glLoadIdentity();
					glTranslatef(0, -1, -10);
					write_2_screen("1");
					break;
				case '2':
					glLoadIdentity();
					glTranslatef(0, -2, -10);
					write_2_screen("2");
					break;
				case '3':
					glLoadIdentity();
					glTranslatef(0, -3, -10);
					write_2_screen("3");
					break;
				case '4':
					glLoadIdentity();
					glTranslatef(0, -4, -10);
					write_2_screen("4");
					break;
				default:
					glLoadIdentity();
					glTranslatef(-20, -5, -10);
					write_2_screen("default");
					break;
			}
			countCharacters += 1;
		}

	}
	else {
		perror(path);
	}
	terrainFile.close();
}

void Terrain::CreateBlock(int l, int c) 
{
	glPushMatrix();
	//Global positionning of the block
	glTranslatef(c*this->blockSize, 0, l*this->blockSize);

	//South
	glPushMatrix();
	glTranslatef(0, 0, -this->blockSize / 2);
	glScalef(this->blockSize, this->blockSize, this->blockSize);
	CreateTexturedQuad(1,0,0);
	glPopMatrix();
	//North
	glPushMatrix();
	glTranslatef(0, 0, this->blockSize / 2);
	glScalef(this->blockSize, this->blockSize, this->blockSize);
	CreateTexturedQuad(0,1,0);
	glPopMatrix();
	//East
	glPushMatrix();
	glTranslatef(this->blockSize / 2, 0, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(this->blockSize, this->blockSize, this->blockSize);
	CreateTexturedQuad(0,0,1);
	glPopMatrix();
	//West
	glPushMatrix();
	glTranslatef(-this->blockSize / 2, 0, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(this->blockSize, this->blockSize, this->blockSize);
	CreateTexturedQuad(1,0,1);
	glPopMatrix();

	glPopMatrix();
	
}

void Terrain::CreateTexturedQuad(int a, int b, int c)
{
	glBegin(GL_QUADS);
	glColor3f(a, b, c);
	/*glVertex3f(this->blockSize, this->blockSize, 0);
	glVertex3f(this->blockSize, -this->blockSize, 0);
	glVertex3f(-this->blockSize, -this->blockSize, 0);
	glVertex3f(-this->blockSize, this->blockSize, 0);*/
	glVertex3f(0.5f, 0.5f, 0);
	glVertex3f(0.5f, -0.5f, 0);
	glVertex3f(-0.5f, -0.5f, 0);
	glVertex3f(-0.5f, 0.5f, 0);
	glEnd();
}