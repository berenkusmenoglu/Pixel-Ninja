#include "GameObject.h"

#include <windows.h>		
#include <gl\gl.h>			
#include <gl\glu.h>			
#include <math.h>


GameObject::GameObject(){
	x = 10;
	y = 10;
	width = 50;
	height = 50;

}
GameObject::GameObject(float x, float y, int width, int height, GLuint texid){
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->setTexture(texid);
	this->center = Point2D(x + width / 2, y + height / 2);

	if (width > height)
		this->radius = width / 2;
	else
		this->radius = height / 2;
}

//GETTERS
int GameObject::getWidth(){ return this->width; }
int GameObject::getHeight(){ return this->height; }
int GameObject::getRadius(){ return this->radius; }
float GameObject::getX(){ return this->x; }
float GameObject::getY(){ return this->y; }
Point2D GameObject::getCenter(){ return this->center; }
float GameObject::getX2(){ return this->x + this->getWidth(); }
float GameObject::getY2(){ return this->y + this->getHeight(); }

//SETTERS
void GameObject::setTexture(GLuint tex){ this->texture = tex; }

//OTHER METHODS
void GameObject::draw(){

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->texture);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex2f(this->x, this->y);
	glTexCoord2f(0.0, 1.0); glVertex2f(this->x, this->getY2());
	glTexCoord2f(1.0, 1.0); glVertex2f(this->getX2(), this->getY2());
	glTexCoord2f(1.0, 0.0); glVertex2f(this->getX2(), this->y);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	glDisable(GL_BLEND);

	glPopMatrix();


}