#include "Ninja.h"

#include <windows.h>		
#include <gl\gl.h>			
#include <gl\glu.h>			
#include <math.h>

#include <iostream>
using namespace std;


Ninja::Ninja(){}
Ninja::Ninja(int width, int height, float x, float y, GLuint texture, float speed) :GameObjectS(width, height, x, y, texture, speed){

	this->damage = 7;
	this->maxHealth = 100;
	this->health = maxHealth;
	this->totalAmmo = 75;
	this->clipSize = 10;
	this->speed = speed;
	this->MAX_SPEED = 0.05f;
	this->acceleration = 0.025f;
	this->multiplier = 1.0f;
	this->box = CollisionBox(x - width / 2, y - height / 2, width, height);
	this->collides = false;
	this->shoot = true;
	this->jump = false;
	this->attackCooldown = 0;
	this->killCount = 0;
	this->center = Point2D(x, y);
	this->shuriken_texture = 0;

}
Ninja::Ninja(int width, int height, float x, float y) :GameObjectS(width, height, x, y, 0, 0){

	this->damage = 7;
	this->maxHealth = 100;
	this->health = maxHealth;
	this->totalAmmo = 75;
	this->clipSize = 10;
	this->speed = 0.00250f;
	this->MAX_SPEED = 0.0005f;
	this->acceleration = 0.00025f;
	this->multiplier = 1.0f;
	this->box = CollisionBox(x - width / 2, y - height / 2, width, height);
	this->collides = false;
	this->shoot = true;
	this->jump = false;
	this->attackCooldown = 0;
	this->killCount = 0;
	this->center = Point2D(x, y);
	this->shuriken_texture = 0;

}

void Ninja::drawPlayer(){
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->texture);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix();
	glTranslatef(x, y, 0);
	if (angle > 90){
		//flips character
		glRotatef(90, 0, 0, 1);
		glScalef(-1.0f, 1.0f, 1.0f);

	}
	else
		glRotatef(270, 0, 0, 1);
	glTranslatef(-(x + this->getWidth() / 2), -(y + this->getHeight() / 2), 0);

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
void Ninja::draw(){
	//draws Shurikens
	glPushMatrix();
	this->drawShurikens();
	glPopMatrix();

	//draws players
	glPushMatrix();
	this->drawPlayer();
	glPopMatrix();


}
bool Ninja::checkBoxes(std::vector<CollisionBox> boxes, double delta, char direction){
	float temp = this->multiplier*(this->speed*delta);

	for (int i = 0; i<boxes.size(); i++){

		switch (direction){
		case 'U':
			if (this->getBox().getBottom() + temp < boxes.at(i).getTop() &&
				this->getBox().getTop() + temp > boxes.at(i).getBottom() &&
				this->getBox().getRight() > boxes.at(i).getLeft() &&
				this->getBox().getLeft() < boxes.at(i).getRight()){

				this->collides = true;
				this->move(temp * 4, 'U');
			}
			else{
				this->collides = false;
			}
			this->move(temp / 2, direction);
			break;
		case 'D':
			if (this->getBox().getBottom() - temp < boxes.at(i).getTop() &&
				this->getBox().getTop() - temp > boxes.at(i).getBottom() &&
				this->getBox().getRight() > boxes.at(i).getLeft() &&
				this->getBox().getLeft() < boxes.at(i).getRight()){

				this->collides = true;


			}
			else{
				this->collides = false;

			}
			this->move(temp, direction);
			break;
		case 'L':
			if (this->getBox().getBottom() < boxes.at(i).getTop() &&
				this->getBox().getTop() > boxes.at(i).getBottom() &&
				this->getBox().getRight() - temp > boxes.at(i).getLeft() &&
				this->getBox().getLeft() - temp < boxes.at(i).getRight()){

				this->collides = true;
				this->move(temp * 2, 'L');

			}
			else{
				this->collides = false;

			}
			this->move(temp, direction);
			break;
		case 'R':
			if (this->getBox().getBottom() < boxes.at(i).getTop() &&
				this->getBox().getTop() > boxes.at(i).getBottom() &&
				this->getBox().getRight() + temp > boxes.at(i).getLeft() &&
				this->getBox().getLeft() + temp < boxes.at(i).getRight()){

				this->collides = true;
				this->move(temp * 2, 'R');

			}
			else{
				this->collides = false;

			}
			this->move(temp, direction);
			break;
		}
	}

	return this->collides;
}

//GETTERS
float Ninja::getMaxSpeed(){ return this->MAX_SPEED; }
bool Ninja::isAlive(){ return true; }
float Ninja::getAngle(){ return this->angle; }
bool Ninja::getIsJumping(){ return this->jump; }
int Ninja::getCurrentClipSize(){ return this->clipSize; }
int Ninja::getTotalAmmo(){ return this->totalAmmo; }
bool Ninja::isColliding(){ return this->collides; }
int Ninja::getDamage(){ return this->damage; }
int Ninja::getKillCount(){ return this->killCount; }
int Ninja::getHealth(){ return this->health; }
int Ninja::getMaxHealth(){ return this->maxHealth; }


