#include "Image_Loading/nvImage.h"

#include <fstream>
#include <windows.h>		
#include <gl\gl.h>			
#include <gl\glu.h>			
#include <math.h>
#include <vector>
#include <sstream>
#include "console.h"		

#include "GameObject.h"
#include "GameMap.h"
#include "Ninja.h"
#include "CollisionBox.h"
#include "Interface.h"
#include "Monster.h"
#include "Point2D.h"
#include "glfont2.h"

using namespace glfont;

//FONT AND CONSOLE
GLFont myfont;
ConsoleWindow console;

//TIME VARIABLES
float jumpTime = 1000.0f;
float walkTime = 0.0f;
float monsterTime = 0.0f;

//BOOLEAN VARIABLES
bool walking = false;
bool shooting = false;
bool finished = false;
bool startScreenActive = true;
bool gameover = false;
bool won = false;
bool lost = false;
bool pressed = false;
bool LeftPressed = false;
bool RightPressed = false;
bool keys[256];
bool gravityOn = true;

//OTHER VARIABLES
int	mouse_x = 0, mouse_y = 0;
int screenWidth = 480, screenHeight = 480;
float gravity = 09.0f;
float r = 0, g = 0, b = 0;
float timer = 0.0f;

//FUNCTION PROTOTYPES
void display(double);
void reshape(int width, int height);
void init();
void processKeys(double);
void update();
string convertFloat(float);
string convertInt(int);

//CREATING PLAYER, SHURIKEN ,MONSTER, MAP AND INTERFACE
Ninja player;
Shuriken tempShuriken;
GameMap map(1920, 1080);
Monster enemy;
Interface healthbar = Interface(125, 8, 240, 15);

//TEXTURES
GLuint ninja_tex = 0;
GLuint shuriken_tex = 0;
GLuint bg_tex = 0;
GLuint monster_tex = 0;
GLuint game_over_tex = 0;
GLuint start_game_tex = 0;

//MONSTER
int summoned_monsters = 0;
int MAX_MONSTER_NUMBER = 16;
int spawn_order = 0;

//CREATING SPAWN POINTS
Point2D* spawn_points[4];
Point2D monster_spawn_1;
Point2D monster_spawn_2;
Point2D monster_spawn_3;
Point2D monster_spawn_4;

//COLLECTION OF MONSTERS
std::vector<Monster> monsters;
double monsterSpawnTimer;
bool spawnMonsters = false;

