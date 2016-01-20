#ifndef _NINJA_H
#define _NINJA_H

class Monster;
#include "GameObjectS.h"

#include "Shuriken.h"
#include "CollisionBox.h"
#include "Monster.h"
#include "Point2D.h"
#include <vector>

class Ninja : public GameObjectS
{
private:

	int health;
	int maxHealth;
	int damage;
	float MAX_SPEED;
	float acceleration;
	std::vector<Shuriken> clip;
	int totalAmmo;
	int clipSize;
	int killCount;
	bool collides;
	bool shoot;
	bool jump;
	double attackCooldown;
	GLuint shuriken_texture;

public:
	//CONSTRUCTORS
	Ninja();
	Ninja(int width, int height, float x, float y, GLuint texture, float speed = 0);
	Ninja(int width, int height, float x, float y);

	//GETTERS
	float getMaxSpeed();
	float getAngle();
	int getHealth();
	int getMaxHealth();
	bool isAlive();
	int getDamage();
	int getTotalAmmo();
	int getCurrentClipSize();
	int getKillCount();
	bool getIsJumping();
	bool isColliding();

	//SETTERS
	void  setX(float ax);
	void setShurikenTexture(GLuint);
	void setMaxHealth(int hp);
	void setMaxSpeed(float ms);
	void changeHealth(int dyhealth);
	void changeSpeed(float ds);
	void addAmmo(int amount);
	void changeDamage(int dyd);
	void setJumping(bool jump);

	//OTHER METHODS
	void move(float magnitude, char direction);
	void fire(float mouse_x, float mouse_y);
	void reload();
	void update(double delta, float x, float y);
	void turn(float x, float y);
	void updateShurikens(double delta, float mouse_x, float mouse_y);
	void updateFiring(double delta);
	void draw();
	void drawPlayer();
	void drawShurikens();
	bool checkBoxes(std::vector<CollisionBox>, double, char);
	bool checkShurikenCollision(std::vector<Monster>*, std::vector<CollisionBox>*);


};
#endif _NINJA_H