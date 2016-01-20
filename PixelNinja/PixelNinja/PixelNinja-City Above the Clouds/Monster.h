#ifndef _ZOMBIE_H
#define _ZOMBIE_H

class Ninja;
class Interface;

#include "GameObjectS.h"
#include "Ninja.h"
#include "Interface.h"
#include "CollisionBox.h"
#include <vector>

class Monster : public GameObjectS{
private:
	int damage;
	float speed;
	int points;
	int health;
	int maxHealth;
	float cooldown;
	bool readyToAttack;
	void drawMonster();
	Interface* bar;

public:
	//CONSTRUCTORS
	Monster();
	Monster(int width, int height, float x, float y, GLuint texture);
	Monster(int width, int height, float x, float y);

	//GETTERS
	int getMaxHealth();
	int getHealth();
	float getAttackTimer();
	bool isAlive();
	bool canAttack();
	bool isPlayerColliding(Ninja* p);
	bool checkBoxes(std::vector<CollisionBox>*, float, float);

	//SETTERS
	void setTexture(GLuint tx);
	void changeHealth(int amount);

	//OTHER METHODS
	void kill();
	void move(double delta);
	void turn(float x, float y);
	void update(double delta, float x, float y, std::vector<CollisionBox>*);
	void draw();
	void readyAttack(double delta);
	void attack(Ninja* p);

};


#endif _ZOMBIE_H