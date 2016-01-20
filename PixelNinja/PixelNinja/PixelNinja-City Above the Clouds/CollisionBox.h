#ifndef _COLLISIONBOX_H
#define _COLLISIONBOX_H

class CollisionBox{
private:
	float left, right, bottom, top;
	float width, height;
public:
	//CONSTRUCTORS
	CollisionBox();
	CollisionBox(float, float, float, float);

	//GETTERS
	float getLeft();
	float getRight();
	float getTop();
	float getBottom();
	float getWidth();
	float getHeight();

	//SETTERS
	void setX(float x);
	void setY(float y);

	//OTHER METHODS
	void draw();
};

#endif _COLLISIONBOX_H