//SETTERS
void Ninja::changeHealth(int x){
	if ((this->health + x) <= this->maxHealth)
		this->health += x;
	else
		this->health = this->maxHealth;

	if (this->health <= 0)
		this->alive = false;
}
void Ninja::setMaxHealth(int hp){
	this->maxHealth = hp;
	int difhp = this->getHealth() - this->maxHealth;

	this->changeHealth(difhp);
}
void Ninja::changeSpeed(float ds){
	if (ds > 0){
		if (ds < this->MAX_SPEED)
			this->speed = ds;
		else
			this->speed = this->MAX_SPEED;
	}
}
void Ninja::drawShurikens(){
	for (int i = 0; i < clip.size(); i++){
		clip.at(i).draw();
	}
}
void Ninja::setX(float x){ this->x = x; }
void Ninja::addAmmo(int amount){ this->totalAmmo += amount; }
void Ninja::changeDamage(int dyd){ this->damage += dyd; }
void Ninja::setJumping(bool jum){ this->jump = jum; }
void Ninja::setShurikenTexture(GLuint shuriken_tex){ this->shuriken_texture = shuriken_tex; }


//OTHER METHODS
void Ninja::move(float move_dy, char direction){

	switch (direction){
	case 'U':
		if (!this->collides){
			this->y += move_dy;
			this->box.setY(this->y - this->height / 2);
		}
		else{
			this->y -= 0.65;
			this->box.setY(this->y - this->height / 2);
		}
		break;
	case 'D':
		if (!this->collides){
			this->y -= move_dy;
			this->box.setY(this->y - this->height / 2);
		}
		else{
			this->y += 0.65;
			this->box.setY(this->y - this->height / 2);
		}
		break;
	case 'L':
		if (!this->collides){
			this->x -= move_dy;
			this->box.setX(this->x - this->width / 2);
		}
		else{
			this->x += 0.65;
			this->box.setX(this->x - this->width / 2);
		}
		break;
	case 'R':
		if (!this->collides){
			this->x += move_dy;
			this->box.setX(this->x - this->width / 2);
		}
		else{
			this->x -= 0.65;
			this->box.setX(this->x - this->width / 2);
		}
		break;
	}

}
void Ninja::turn(float mouse_x, float mouse_y){
	float dify = mouse_y - 240;
	float difx = mouse_x - 240;

	this->setAngle(atan2f(dify, difx)*(180.0f / PI));
}
void Ninja::fire(float mouse_x, float mouse_y){
	if (this->shoot){
		if (this->clipSize > 0){
			Shuriken b = Shuriken(this->x, this->y, this->angle, this->getDamage());
			b.setTexture(this->shuriken_texture);
			this->clip.push_back(b);
			this->clipSize--;
		}
		this->shoot = false;
	}

}
void Ninja::reload(){
	if (this->totalAmmo > 0){
		if (this->clipSize == 0){
			clip.clear();
			if (this->totalAmmo > 10){
				clipSize = 10;
				this->totalAmmo -= 10;
			}
			else{
				this->clipSize = this->totalAmmo;
				this->totalAmmo = 0;
			}
			this->shoot = true;
		}

	}
}
bool Ninja::checkShurikenCollision(std::vector<Monster>* monsters, std::vector<CollisionBox>* boxes){

	for (int i = 0; i < monsters->size(); i++){
		if (!monsters->at(i).isAlive())
			continue;
		for (int k = 0; k < this->clip.size(); k++){
			if (this->clip.at(k).isFired()){
				float difx = this->clip.at(k).getX() - monsters->at(i).getX();
				float dify = this->clip.at(k).getY() - monsters->at(i).getY();

				float r = clip.at(k).getRadius() + monsters->at(i).getRadius();
				r = r*r;
				float dist = difx*difx + dify*dify;

				if (dist <= r){
					monsters->at(i).changeHealth(-1 * (this->clip.at(k).getDamage()));
					this->clip.at(k).kill();
					if (monsters->at(i).getHealth() <= 0){
						monsters->at(i).kill();
						this->killCount++;
					}
					return true;
				}
				else
					continue;
			}
			else
				continue;
		}
	}


	for (int i = 0; i < boxes->size(); i++){
		for (int k = 0; k < this->clip.size(); k++){
			if (this->clip.at(k).isFired()){
				if (boxes->at(i).getLeft() <= this->clip.at(k).getX() &&
					boxes->at(i).getRight() >= this->clip.at(k).getX() &&
					boxes->at(i).getBottom() <= this->clip.at(k).getY() &&
					boxes->at(i).getTop() >= this->clip.at(k).getY()){
					this->clip.at(k).kill();
					return true;
				}
				else
					continue;
			}
			else
				continue;
		}
	}
	return false;
}


//UPDATE METHODS
void Ninja::update(double delta, float x, float y){
	this->updateFiring(delta);
	this->updateShurikens(delta, x, y);

	this->turn(x, y);


}
void Ninja::updateFiring(double delta){
	if (!this->shoot){
		this->attackCooldown += delta;

	}

	if (this->attackCooldown > 10000){
		this->shoot = true;
		this->attackCooldown = 0;
	}

}
void Ninja::updateShurikens(double delta, float mouse_x, float mouse_y){
	for (int i = 0; i < clip.size(); i++){
		if (clip.at(i).isFired() == true){
			clip.at(i).update(delta);
		}
	}
}










