#ifndef _BOUNDARYLINE_H
#define _BOUNDARYLINE_H


#include "BBox.h"
#include "Point.h"


class BoundaryLine{
private:
	Point A, B;
	BBox box;
	int type;
	int trackside;
public:

	BoundaryLine(float, float, float, float, int);
	BoundaryLine(Point, Point, int);
	BBox getBox();
	Point getA();
	Point getB();
	int getType();
	int getTrackside();

	void draw();
};

#endif _BOUNDARYLINE_H