#pragma once

#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include "ogl.h"
using namespace std;


class Terrain
{
private:
	int blockSize;
	int currentColumn;
	int currentLine;
public:
	Terrain();
	Terrain(int);
	~Terrain();

	int	GetBlockSize()
	{
		return this->blockSize;
	}
	void SetBlockSize(int i)
	{
		this->blockSize = i;
	}
	int	GetCurrentColumn()
	{
		return this->currentColumn;
	}
	void SetCurrentColumn(int i)
	{
		this->currentColumn = i;
	}
	int	GetCurrentLine()
	{
		return this->currentLine;
	}
	void SetCurrentLine(int i)
	{
		this->currentLine = i;
	}

	void GenerateTerrain();
	void GetBlockPosition(int, int, int);
	void CreateBlock(int, int);
	void CreateTexturedQuad(int, int , int);
};

#endif

