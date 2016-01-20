#include "GameObjectS.h"


#include <windows.h>		
#include <gl\gl.h>			
#include <gl\glu.h>			
#include <math.h>

double GameObjectS::PI = 3.1415926535897932384626433832795;


GameObjectS::GameObjectS(){}

GameObjectS::GameObjectS(int width, int height, float x, float y, GLuint texture, float s) : GameObject(x, y, width, height, texture){
	this->speed = s;

	if (width > height)
		this->radius = width / 2;
	else
		this->radius = height / 2;
}

void GameObjectS::changeSpeed(float s){ this->speed += s; }
void GameObjectS::setMultiplier(float ds){ this->multiplier = ds; }
void GameObjectS::setAngle(float angle){ this->angle = angle; }

bool GameObjectS::isAlive(){ return this->alive; }
float GameObjectS::getAngle(){ return this->angle; }
float GameObjectS::getSpeed(){ return this->speed; }
float GameObjectS::getRadius(){ return this->radius; }
CollisionBox GameObjectS::getBox(){ return this->box; }

void GameObjectS::update(double delta){ this->move(delta); }
void GameObjectS::turn(float x, float y){}
void GameObjectS::move(double delta){}



void GameObjectS::draw(){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->texture);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix();


	glBegin(GL_QUADS);
	glColor3f(0.5, 0.5, 1.0);
	glTexCoord2f(0.0, 0.0); glVertex2f(this->x, this->y);
	glTexCoord2f(0.0, 1.0); glVertex2f(this->x, this->getY2());
	glTexCoord2f(1.0, 1.0); glVertex2f(this->getX2(), this->getY2());
	glTexCoord2f(1.0, 0.0); glVertex2f(this->getX2(), this->y);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	glDisable(GL_BLEND);
	glPopMatrix();
}
