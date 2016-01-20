#ifndef _SHURIKEN_H
#define _SHURIKEN_H

#include <windows.h>		
#include <gl\gl.h>			
#include <gl\glu.h>

class Shuriken{
private:
	float x, y;
	float speed_x;
	float speed_y;
	float damage;
	float angle;
	bool fired;
	double PI;
	GLuint texture;
	int width, height, radius;

public:

	//CONSTRUCTORS
	Shuriken();
	Shuriken(float x, float y, float a, float d);

	//GETTERS
	float getX();
	float getY();
	float getSpeedX();
	float getSpeedY();
	float getDamage();
	float getAngle();
	int getRadius();
	int getWidth();
	int getHeight();
	bool isFired();

	//SETTERS
	void setSpeedX(float x);
	void setSpeedY(float y);
	void setDamage(float x);
	void setX(float x);
	void setY(float y);
	void setAngle(float o);
	void setTexture(GLuint);

	//OTHER METHODS
	void draw();
	void update(double delta);
	void kill();

	//STATIC
	static int xMax;
	static int yMax;

};


#endif _SHURIKEN_H