//OPENGL FUNCTIONS
void display(double delta)
{
	//GAME CONDITIONS WIN-LOSE
	//START SCREEN
	if (startScreenActive){
		glLoadIdentity();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, start_game_tex);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPushMatrix();

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, (float)screenHeight);
		glTexCoord2f(1.0f, 1.0f); glVertex2f((float)screenWidth, (float)screenHeight);
		glTexCoord2f(1.0f, 0.0f); glVertex2f((float)screenWidth, 0.0f);
		glEnd();

		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);

	}

	//GAMEPLAY START
	else if (!gameover){

		timer += 0.001f;

		if (player.getX() < 50){
			player.setX(1860);

		}
		else if (player.getX() > 1885){
			player.setX(55);
		}


		//FOR ANIMATED TEXTURES
		walkTime++;
		if (walkTime >= 200){
			walkTime = 0;
		}

		//STARTING THE CAMERA (CAMERA ALWAYS FOLLOWS THE PLAYER)
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		float x, x2, y, y2;
		x = player.getX() - 240;
		x2 = player.getX() + 240;
		y = player.getY() - 240;
		y2 = player.getY() + 240;
		gluOrtho2D(x, x2, y, y2);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//DRAWING THE MAP
		r += 0.0002f;
		g += 0.0002f;
		b += 0.0002f;
		glColor3f(r, g, b);
		map.draw();

		//CREATING THE MONSTERS AND UPDATING THEIR LOCATION ACCORDING TO THE PLAYER
		//IF THEY ARE COLLIDING WITH THE PLAYER THEY ATTACK

		for (size_t i = 0; i < monsters.size(); i++){

			if (monsters.at(i).isAlive()){
				monsters.at(i).update(delta, player.getX(), player.getY(), &map.getColBoxes());
				monsters.at(i).draw();

				if (!monsters.at(i).isPlayerColliding(&player)){
					monsters.at(i).setMultiplier(1.5);
				}
				else{
					monsters.at(i).attack(&player);
				}
			}
			else
				continue;
		}

		if (!spawnMonsters){
			monsterSpawnTimer += delta;
		}

		if (monsterSpawnTimer > 140000 && summoned_monsters < MAX_MONSTER_NUMBER){
			Monster enemy = Monster(32, 32, spawn_points[spawn_order]->getX(), spawn_points[spawn_order]->getY(), monster_tex);
			monsters.push_back(enemy);
			summoned_monsters++;
			spawn_order++;

			if (spawn_order > 3)
				spawn_order = 0;

			monsterSpawnTimer = 0;
		}

		//UPDATING AND DRAWING THE PLAYER
		player.update(delta, (float)mouse_x, (float)mouse_y);
		player.draw();

		//APPLYING GRAVITATIONAL FORCE ONTO THE PLAYER
		if (gravityOn)
			player.checkBoxes(map.getColBoxes(), gravity, 'D');

		//CHEKING THE SHURIKENS AGAINNST THE BOXES IN THE MAP
		player.checkShurikenCollision(&monsters, &map.getColBoxes());

		//DRAWING THE USER INTERFACE 
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, screenWidth, 0, screenHeight);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glDisable(GL_CULL_FACE);

		glClear(GL_DEPTH_BUFFER_BIT);

		//TEXT-HUD
		glPushMatrix();
		glPushMatrix();
		glBegin(GL_QUADS);
		glColor3f(0.2f, 0.3f, 0.55f);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(0.0f, 50.0f);
		glVertex2f((float)screenWidth, 50.0f);
		glVertex2f((float)screenWidth, 0.0f);
		glEnd();
		glPopMatrix();

		//DRAWING THE HEALTH BAR FOR THE PLAYER 
		healthbar.update(&player);
		healthbar.draw();

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);

		float colorset1[3] = { 1, 1, 1 };
		float colorset2[3] = { 0, 0, 0 };

		myfont.Begin();
		string time = "Time: ";
		time += convertFloat(timer);
		time += "s";
		string hp = "HP: ";
		hp += convertInt(player.getHealth());
		hp += "/";
		hp += convertInt(player.getMaxHealth());
		string ammo = "Ammo: ";
		ammo += convertInt(player.getCurrentClipSize());
		ammo += " (";
		ammo += convertInt(player.getTotalAmmo());
		ammo += ")";

		string ninJump = "NinJump Level: ";
		ninJump += convertInt(player.getKillCount() + 1);

		//DRAWING THE TEXTS ON THE SCREEN
		myfont.DrawString(time, 0.6f, 190.0f, 50.0f, colorset1, colorset1);
		myfont.DrawString(hp, 0.6f, 10.0f, 26.0f, colorset1, colorset1);
		myfont.DrawString(ammo, 0.6f, 360.0f, 50.0f, colorset1, colorset1);
		myfont.DrawString(ninJump, 0.6f, 10.0f, 50.0f, colorset1, colorset1);

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glColor3f(1, 1, 1);
		glPopMatrix();

		glPopMatrix();


	}
	else{
		glLoadIdentity();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, game_over_tex);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPushMatrix();

		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex2f(0, 0);
		glTexCoord2f(0.0, 1.0); glVertex2f(0, (float)screenHeight);
		glTexCoord2f(1.0, 1.0); glVertex2f((float)screenWidth, (float)screenHeight);
		glTexCoord2f(1.0, 0.0); glVertex2f((float)screenWidth, 0);
		glEnd();

		glPopMatrix();

		float colorset1[3] = { 1.0, 1.0, 1.0 };
		float colorset2[3] = { 1.0, 1.0, 1.0 };

		myfont.Begin();

		string msg;

		if (won){
			msg = "YOU WON";
			string score = "YOU'VE KILLED ";
			score += convertInt(player.getKillCount());
			score += " MONSTERS!";

			string timeScore = "YOUR TIME IS: ";
			timeScore += convertFloat(timer);
			timeScore += " seconds!";

			myfont.DrawString(score, 0.6f, 115.0f, 275.0f, colorset1, colorset2);
			myfont.DrawString(msg, 0.6f, 190.0f, 300.0f, colorset1, colorset2);
			myfont.DrawString(timeScore, 0.6f, 120.f, 240.0f, colorset1, colorset2);
		}
		else if (lost){
			msg = "GAME OVER";
			string score = "YOU'VE KILLED ";
			score += convertInt(player.getKillCount());
			score += " MONSTERS!";

			string timeScore = "YOUR TIME IS: ";
			timeScore += convertFloat(timer);
			timeScore += " seconds!";

			myfont.DrawString(score, 0.6f, 115.0, 275.0f, colorset1, colorset2);
			myfont.DrawString(msg, 0.6f, 190.0f, 300.0f, colorset1, colorset2);
			myfont.DrawString(timeScore, 0.6f, 120.f, 240.0f, colorset1, colorset2);
		}


		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);



		glFlush();

	}
}
void update(){

	//DROWNING OR GETTING KILLED BY MONSTERS GAME LOST
	if (player.getY() < -5 || player.getHealth() <= 0){
		gameover = true;
		won = false;
		lost = true;
	}

	//FINAL CHECKPOINT GAME WON
	if (player.getY() >= 934 && player.getY() <= 942
		&& player.getX() >= 362 && player.getX() <= 364){
		lost = false;
		won = true;
		gameover = true;
	}

	//NINJUMP TIMER
	if (player.getIsJumping()){
		jumpTime -= 5;
	}
	//JUMPABLE AREAS 
	else if ((player.getY() >= 67 && player.getY() <= 68.5) || (player.getY() >= 267 && player.getY() <= 269 && player.getX() >= 1200)){
		//NINJUMP = (KILLCOUNT + 1) * 1000;
		jumpTime = 1000.0f * (player.getKillCount() + 1.0f);
	}


	//PLAYER ANIMATED TEXTURES
	nv::Image aniTex;

	//WALKING ANIMATION
	if (walking){
		if (walkTime < 50){
			if (aniTex.loadImageFromFile("Assets/walk_1.png"))
			{
				glGenTextures(1, &ninja_tex);
				glBindTexture(GL_TEXTURE_2D, ninja_tex);
				glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
				glTexImage2D(GL_TEXTURE_2D, 0, aniTex.getInternalFormat(), aniTex.getWidth(), aniTex.getHeight(), 0, aniTex.getFormat(), aniTex.getType(), aniTex.getLevel(0));
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

			}

		}
		else if (walkTime <= 100){
			if (aniTex.loadImageFromFile("Assets/walk_2.png"))
			{
				glGenTextures(1, &ninja_tex);
				glBindTexture(GL_TEXTURE_2D, ninja_tex);
				glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
				glTexImage2D(GL_TEXTURE_2D, 0, aniTex.getInternalFormat(), aniTex.getWidth(), aniTex.getHeight(), 0, aniTex.getFormat(), aniTex.getType(), aniTex.getLevel(0));
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

			}

		}
		else if (walkTime <= 150){
			if (aniTex.loadImageFromFile("Assets/walk_3.png"))
			{
				glGenTextures(1, &ninja_tex);
				glBindTexture(GL_TEXTURE_2D, ninja_tex);
				glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
				glTexImage2D(GL_TEXTURE_2D, 0, aniTex.getInternalFormat(), aniTex.getWidth(), aniTex.getHeight(), 0, aniTex.getFormat(), aniTex.getType(), aniTex.getLevel(0));
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

			}

		}
		else if (walkTime <= 200){
			if (aniTex.loadImageFromFile("Assets/walk_4.png"))
			{
				glGenTextures(1, &ninja_tex);
				glBindTexture(GL_TEXTURE_2D, ninja_tex);
				glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
				glTexImage2D(GL_TEXTURE_2D, 0, aniTex.getInternalFormat(), aniTex.getWidth(), aniTex.getHeight(), 0, aniTex.getFormat(), aniTex.getType(), aniTex.getLevel(0));
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

			}

		}

	}
	//JUMPING TEXTURE
	if (player.getIsJumping()){
		if (aniTex.loadImageFromFile("Assets/jump.png"))
		{
			glGenTextures(1, &ninja_tex);
			glBindTexture(GL_TEXTURE_2D, ninja_tex);
			glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
			glTexImage2D(GL_TEXTURE_2D, 0, aniTex.getInternalFormat(), aniTex.getWidth(), aniTex.getHeight(), 0, aniTex.getFormat(), aniTex.getType(), aniTex.getLevel(0));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

		}


	}
	//ATTACK TEXTURE
	if (LeftPressed){
		if (aniTex.loadImageFromFile("Assets/attack.png"))
		{
			glGenTextures(1, &ninja_tex);
			glBindTexture(GL_TEXTURE_2D, ninja_tex);
			glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
			glTexImage2D(GL_TEXTURE_2D, 0, aniTex.getInternalFormat(), aniTex.getWidth(), aniTex.getHeight(), 0, aniTex.getFormat(), aniTex.getType(), aniTex.getLevel(0));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

		}

	}
	//IDLE TEXTURE
	else{
		if (aniTex.loadImageFromFile("Assets/idle.png"))
		{
			glGenTextures(1, &ninja_tex);
			glBindTexture(GL_TEXTURE_2D, ninja_tex);
			glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
			glTexImage2D(GL_TEXTURE_2D, 0, aniTex.getInternalFormat(), aniTex.getWidth(), aniTex.getHeight(), 0, aniTex.getFormat(), aniTex.getType(), aniTex.getLevel(0));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

		}

	}
	player.setTexture(ninja_tex);
	//MAKING THE CHARACTER IDLE
	walking = false;
	player.setJumping(false);
}
void reshape(int width, int height)								 // Resize the OpenGL window
{
	screenWidth = width;											 // to ensure the mouse coordinates match 
	screenHeight = height;									 	 // we will use these values to set the coordinate system
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);								// Select The Projection Matrix
	glLoadIdentity();											 // Reset The Projection Matrix		
	gluOrtho2D(0, screenWidth, 0, screenHeight);					// set the coordinate system for the window	
	glMatrixMode(GL_MODELVIEW);									// Select The Modelview Matrix
	glLoadIdentity();											// Reset The Modelview Matrix
}
void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);	 //sets the clear colour to black

	//FILLING THE MAP WITH COLLISION BOXES
	map.setColBoxes();

	tempShuriken.xMax = map.getWidth();
	tempShuriken.yMax = map.getHeight();

	monster_spawn_1 = Point2D(32.0f, 150.0f);
	monster_spawn_2 = Point2D(800.0f, 150.0f);
	monster_spawn_3 = Point2D(32.0f, 728.0f);
	monster_spawn_4 = Point2D(800.0f, 725.0f);

	spawn_points[0] = &monster_spawn_1;
	spawn_points[1] = &monster_spawn_2;
	spawn_points[2] = &monster_spawn_3;
	spawn_points[3] = &monster_spawn_4;

	nv::Image backGround;

	if (backGround.loadImageFromFile("Assets/bg2.png"))
	{
		glGenTextures(1, &bg_tex);
		glBindTexture(GL_TEXTURE_2D, bg_tex);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D, 0, backGround.getInternalFormat(), backGround.getWidth(), backGround.getHeight(), 0, backGround.getFormat(), backGround.getType(), backGround.getLevel(0));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

		map.setBgTexture(bg_tex);
	}
	else
		MessageBox(NULL, "Failed to load texture background", "End of the world", MB_OK | MB_ICONINFORMATION);

	nv::Image enemy;
	if (enemy.loadImageFromFile("Assets/frame-2.png"))
	{
		glGenTextures(1, &monster_tex);
		glBindTexture(GL_TEXTURE_2D, monster_tex);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D, 0, enemy.getInternalFormat(), enemy.getWidth(), enemy.getHeight(), 0, enemy.getFormat(), enemy.getType(), enemy.getLevel(0));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
	}
	else
		MessageBox(NULL, "Failed to load texturetr enemy", "End of the world", MB_OK | MB_ICONINFORMATION);


	nv::Image shuriken;
	if (shuriken.loadImageFromFile("Assets/shuriken.PNG"))
	{
		glGenTextures(1, &shuriken_tex);
		glBindTexture(GL_TEXTURE_2D, shuriken_tex);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D, 0, shuriken.getInternalFormat(), shuriken.getWidth(), shuriken.getHeight(), 0, shuriken.getFormat(), shuriken.getType(), shuriken.getLevel(0));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

		player.setShurikenTexture(shuriken_tex);
	}
	else
		MessageBox(NULL, "Failed to load texture shuriken", "End of the world", MB_OK | MB_ICONINFORMATION);

	//INITIAL PLAYER TEXTURE
	nv::Image aniTex;

	if (aniTex.loadImageFromFile("Assets/idle.png"))
	{
		glGenTextures(1, &ninja_tex);
		glBindTexture(GL_TEXTURE_2D, ninja_tex);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D, 0, aniTex.getInternalFormat(), aniTex.getWidth(), aniTex.getHeight(), 0, aniTex.getFormat(), aniTex.getType(), aniTex.getLevel(0));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

		player = Ninja(aniTex.getWidth(), aniTex.getHeight(), 416, 100);
		player.setTexture(ninja_tex);
	}
	else
		MessageBox(NULL, "Failed to load texture idle", "End of the world", MB_OK | MB_ICONINFORMATION);

	//START SCREEN BACKGROUND
	nv::Image startTex;
	if (startTex.loadImageFromFile("Assets/StartScreen.png"))
	{
		glGenTextures(1, &start_game_tex);
		glBindTexture(GL_TEXTURE_2D, start_game_tex);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D, 0, startTex.getInternalFormat(), startTex.getWidth(), startTex.getHeight(), 0, startTex.getFormat(), startTex.getType(), startTex.getLevel(0));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

	}
	else
		MessageBox(NULL, "Failed to load texture startscreen", "End of the world", MB_OK | MB_ICONINFORMATION);

	nv::Image endTex;
	if (endTex.loadImageFromFile("Assets/EndScreen.png"))
	{
		glGenTextures(1, &game_over_tex);
		glBindTexture(GL_TEXTURE_2D, game_over_tex);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D, 0, endTex.getInternalFormat(), endTex.getWidth(), endTex.getHeight(), 0, endTex.getFormat(), endTex.getType(), endTex.getLevel(0));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

	}
	else
		MessageBox(NULL, "Failed to load texture endscreen", "End of the world", MB_OK | MB_ICONINFORMATION);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (!myfont.Create("Arial.glf", 13))
		exit(0);
}
void processKeys(double delta)
{

	if (keys[' '])
	{

		pressed = true;
		if (jumpTime > 0){

			player.checkBoxes(map.getColBoxes(), delta, 'U');
			player.setJumping(true);

		}
		gravityOn = true;
	}

	if (keys['A'])
	{

		walking = true;
		pressed = true;
		if (player.getX() - 32 > 0){
			player.checkBoxes(map.getColBoxes(), delta, 'L');
		}
	}

	if (keys['S'])
	{

		if (player.getY() - 32 > 0){
			player.checkBoxes(map.getColBoxes(), delta, 'D');
		}
	}


	if (keys['D'])
	{
		walking = true;
		if (player.getX() + 32 < map.getWidth()){
			player.checkBoxes(map.getColBoxes(), delta, 'R');
		}
	}

	if (keys['R']){
		player.reload();
	}

	if (LeftPressed)
	{

		player.fire((float)mouse_x, (float)mouse_y);
	}

}

