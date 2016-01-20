#ifndef _POINT2D_H
#define _POINT2D_H


class Point2D{
private:
	float x, y;

public:
	//CONSTRUCTORS
	Point2D();
	Point2D(float, float);

	//GETTERS
	float getX();
	float getY();

	//SETTERS
	void setX(float);
	void setY(float);

	//OTHER METHODS
	void draw();
};

#endif _POINT2D_H