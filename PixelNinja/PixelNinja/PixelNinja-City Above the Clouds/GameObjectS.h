#ifndef _GAMEOBJECTS_H
#define _GAMEOBJECTS_H

#include <windows.h>
#include <gl\gl.h>
#include "GameObject.h"
#include "CollisionBox.h"

class GameObjectS : public GameObject{
protected:
	float radius;
	float angle;
	float speed, multiplier;
	static double PI;
	bool alive;
	CollisionBox box;

public:

	//CONSTRUCTORS
	GameObjectS();
	GameObjectS(int width, int height, float x, float y, GLuint texture, float speed = 2.0);

	//GETTERS
	float getAngle();
	float getSpeed();
	CollisionBox getBox();
	float getRadius();
	virtual bool isAlive();
	virtual void changeSpeed(float ds);

	//SETTERS
	void setAngle(float a);
	void setMultiplier(float s);

	//OTHER METHODS
	virtual void move(double delta);
	virtual void update(double delta);
	virtual void turn(float x, float y);
	virtual void draw();

};


#endif _GAMEOBJECTS_H