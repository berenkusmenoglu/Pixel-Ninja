#include "Monster.h"

Monster::Monster(){}
Monster::Monster(int width, int height, float x, float y) :GameObjectS(width, height, x, y, 0, 0){

	this->speed = 0.00025f;
	this->damage = 35;
	this->points = 30;
	this->health = 20;
	this->maxHealth = 20;
	this->readyToAttack = true;
	this->cooldown = 0;
	this->multiplier = 1.0;
	int boxSizes;
	if (width > height){
		boxSizes = width;
		this->radius = width * 0.6f;
	}
	else
	{
		boxSizes = height;
		this->radius = height * 0.6f;
	}

	this->box = CollisionBox(x - boxSizes / 2, y - boxSizes / 2, boxSizes, boxSizes);
	this->center = Point2D(x, y);
}
Monster::Monster(int width, int height, float x, float y, GLuint texture) :GameObjectS(width, height, x, y, texture, 0){
	this->damage = 30;
	this->speed = 0.00025f;
	this->health = 20;
	this->maxHealth = 20;
	this->points = 35;
	this->readyToAttack = true;
	this->cooldown = 0;
	this->multiplier = 1.0;
	int boxSizes;
	if (width > height){
		boxSizes = width;
	}
	else{
		boxSizes = height;
		this->radius = height * 0.6f;
	}

	this->box = CollisionBox(x - boxSizes / 2, y - boxSizes / 2, boxSizes, boxSizes);
	this->center = Point2D(x, y);


}

//GETTERS
bool Monster::isAlive(){ return this->alive; }
bool Monster::canAttack(){ return this->readyToAttack; }
int Monster::getHealth(){ return this->health; }
int Monster::getMaxHealth(){ return this->health; }
float Monster::getAttackTimer(){ return this->cooldown; }

//SETTERS
void Monster::setTexture(GLuint tx){ this->texture = tx; }
void Monster::changeHealth(int amount){
	if ((this->health + amount) <= this->maxHealth)
		this->health += amount;
	else
		this->health = maxHealth;

	if (this->health <= 0)
		this->kill();
}
void Monster::kill(){ this->alive = false; }

