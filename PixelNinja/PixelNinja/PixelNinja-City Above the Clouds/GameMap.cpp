#include "GameMap.h"

#include <windows.h>		
#include <gl\gl.h>			
#include <gl\glu.h>			
#include <math.h>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

GameMap::GameMap(){
	this->width = 800;
	this->height = 800;
}

GameMap::GameMap(int width, int height){
	this->width = width;
	this->height = height;
}

//GETTERS
int GameMap::getHeight(){ return this->height; }
int GameMap::getWidth(){ return this->width; }
std::vector<CollisionBox> GameMap::getColBoxes(){ return this->collisionBoxes; }

//SETTERS
void GameMap::setBgTexture(GLuint id){ this->bg_texture = id; }
void GameMap::setColBoxes(){

	//floor
	CollisionBox box0 = CollisionBox(0, 0, 900, 50);
	collisionBoxes.push_back(box0);
	//floor2
	CollisionBox box1 = CollisionBox(1200, 0, 960, 50);
	collisionBoxes.push_back(box1);
	//home roof
	CollisionBox box2 = CollisionBox(1480, 50, 200, 190);
	collisionBoxes.push_back(box2);
	// finish
	CollisionBox box4 = CollisionBox(240, 890, 280, 30);
	collisionBoxes.push_back(box4);

}

//OTHER METHODS
void GameMap::draw(){

	this->drawBackground();

}
void GameMap::drawBackground(){

	glLoadIdentity();
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, bg_texture);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex2f(0, 0);
	glTexCoord2f(0.0, 1.0); glVertex2f(0, this->height);
	glTexCoord2f(1.0, 1.0); glVertex2f(this->width, this->height);
	glTexCoord2f(1.0, 0.0); glVertex2f(this->width, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glPopMatrix();
}
void GameMap::drawColBoxes(){
	for (int i = 0; i < collisionBoxes.size(); i++){
		collisionBoxes[i].draw();
	}
}