//WIN32 functions
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc
void KillGLWindow();									// releases and destroys the window
bool CreateGLWindow(char* title, int width, int height); //creates the window
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);  // Win32 main function

//win32 global variabless
HDC			hDC = NULL;		// Private GDI Device Context
HGLRC		hRC = NULL;		// Permanent Rendering Context
HWND		hWnd = NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application


/******************* WIN32 FUNCTIONS ***************************/
int WINAPI WinMain(HINSTANCE	hInstance,			// Instance
	HINSTANCE	hPrevInstance,		// Previous Instance
	LPSTR		lpCmdLine,			// Command Line Parameters
	int			nCmdShow)			// Window Show State
{
	LARGE_INTEGER freq, last, current;
	double delta = 0.0f;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&last);

	MSG		msg;
	bool	done = false;


	console.Open();

	// Create Our OpenGL Window
	if (!CreateGLWindow("PixelNinja - City Over the Clouds", screenWidth, screenHeight))
	{
		return 0;
	}



	while (!done)
	{
		QueryPerformanceCounter(&current);

		delta = (double)(current.QuadPart - last.QuadPart) / 50;

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				done = true;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (keys[VK_RETURN]){
				if (startScreenActive){
					startScreenActive = false;
				}
			}
			if (keys[VK_ESCAPE])
				done = true;

			/*if(player.getKillCount() == MAX_MONSTER_NUMBER){
				gameover = true;
				won = true;
				}
				*/
			if (player.getHealth() <= 0){
				gameover = true;
				won = false;
			}

			update();
			processKeys(delta);
			display(delta);					// Draw The Scene
			SwapBuffers(hDC);				// Swap Buffers (Double Buffering)

			last = current;
		}
	}


	console.Close();

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (int)(msg.wParam);						// Exit The Program
}

