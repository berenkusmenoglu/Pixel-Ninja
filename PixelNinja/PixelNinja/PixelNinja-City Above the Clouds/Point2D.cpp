#include "Point2D.h"

#include <windows.h>
#include <gl\gl.h>			
#include <gl\glu.h>	


Point2D::Point2D(){}
Point2D::Point2D(float x, float y){ this->x = x; this->y = y; }

//GETTERS
float Point2D::getX(){ return this->x; }
float Point2D::getY(){ return this->y; }

//SETTERS
void Point2D::setX(float x){ this->x = x; }
void Point2D::setY(float y){ this->y = y; }

//OTHER METHODS
void Point2D::draw(){}