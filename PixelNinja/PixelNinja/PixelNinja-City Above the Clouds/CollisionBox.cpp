#include "CollisionBox.h"

#include <windows.h>
#include <gl\gl.h>			
#include <gl\glu.h>

CollisionBox::CollisionBox(){}

CollisionBox::CollisionBox(float x, float y, float w, float h){
	this->left = x;
	this->bottom = y;
	this->right = x + w;
	this->top = y + h;
	this->width = w;
	this->height = h;
}

//GETTERS
float CollisionBox::getLeft(){ return this->left; }
float CollisionBox::getRight(){ return this->right; }
float CollisionBox::getTop(){ return this->top; }
float CollisionBox::getBottom(){ return this->bottom; }
float CollisionBox::getWidth(){ return this->width; }
float CollisionBox::getHeight(){ return this->height; }

//SETTERS
void CollisionBox::setX(float x){
	this->left = x;
	this->right = this->left + this->width;
}

void CollisionBox::setY(float y){
	this->bottom = y;
	this->top = this->bottom + this->height;
}

//OTHER METHODS
void CollisionBox::draw(){
	glLineWidth(1.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2f(this->left, this->bottom);
	glVertex2f(this->left, this->top);
	glVertex2f(this->right, this->top);
	glVertex2f(this->right, this->bottom);
	glEnd();

}

