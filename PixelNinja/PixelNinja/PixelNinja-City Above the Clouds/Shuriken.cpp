#include "Image_Loading/nvImage.h"
#include "Shuriken.h"

#include <fstream>
#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <math.h>
#include <vector>
#include <sstream>
#include <iostream>

#define PI 3.1415926535897932384626433832795
using namespace std;

int Shuriken::xMax = 0;
int Shuriken::yMax = 0;

GLuint shur_tex = 0;


Shuriken::Shuriken(){}
Shuriken::Shuriken(float x, float y, float angle, float damage){
	this->x = x;
	this->y = y;
	this->speed_x = 0.005f;
	this->speed_y = 0.003f;
	this->damage = damage;
	this->fired = true;
	this->angle = angle;
	this->width = 4;
	this->height = 4;
	this->texture = 0;
	if (width > height)
		this->radius = width / 2;
	else
		this->radius = height / 2;
}

//GETTERS
float Shuriken::getDamage(){ return this->damage; }
float Shuriken::getSpeedX(){ return this->speed_x; }
float Shuriken::getSpeedY(){ return this->speed_y; }
float Shuriken::getX(){ return this->x; }
float Shuriken::getY(){ return this->y; }
float Shuriken::getAngle(){ return this->angle; }
int Shuriken::getRadius(){ return this->radius; }
int Shuriken::getWidth(){ return this->width; }
int Shuriken::getHeight(){ return this->height; }
bool Shuriken::isFired(){ return this->fired; }

//SETTERS
void Shuriken::setAngle(float o){ this->angle = o; }
void Shuriken::setDamage(float ninJump){ this->damage = ninJump; }
void Shuriken::setSpeedX(float dys){ this->speed_x = dys; }
void Shuriken::setSpeedY(float dys){ this->speed_y = dys; }
void Shuriken::setX(float x){ this->x = x; }
void Shuriken::setY(float y){ this->y = y; }
void Shuriken::setTexture(GLuint tex){ this->texture = tex; }
void Shuriken::kill(){ this->fired = false; }

//OTHER METHODS
void Shuriken::draw(){

	nv::Image img2;
	if (img2.loadImageFromFile("Assets/dead.PNG"))
	{

		glGenTextures(1, &shur_tex);
		glBindTexture(GL_TEXTURE_2D, shur_tex);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D, 0, img2.getInternalFormat(), img2.getWidth(), img2.getHeight(), 0, img2.getFormat(), img2.getType(), img2.getLevel(0));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

		setTexture(shur_tex);
	}
	else
		MessageBox(NULL, "Failed to load texture", "End of the world", MB_OK | MB_ICONINFORMATION);


	if (fired == true){

		
		glLoadIdentity();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, this->texture);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPushMatrix();

		glTranslatef(x, y, 0);
		glRotatef(angle + 90, 0, 0, 1);
		glTranslatef(-(x + this->getWidth() / 2), -(y + this->getHeight() / 2), 0);

		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex2f(this->x, this->y);
		glTexCoord2f(0.0, 1.0); glVertex2f(this->x, this->y + this->getHeight());
		glTexCoord2f(1.0, 1.0); glVertex2f(this->x + this->getWidth(), this->y + this->getHeight());
		glTexCoord2f(1.0, 0.0); glVertex2f(this->x + this->getWidth(), this->y);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glPopMatrix();

	}
}

//UPDATE METHODS
void Shuriken::update(double delta){
	this->x += cos(this->angle*(PI / 180))*(speed_x*delta);
	this->y += sin(this->angle*(PI / 180))*(speed_x*delta);

	//BOUNDARIES OF THE MAP if collides with boundaries of the map gets set to false.
	if (x >= xMax || x <= 0)
		fired = false;
	if (y >= yMax || y <= 0)
		fired = false;

}