//OTHER METHODS 
void Monster::move(double delta){
	this->x += cos(this->getAngle()*(PI / 180))*(speed*delta)*this->multiplier;
	this->box.setX(this->x - this->width / 2);
	this->y += sin(this->getAngle()*(PI / 180))*(speed*delta)*this->multiplier;
	this->box.setY(this->y - this->height / 2);
}
void Monster::turn(float monsterX, float monsterY){
	float tempAngle = 0;
	float difx = 0.0;
	float dify = 0.0;
	if (monsterX < this->x){
		if (monsterX < this->y){

			dify = this->y - monsterY;
			difx = this->x - monsterX;

			tempAngle = atan2(dify, difx)*(180.0f / PI);
			tempAngle = tempAngle - 180;
		}
		else if (monsterY > this->y){

			dify = monsterY - this->y;
			difx = this->x - monsterX;

			tempAngle = atan2(dify, difx)*(180.0f / PI);
			tempAngle = -1 * (tempAngle)-180;
		}
		else{
			tempAngle = 180;
		}
	}
	else if (monsterX > this->x){
		if (monsterY < this->y){

			dify = this->y - monsterY;
			difx = monsterX - this->x;

			tempAngle = atan2f(dify, difx)*(180.0f / PI);
			tempAngle = -1 * (tempAngle);
		}
		else if (monsterY > this->y){

			dify = monsterY - this->y;
			difx = monsterX - this->x;

			tempAngle = atan2f(dify, difx)*(180.0f / PI);
		}
		else{
			tempAngle = 360;
		}
	}
	else{
		if (monsterY < this->y){
			tempAngle = 270;
		}
		else if (monsterY > this->y){
			tempAngle = 90;
		}
	}
	this->setAngle(tempAngle);
}
void Monster::attack(Ninja* p){
	if (canAttack()){
		this->multiplier = 0;
		p->changeHealth(-1 * this->damage);
		this->readyToAttack = false;
	}
}
void Monster::readyAttack(double delta){
	if (!canAttack()){
		cooldown += (float)delta;

		if (cooldown >= 100000.0){
			readyToAttack = true;
			cooldown = 0.0;
		}
	}
}
bool Monster::isPlayerColliding(Ninja* player){
	float difx = (this->getX() - player->getX())* (this->getX() - player->getX());
	float dify = (this->getY() - player->getY()) *(this->getY() - player->getY());
	float dist = difx*difx + dify*dify;
	float r2 = (this->getRadius() + player->getRadius())*(this->getRadius() + player->getRadius());
	if (dist <= (r2))
		return true;
	return false;
}
bool Monster::checkBoxes(std::vector<CollisionBox>* boxes, float p_x, float p_y){
	CollisionBox box;

	Point2D A = Point2D(this->getX(), this->getY());
	Point2D B;
	for (int i = 0; i < boxes->size(); i++){
		box = boxes->at(i);


		if (box.getRight() < this->getBox().getLeft())
			B.setX(box.getRight());
		else if (box.getLeft() > this->getBox().getRight())
			B.setX(box.getLeft());
		else
			B.setX(this->getX());

		if (box.getTop() < this->getBox().getBottom())
			B.setY(box.getTop());
		else if (box.getBottom() > this->getBox().getTop())
			B.setY(box.getBottom());
		else
			B.setY(this->getY());


		float difx = (B.getX() - A.getX()) * (B.getX() - A.getX());
		float dify = (B.getY() - A.getY()) * (B.getY() - A.getY());

		float dist = difx + dify;
		if (dist <= this->getRadius() * this->getRadius()){


			if ((this->getBox().getBottom() > box.getTop() ||
				this->getBox().getTop() < box.getBottom())
				){
				if (this->getBox().getBottom() > box.getTop()){
					if (p_x == this->getX()){
						this->angle = 0;
					}

					else if (p_x > this->getX()){

						if (this->getY() < p_y)
							this->angle = 8;
						else
							this->angle = 0;

					}
					else {

						if (this->getY() < p_y)
							this->angle = 172;
						else
							this->angle = 180;
					}

				}
				else if (this->getBox().getTop() < box.getBottom()){
					if (p_x == this->getX()){
						this->angle = 0;
					}

					else if (p_x > this->getX()){
						if (this->getY() > p_y)
							this->angle = 352;
						else
							this->angle = 0;

					}
					else{
						if (this->getY() > p_y)
							this->angle = 188;
						else
							this->angle = 180;
					}
				}

			}
			else if ((this->getBox().getRight() < box.getLeft() || this->getBox().getLeft() > box.getRight())
				){

				float up = box.getTop() - this->getY();
				float down = this->getY() - box.getBottom();

				if (this->getBox().getRight() < box.getLeft()){
					if (p_y == this->getY()){
						this->angle = 90;
					}
					else if (p_y > this->getY()){
						if (this->getX() > p_x)
							this->turn(this->getX() - 16, box.getTop() + 32);
						else
							this->turn(this->getX(), box.getTop() + 32);
					}
					else{
						if (this->getX() > p_x)
							this->turn(this->getX() - 16, box.getBottom() - 32);
						else
							this->turn(this->getX(), box.getBottom() - 32);
					}

				}
				else if (this->getBox().getLeft() > box.getRight()){

					if (p_y == this->getY()){
						this->angle = 90;
					}
					else if (p_y > this->getY()){
						if (this->getX() < p_x)
							this->turn(this->getX() + 16, box.getTop() + 32);
						else
							this->turn(this->getX(), box.getTop() + 32);
					}
					else{
						if (this->getX() < p_x)
							this->turn(this->getX() + 16, box.getBottom() - 32);
						else
							this->turn(this->getX(), box.getBottom() - 32);
					}
				}
			}
			return true;
		}
		else
			continue;
	}
	return false;
}
void Monster::draw(){
	if (this->isAlive())
		this->drawMonster();

}
void Monster::drawMonster(){
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
	glTexCoord2f(0.0, 1.0); glVertex2f(this->x, this->getY2());
	glTexCoord2f(1.0, 1.0); glVertex2f(this->getX2(), this->getY2());
	glTexCoord2f(1.0, 0.0); glVertex2f(this->getX2(), this->y);
	glEnd();

	glBegin(GL_QUADS);


	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glPopMatrix();
}

//UPDATE METHODS
void Monster::update(double delta, float x, float y, std::vector<CollisionBox>* boxes){
	if (this->isAlive()){
		this->readyAttack(delta);
		CollisionBox box;
		if (!checkBoxes(boxes, x, y))
			this->turn(x, y);
		this->move(delta);

	}
}


