#pragma once
#include "constants.h"
class NuObject
{
public:
	NuObject(HWND, LPCSTR, int, int);
	LPCSTR name;
	HWND reference;

	//==================================================================================================
	// returns the stored yposition on the gamescreen
	//================================================
	//int getYPos() {
	//	return yPos;
	//}
	//================================================
	// returns the stored xposition on the gamescreen
	//================================================
	//int getXPos() {
	//	return xPos;
	//}
	//===============================================================================================
	// returns the stored type (aka a number to decide what if statement should be valid on creation
	//===============================================================================================
	//int getType() {
	//	return type;
	//}
	//================================================
	// chenges the stored yposition on the gamescreen
	//================================================
	//void ChangeYPos(int newVal) {
	//	yPos = newVal;
	//	return;
	//}
	//================================================
	// changes the stored xposition on the gamescreen
	//================================================
	//void ChangeXPos(int newVal) {
	//	xPos = newVal;
	//	return;
	//}
	//================================================
	// change the type, currently not implemented
	//================================================
	//void ChangeType(int newVal) {
	//	type = newVal;
	//	return;
	//}
	//================================================
	// returns the name, LPCSTR is a type of string
	//================================================
	LPCSTR getName() {
		return name;
	}
	//================================================
	// changes the name
	//================================================
	//void ChangeName(LPCSTR newName) {
	//	name = newName;
	//	return;
	//}
	//int getNum() {
	//	return num;
	//}
	HWND getReference() {
		return reference;
	}
};