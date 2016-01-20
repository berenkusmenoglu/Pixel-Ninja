#ifndef _GAMEMAP_H
#define _GAMEMAP_H

#include <windows.h>		
#include <gl\gl.h>			
#include <gl\glu.h>	
#include <string>
#include "CollisionBox.h"
#include <vector>

class GameMap{
private:
	int width;
	int height;
	GLuint bg_texture;
	std::vector<CollisionBox> collisionBoxes;

public:
	//CONSTRUCTORS
	GameMap();
	GameMap(int widht, int height);

	//GETTERS
	int getWidth();
	int getHeight();
	std::vector<CollisionBox> getColBoxes();

	//SETTERS
	void setColBoxes();
	void setBgTexture(GLuint);

	//OTHER METHODS
	void draw();
	void drawBackground();
	void drawColBoxes();
};

#endif _GAMEMAP_H