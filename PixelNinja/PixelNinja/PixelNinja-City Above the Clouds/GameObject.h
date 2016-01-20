#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#include <windows.h>
#include <gl/GLU.h>
#include <string>
#include "Point2D.h"

class GameObject{

protected:

	float x, y;
	int width, height;
	int radius;
	GLuint texture;
	Point2D center;

public:
	GameObject();
	GameObject(float, float, int, int, GLuint);

	//GETTER METHODS
	int getWidth();
	int getHeight();
	int getRadius();
	float getX();
	float getY();
	float getX2();
	float getY2();
	Point2D getCenter();

	virtual void setTexture(GLuint tex);
	virtual void draw();

};


#endif _GAMEOBJECT_H
