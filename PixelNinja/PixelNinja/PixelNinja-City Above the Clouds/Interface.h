#ifndef _INTERFACE_H
#define _INTERFACE_H

#include "Ninja.h"
#include "Monster.h"

class Interface{
private:

	float x, y;
	int width, height;
	int value, maxvalue;
	int red, green, blue;

public:
	//CONSTRUCTORS
	Interface();
	Interface(float, float, int, int);

	//GETTERS
	float getX();
	float getY();
	int getWidth();
	int getHeight();
	int getValue();
	int getMaxValue();

	//SETTERS
	void setX(float);
	void setY(float);
	void setWidth(int);
	void setHeight(int);
	void setValue(int);
	void setMaxValue(int);

	//OTHER METHODS
	void draw();
	void update(Ninja*);

};

#endif _INTERFACE_H
