#include "Interface.h"

#include <windows.h>		
#include <gl\gl.h>			
#include <gl\glu.h>			
#include <math.h>

Interface::Interface(){}

Interface::Interface(float x, float y, int width, int height){
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->value = 0;
	this->maxvalue = 0;
}

//GETTERS
float Interface::getX(){ return this->x; }
float Interface::getY(){ return this->y; }
int Interface::getWidth(){ return this->width; }
int Interface::getHeight(){ return this->height; }
int Interface::getValue(){ return this->value; }
int Interface::getMaxValue(){ return this->maxvalue; }

//SETTERS
void Interface::setMaxValue(int x){ this->maxvalue = x; }
void Interface::setX(float x){ this->x = x; }
void Interface::setY(float y){ this->y = y; }
void Interface::setWidth(int w){ this->width = w; }
void Interface::setHeight(int h){ this->height = h; }
void Interface::setValue(int v){ this->value = v; }

//OTHER METHODS
void Interface::draw(){
	glLoadIdentity();
	glPushMatrix();
	glBegin(GL_QUADS);
	glColor3f(0.7f, 0.7f, 0.7f);
	glVertex2f(this->x, this->y - 1);
	glVertex2f(this->x, this->y + this->height + 1);
	glVertex2f(this->x + this->width, this->y + this->height);
	glVertex2f(this->x + this->width, this->y);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBegin(GL_QUADS);
	glColor3ub(red, green, blue);
	glVertex2f(this->x, this->y);
	glVertex2f(this->x, this->y + this->height);
	glVertex2f(this->x + this->value, this->y + this->height);
	glVertex2f(this->x + this->value, this->y);
	glColor3f(0, 0, 0);
	glEnd();
	glPopMatrix();

	glLineWidth(1.0f);
	glPushMatrix();
	glBegin(GL_LINE_LOOP);
	glVertex2f(this->x, this->y - 1);
	glVertex2f(this->x, this->y + this->height + 1);
	glVertex2f(this->x + this->width, this->y + this->height);
	glVertex2f(this->x + this->width, this->y);
	glEnd();
	glPopMatrix();
}

//UPDATE METHODS
void Interface::update(Ninja* p){


	this->value = p->getHealth();
	this->maxvalue = p->getMaxHealth();

	this->value = (this->value * this->width) / this->maxvalue;

	green = (this->value * 255) / this->width;
	red = 255 - green;
	blue = 0;

}