//WIN32 Processes function - useful for responding to user inputs or other events.
LRESULT CALLBACK WndProc(HWND	hWnd,			// Handle For This Window
	UINT	uMsg,			// Message For This Window
	WPARAM	wParam,			// Additional Message Information
	LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
	case WM_CLOSE:								// Did We Receive A Close Message?
	{
		PostQuitMessage(0);						// Send A Quit Message
		return 0;								// Jump Back
	}
	break;

	case WM_SIZE:								// Resize The OpenGL Window
	{
		reshape(LOWORD(lParam), HIWORD(lParam));  // LoWord=Width, HiWord=Height
		return 0;								// Jump Back
	}
	break;

	case WM_LBUTTONDOWN:
	{
		mouse_x = LOWORD(lParam);
		mouse_y = screenHeight - HIWORD(lParam);
		LeftPressed = true;
	}
	break;

	case WM_LBUTTONUP:
	{
		LeftPressed = false;
	}
	break;

	case WM_RBUTTONDOWN:
	{
		// mouse_x = LOWORD(lParam);          
		//mouse_y = screenHeight - HIWORD(lParam);
		RightPressed = true;
	}
	break;

	case WM_RBUTTONUP:
	{
		RightPressed = false;
	}
	break;

	case WM_MOUSEMOVE:
	{
		mouse_x = LOWORD(lParam);
		mouse_y = screenHeight - HIWORD(lParam);//
	}
	break;
	case WM_KEYDOWN:							// Is A Key Being Held Down?
	{
		keys[wParam] = true;					// If So, Mark It As TRUE
		return 0;								// Jump Back
	}
	break;
	case WM_KEYUP:								// Has A Key Been Released?
	{
		keys[wParam] = false;					// If So, Mark It As FALSE
		return 0;								// Jump Back
	}
	break;
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void KillGLWindow()								// Properly Kill The Window
{
	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL, NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}
		hRC = NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd, hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hDC = NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL", hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;									// Set hInstance To NULL
	}
}
bool CreateGLWindow(char* title, int width, int height)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left = (long)0;			// Set Left Value To 0
	WindowRect.right = (long)width;		// Set Right Value To Requested Width
	WindowRect.top = (long)0;				// Set Top Value To 0
	WindowRect.bottom = (long)height;		// Set Bottom Value To Requested Height

	hInstance = GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc = (WNDPROC)WndProc;					// WndProc Handles Messages
	wc.cbClsExtra = 0;									// No Extra Window Data
	wc.cbWndExtra = 0;									// No Extra Window Data
	wc.hInstance = hInstance;							// Set The Instance
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground = NULL;									// No Background Required For GL
	wc.lpszMenuName = NULL;									// We Don't Want A Menu
	wc.lpszClassName = "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;											// Return FALSE
	}

	dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style

	dwStyle = (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);// Windows Style //no resizing no maximizing		

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd = CreateWindowEx(dwExStyle,							// Extended Style For The Window
		"OpenGL",							// Class Name
		title,								// Window Title
		dwStyle |							// Defined Window Style
		WS_CLIPSIBLINGS |					// Required Window Style
		WS_CLIPCHILDREN,					// Required Window Style
		0, 0,								// Window Position
		WindowRect.right - WindowRect.left,	// Calculate Window Width
		WindowRect.bottom - WindowRect.top,	// Calculate Window Height
		NULL,								// No Parent Window
		NULL,								// No Menu
		hInstance,							// Instance
		NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		24,											// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		24,											// 24Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(hDC = GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!SetPixelFormat(hDC, PixelFormat, &pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!(hRC = wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!wglMakeCurrent(hDC, hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	ShowWindow(hWnd, SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	reshape(width, height);					// Set Up Our Perspective GL Screen

	init();

	return true;									// Success
}
//CONVERTION FUNCTIONS FOR THE INTERFACE
string convertInt(int number)
{
	stringstream ss;
	ss << number;
	return ss.str();
}
string convertFloat(float number)
{
	stringstream ss;
	ss << number;
	return ss.str();
}