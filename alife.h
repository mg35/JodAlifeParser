#pragma once

#include <string>

struct alife { 
	std::string name;
	bool isKing;
	bool isMepian;
	float likeability;
	float lifespan;
	unsigned int generation;
	unsigned char parts[5];
	unsigned short stats[8][2];
	unsigned char mystery38b;
};