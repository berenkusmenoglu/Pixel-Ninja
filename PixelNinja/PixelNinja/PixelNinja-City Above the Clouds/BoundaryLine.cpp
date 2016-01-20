#include "BoundaryLine.h"

#include <windows.h>
#include <gl\gl.h>			
#include <gl\glu.h>	

BoundaryLine::BoundaryLine(float ax, float ay, float bx, float by, int trackside){
	this->A.setX(ax);
	this->A.setY(ay);
	this->B.setX(bx);
	this->B.setY(by);
	this->trackside = trackside;

	float w;
	float h;
	float x1, y1;

	//on the same line vertical line
	if (A.getX() == B.getX()){
		this->type = 1;
		x1 = A.getX() - 5;
		w = 10;
		y1 = A.getY();
		h = B.getY() - A.getY();
	}// horizontal line
	else if (A.getY() == B.getY()){
		this->type = 2;
		y1 = A.getY() - 5;
		h = 10;
		w = B.getX() - A.getX();
		x1 = A.getX();

	}// inclined line
	else{
		//down inclined
		if (A.getY() > B.getY()){
			this->type = 3;
			x1 = A.getX();
			y1 = B.getY();
			w = B.getX() - A.getX();
			h = A.getY() - B.getY();
		}
		//up inclined
		else if (A.getY() < B.getY()){
			this->type = 4;
			x1 = A.getX();
			y1 = A.getY();
			w = B.getX() - A.getX();
			h = B.getY() - A.getY();
		}

	}

	this->box = BBox(x1, y1, w, h);

}

BoundaryLine::BoundaryLine(Point A, Point B, int trackside){
	this->A = A;
	this->B = B;
	this->trackside = trackside;

	float w;
	float h;
	float x1, y1;

	//on the same line vertical line
	if (A.getX() == B.getX()){
		this->type = 1;
		x1 = A.getX() - 5;
		w = 10;
		y1 = A.getY();
		h = B.getY() - A.getY();
	}// horizontal line
	else if (A.getY() == B.getY()){
		this->type = 2;
		y1 = A.getY() - 5;
		h = 10;
		w = B.getX() - A.getX();
		x1 = A.getX();

	}// inclined line
	else{
		if (A.getY() > B.getY()){
			this->type = 3;
			x1 = A.getX();
			y1 = B.getY();
			w = B.getX() - A.getX();
			h = A.getY() - B.getY();
		}
		else if (A.getY() < B.getY()){
			this->type = 4;
			x1 = A.getX();
			y1 = A.getY();
			w = B.getX() - A.getX();
			h = B.getY() - A.getY();
		}

	}

	this->box = BBox(x1, y1, w, h);



}

BBox BoundaryLine::getBox(){
	return this->box;
}

Point BoundaryLine::getA(){
	return this->A;
}

Point BoundaryLine::getB(){
	return this->B;
}

int BoundaryLine::getType(){
	return this->type;
}

int BoundaryLine::getTrackside(){
	return this->trackside;
}

void BoundaryLine::draw(){
	glLineWidth(1.0f);
	glBegin(GL_LINES);
	glVertex2f(this->A.getX(), this->A.getY());
	glVertex2f(this->B.getX(), this->B.getY());
	glEnd();

	this->box.draw();
}
