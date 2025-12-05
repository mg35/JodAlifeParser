// jodAlifeReader.cpp : Defines the entry point for the application.
//

#include "jodAlifeReader.h"
#include "alife.h"

#include <fstream>
#include <string>
#include <iostream>
#include <vector>

void flipDword(char* dword, char* reverseDword);
void flipWord(char* word, char* reverseWord);

std::vector<std::string> nameSegments = { 
	"", "C", "D", "F", "H", "K", "L", "M", "N", "P", "R", "S", "T", "W", "Y", "PH", "LY", 
	"", "B", "G", "J", "V", "Q", "X", "Z", "CH", "SH", "KY", "NY", "MY", "HY", "RY", 
	"", "A", "AI", "AU", "AE", "AO", "I", "IA", "IU", "IE", "IO", "U", "UA", "UI", "UE", "UO", "E", "EA", "EI", "EU", "EO", "O", "OA", "OI", "OU", "OE" };

std::vector<std::string> mepianTypes = { "", "Beez", "Seapo", "Eelon", "Mewwy", "Shleep", "Scaropion", "Mothpil", "Balooga", "Murtle", "Deworm" };

int main()
{
	std::string filename;
	std::cout << "Enter filename: ";
	std::getline(std::cin, filename);
	std::ifstream reader = std::ifstream();
	reader.open(filename, std::ios::binary);
	int baseOffset = 0x01ba9b80;
	char dword[4] = { 0,0,0,0 };
	char reverseDword[4] = { 0,0,0,0 };
	char word[2] = { 0,0 };
	char reverseWord[2] = { 0,0 };
	std::vector<alife> alifeList;
	printf("%x", *(int*) reverseDword);
	for (int i = 0; i < 445; i++) {
		reader.seekg(baseOffset + i * 0xa0 + 0x40);
		reader.read(dword, 4);
		flipDword(dword, reverseDword);
		int selfPtr = *(int*)reverseDword - 0x90000000;
		if (selfPtr == baseOffset + i * 0xa0) { // valid self pointer
			alife currAlife;
			reader.seekg(baseOffset + i * 0xa0 + 0x50);
			reader.read(dword, 4);
			flipDword(dword, reverseDword);
			currAlife.lifespan = *(float*)reverseDword;
			reader.seekg(baseOffset + i * 0xa0 + 0x58);
			reader.read(dword, 4);
			flipDword(dword, reverseDword);
			currAlife.likeability = *(float*)reverseDword;
			reader.seekg(baseOffset + i * 0xa0 + 0x9A);
			reader.read(word, 2);
			flipWord(word, reverseWord);
			currAlife.generation = *(short*)reverseWord;
			reader.seekg(baseOffset + i * 0xa0 + 0x4F);
			reader.read(word, 1);
			currAlife.isKing = (word[0] & 2) == 2;
			reader.seekg(baseOffset + i * 0xa0 + 0x70);
			reader.read(dword, 4);
			flipDword(dword, reverseDword);
			int dataPtr = *(int*)reverseDword - 0x90000000;
			reader.seekg(dataPtr + 0x348);
			for (int j = 0; j < 16; j++) {
				reader.read(word, 2);
				flipWord(word, reverseWord);
				currAlife.stats[j % 8][j / 8] = *(short*)reverseWord;
			}
			reader.seekg(dataPtr + 0x386);
			currAlife.isMepian = false;
			for (int j = 0; j < 5; j++) {
				reader.read(word, 1);
				currAlife.parts[j] = word[0];
				if (word[0] > 0) {
					currAlife.isMepian = true;
				}
			}
			reader.read(word, 1);
			currAlife.mystery38b = word[0];
			for (int j = 0; j < 5; j++) {
				reader.read(word, 1);
				currAlife.name += nameSegments[word[0]];
			}
			alifeList.push_back(currAlife);
		}
	}
	std::cout << "Name,Type,Mood,Lifespan,Generation,Body,Head,Arms,Legs,Wings,MysteryGene,";
	std::cout << "Stat1,Stat2,Stat3,Stat4,Stat5,Stat6,Stat7,Stat8" << std::endl;
	for (int k = 0; k < alifeList.size(); k++) {
		std::string type = "Nightopian";
		if (alifeList.at(k).isKing) {
			type = "King";
		}
		else if (alifeList.at(k).isMepian) {
			type = "Mepian";
		}
		std::cout << alifeList.at(k).name << ",";
		std::cout << type << ",";
		std::cout << alifeList.at(k).likeability << ",";
		std::cout << alifeList.at(k).lifespan << ",";
		std::cout << alifeList.at(k).generation << ",";
		for (int l = 0; l < 5; l++) {
			std::cout << mepianTypes.at(alifeList.at(k).parts[l]) << ",";
		}
		std::cout << (int) alifeList.at(k).mystery38b;
		for (int m = 0; m < 8; m++) {
			std::cout << "," << (int) alifeList.at(k).stats[m][0];
		}
		std::cout << std::endl;
	}
}

void flipDword(char* dword, char* reverseDword) {
	reverseDword[0] = dword[3];
	reverseDword[1] = dword[2];
	reverseDword[2] = dword[1];
	reverseDword[3] = dword[0];
}

void flipWord(char* word, char* reverseWord) {
	reverseWord[0] = word[1];
	reverseWord[1] = word[0];
}