#include <iostream>
#include <sstream>
#include <string.h>
#include <glut.h>
#include<math.h>
#include <windows.h> // use as needed for your system
#include <gl/Gl.h>

using namespace std;
int X;
int Y;
int deadAttackers=0;
float cameraX;
float cameraY;
float cameraZ;
int build=2;
int topRow=3;
int bottomRow=-2;
bool pauseGame=false;
int cameraView=0;
bool pressedV=false;
int attackerAppear=1;
bool youWon=false;
float bulletTimer;
float attackerTimer;
int newModuleTimer=1;
int rGathererNumber=0;
int defenderNumber=0;
bool firstLoop;
float rotAng;
bool selectRow=false;
bool selectCol=false;
const float DEG2RAD = 3.14159/180;
int counter;
int gameStatus=2;
const float attackerSpeed=0.03;
const float bulletSpeed=0.1;
#define leftBoundary -3
#define rightBoundary 4

void drawText(float x, float y, const char *str);
void Key(unsigned char key, int x, int y);
void Switch_Key (int key, int x, int y);
void initialize(void);
void drawCircle(float);
void drawLand(void);
void axes(float);
void update(void);
void drawTile(int x,int y,int z);
void renderBitmapString(float x, float y, float z, void *font, char *string);

class Tile
{
public:
	bool free;
	float x;
	float y;

	Tile::Tile()
	{
		x=0;
		y=0;
		free=true;
	}
	Tile::Tile(float X,float Y)
	{
		x=X;
		y=Y;
		free=false;
	}

	void setFreedom(bool f)
	{
		free=f;
	}

}; 
Tile tiles[5][9];

class attacker {

public:
	float timer;
	int die;
	int lives;
	float x;
	float y;
	float z;
	float color;
	bool visible;
	bool forbidden;

	attacker::attacker(){

		x=rightBoundary+10;
		y=-1.5+10;
		z=0;
		timer=0;
		die=true;
		lives = 0;
		visible=false;
		forbidden=false;
	}

	attacker::attacker(float X, float Y, float Z){
		visible=true;
		x=X;
		y=Y;
		z=Z;
		timer=0;
		die=0;
		lives = 2;
		forbidden=false;
		die=false;
	}

	float getX()
	{
		return this->x;
	}

	void setForbidden()
	{
		forbidden=true;
	}

	void update()
	{
		if(x<leftBoundary+1)
		{
			gameStatus--;
			killAttacker();
		}
		if(lives >0)
		{
			x-=attackerSpeed;
		}

	}

	void resetPosition()
	{
		x=rightBoundary;
		lives=2;
		//attackerTimer=0;
		die=false;
		visible=true;
	}

	void decLives()
	{
		lives=1;
	}

	void killAttacker()
	{
		this->lives=0;
		this->die=true;
		this->visible=false;
	}

	void drawWheel(float r)
	{
		glPushMatrix();
		glTranslated(0,0,-0.25);
		glColor3f(0,0,0);
		GLUquadric*shape=gluNewQuadric();
		gluCylinder(shape,r,r,0.5,10,10);
		glPopMatrix();
	}

	void draw() {
		if(!forbidden){
			GLUquadric *shape = gluNewQuadric();
			glPushMatrix();
			glTranslatef(x- 0.5f,y, z);
			glRotated(90,1,0,0);
			glScalef(0.25f, 0.15f, 0.25f);
			drawCube(x,y,z,3.0f);

			// first wheel
			glPushMatrix();
			glTranslatef(x + 0.8f, y, z + 1.5f);
			drawWheel(0.4f);
			glPushMatrix();
			glTranslatef(0.0f, 0.0f, 0.25f);
			gluDisk(shape, 0.0001f, 0.4f, 10, 1);
			glPopMatrix();
			glPopMatrix();

			// second wheel
			glPushMatrix();
			glTranslatef(x - 0.8f, y, z + 1.5f);
			drawWheel(0.4f);
			glPushMatrix();
			glTranslatef(0.0f, 0.0f, 0.25f);
			gluDisk(shape, 0.0001f, 0.4f, 10, 1);
			glPopMatrix();
			glPopMatrix();


			// third wheel
			glPushMatrix();
			glTranslatef(x + 0.8f, y, z - 1.5f);
			drawWheel(0.4f);
			glPushMatrix();
			glTranslatef(0.0f, 0.0f, -0.25f);
			gluDisk(shape, 0.0001f, 0.4f, 10, 1);
			glPopMatrix();
			glPopMatrix();

			// fourth wheel
			glPushMatrix();
			glTranslatef(x - 0.8f, y, z - 1.5f);
			drawWheel(0.4f);
			glPushMatrix();
			glTranslatef(0.0f, 0.0f, -0.25f);
			gluDisk(shape, 0.0001f, 0.4f, 10, 1);
			glPopMatrix();
			glPopMatrix();

			// front spikes
			glPushMatrix();
			glTranslatef(x - 1.5f, y + 2.0f, z + 1.0f);
			glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
			glutSolidCone(0.3, 0.6, 10, 10);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(x - 1.5f , y + 2.0f, z);
			glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
			glutSolidCone(0.3, 0.6, 10, 10);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(x - 1.5f, y + 2.0f, z - 1.0f);
			glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
			glutSolidCone(0.3, 0.6, 10, 10);
			glPopMatrix();
			// head or whatever it may be
			glPushMatrix();
			glTranslatef(x, y+3.5f, z);
			glScalef(0.7f, 0.7f, 0.7f);
			glutSolidDodecahedron();
			glPopMatrix();
			glPopMatrix();
		}
	}
	void drawCube(float x, float y, float z , float width) 
	{
		float offset = width / 2;
		glColor3d(lives/2.0f, 0.0f, 0.0f);     // RED
		glBegin(GL_QUADS);  

		// Top facey
		glVertex3f(x + offset , y + width, z - offset);
		glVertex3f(x - offset, y + width, z - offset);
		glVertex3f(x - offset, y + width, z + offset);
		glVertex3f(x + offset, y + width, z + offset);

		// Bottom face 
		glVertex3f(x + offset, y , z + offset);
		glVertex3f(x - offset, y , z + offset);
		glVertex3f(x - offset, y , z - offset);
		glVertex3f(x + offset, y , z - offset);

		// Front face 
		glVertex3f(x + offset, y + width, z + offset);
		glVertex3f(x - offset, y + width, z + offset);
		glVertex3f(x - offset, y , z + offset);
		glVertex3f(x + offset, y , z + offset);

		// Back face 
		glVertex3f(x + offset, y , z - offset);
		glVertex3f(x - offset, y , z - offset);
		glVertex3f(x - offset, y + width, z - offset);
		glVertex3f(x + offset, y + width, z - offset);

		// Left face 
		glVertex3f(x - offset, y + width, z + offset);
		glVertex3f(x - offset, y + width, z - offset);
		glVertex3f(x - offset, y , z - offset);
		glVertex3f(x - offset, y , z + offset);

		// Right face 
		glVertex3f(x + offset, y + width, z - offset);
		glVertex3f(x + offset, y + width, z + offset);
		glVertex3f(x + offset, y , z + offset);
		glVertex3f(x + offset, y , z - offset);
		glEnd(); 
	}

};

attacker zombies[5];

class defender {
public:

	float timer;
	bool die;
	float x;
	float y;
	float z;
	bool visible;
	bool forbidden;

	defender::defender(){
		timer=0;
		x=leftBoundary;
		y=0.8;
		z=1;
		visible=false;
		forbidden=false;
		die =true;
	}

	defender::defender(float X, float Y, float Z)
	{
		timer=0;
		x=X;
		y=Y+0.3;
		z=Z;
		visible=true;
		forbidden=false;
		die=false;
	}

	void setForbidden()
	{
		forbidden=true;
	}

	void defenderDies()
	{
		this->die=true;
		this->visible=false;
	}

	void resetPosition()
	{
		x=leftBoundary-1;
	}

	void draw()
	{
		if(!forbidden)
		{
			if(visible){
				glPushMatrix();
				glTranslated(x,y-0.3,z);
				glScaled(1,0.7,1);
				glRotated(90.0, 1,0,0);
				glRotated(90.0, 0,1,0);

				//The above sphere
				glPushMatrix();
				glRotated(90.0, 1,0,0);
				glColor3f(0,0,0);
				glutSolidSphere(0.25, 10, 10);
				glPopMatrix();

				glPushMatrix();
				glColor3f(1,0,0);
				glRotated(90.0, 1,0,0);
				GLUquadricObj * qobj;
				qobj = gluNewQuadric();
				gluQuadricDrawStyle(qobj,GLU_FILL);
				gluCylinder(qobj, 0.1, 0.1, 1, 8,8);
				glPopMatrix();

				//Draw face cylinder
				glPushMatrix();
				glColor3f(1,0,0);
				//glRotated(90.0, 1,0,0);
				qobj = gluNewQuadric();
				gluQuadricDrawStyle(qobj,GLU_FILL);
				gluCylinder(qobj, 0.2, 0.2, 0.5, 8,8);
				glPopMatrix();

				glPopMatrix();
			}
		}
	}

	void update()
	{
	}
};

defender defenders[5];

bool attackerKillDefender(attacker a,defender d);

class bullet
{
public:
	float Y;
	float X;
	float Z;
	float originalX;
	int timer;
	bool visible;
	bool die;
	bool forbidden;

	bullet::bullet()
	{
		X=leftBoundary+0.4;
		originalX=X;
		Y=-3.5;
		Z=0;
		die=true;
		timer=0;
		visible=false;
		forbidden=false;
	}
	bullet::bullet(float x, float y, float z)
	{
		X=x;
		Y=y+0.3;
		Z=z;
		originalX=X;
		die=false;
		timer=0;
		visible = true;
		forbidden=false;
	}
	void hide()
	{
		visible=false;
		die=true;
	}
	void show()
	{
		visible=true;
	}
	void setForbidden()
	{
		forbidden=true;
	}
	void resetPosition()
	{
		X=originalX;
		this->show();
	}
	void update()
	{
		if(X>rightBoundary+5)
			resetPosition();

		if(visible)
		{
			X+=bulletSpeed;
		}

	}
	void draw()
	{		
		if(!forbidden){
			if(!visible){
				return;
			}

			glPushMatrix();
			glColor3f(0.5,0.5,0.5);
			glTranslated(X,Y,Z); // sphere at (1,1,0)
			glutSolidSphere(0.1, 10, 10);
			glPopMatrix();
		}
	}
	void setPosition(float x, float y, float z)
	{
		X=x;
		Y=y;
		Z=z;
	}
};
bullet bullets[5];

class rGatherer {
public:
	float timer;
	bool die;
	float angleRotation;
	float x;
	float y;
	float z;
	bool forbidden;


	rGatherer::rGatherer()
	{
		angleRotation=0;
		die=true;
		timer=0;
		x=-2.5;
		y=-3.5;
		z=0;
		forbidden=false;
	}

	rGatherer::rGatherer(float X,float Y,float Z)
	{
		angleRotation=0;
		die=false;
		timer=0;
		x=X;
		y=Y;
		z=Z;
		forbidden=false;
	}

	void resetPosition()
	{
		x=leftBoundary-1;
	}

	void setForbidden()
	{
		forbidden=true;
	}

	void killRGatherer()
	{
		this->die=true;
	}

	void update()
	{
		if(!pauseGame)
			if(!die)
			{
				angleRotation+=50;
			}
	}

	void draw(float radius)
	{
		if(!forbidden){
			if(!die){
				glPushMatrix();
				glTranslated(x, y,z);
				glRotatef(angleRotation, 0, 0, 1);
				glRotatef(90, 1,0, 0);

				glPushMatrix();
				glTranslated(0, 1.5 ,0);
				glScaled(0.15, 0.15, 0.15);
				glColor3f(1.0f, 0.0f, 0.0f);
				glutSolidDodecahedron();
				glPopMatrix();

				for(float i =0; i <= 33; i+=0.03){
					glPushMatrix();
					double x = cos(i);
					double z = sin(i);
					glColor3f(0.0f, 0.0f, 0.0f);
					glTranslated(x/5,i*0.04,z/10);
					glutSolidSphere(0.08, 5, 5);
					glPopMatrix();

				}
				glPopMatrix();
			}
		}
	}

};
rGatherer rGatherers[5];
bool attackerKillrGatherer(attacker a);


bool attackerKillDefender(attacker a)
{
	for(int i=0;i<5;i++)
		if(a.x<defenders[i].x+0.5 && fabs(a.y-defenders[i].y) < 1) 
		{
			defenders[i].defenderDies();
			bullets[i].hide();
			return true;
		}
		return false;
}

bool attackerKillrGatherer(attacker a)
{
	for(int i=0;i<5;i++)
		if(a.x<rGatherers[i].x+0.5 && fabs(rGatherers[i].y-a.y) < 1 && !rGatherers[i].die) 
		{
			cout<<"Attacker kill R"<<endl;
			rGatherers[i].killRGatherer();
			return true;
		}
		return false;
}

bool bulletKillAttacker(attacker a,int j)
{
	for(int i=0;i<5;i++)
		if(a.x<bullets[i].X+0.4 && fabs(bullets[i].Y-a.y)<1) 
		{
			//cout<<"bulletKillAttacker"<<endl;
			if(zombies[j].lives==2)
				zombies[j].decLives();
			else
			{
				zombies[j].killAttacker();
				cout<<deadAttackers<<endl;
				deadAttackers++;
				zombies[j].resetPosition();
			}

			bullets[i].resetPosition();
			return true;
		}
		return false;

}

void drawAxes()
{
	glPushMatrix();
	glTranslated(-5,0,0);

	glPushMatrix();
	glBegin(GL_LINES);
	glColor3f(0,1,0);
	glVertex3d(0, 0, 0);
	glVertex3d(0,0,1); // Z-axis
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBegin(GL_LINES);
	glColor3f(0,0,1);
	glVertex3d(0, 0, 0);
	glVertex3d(0,1,0); //Y-axis
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBegin(GL_LINES);
	glColor3f(1,1,0);
	glVertex3d(0, 0, 0);
	glVertex3d(1,0,0); //X-axis
	glEnd();
	glPopMatrix();

	glPopMatrix();

}

void decGameStatus()
{
	gameStatus=1;
}


void Display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(pauseGame && !youWon)
		drawText(-1,5,"Pause");
	if(youWon && gameStatus>0)
		drawText(-1,5,"You Won");
	if(deadAttackers==50)
		youWon=true;


	if(gameStatus<0)
	{
		drawText(-1,5,"You Lost");
		youWon=true;
	}

	glPushMatrix();
	glRotated(cameraX,1,0,0);
	glRotated(cameraView,0,0,1);
	glRotated(cameraY,0,1,0);

	if(selectCol && selectRow)
		drawTile(Y-5,X-3,0);
	drawLand();

	for (int i=0;i<5;i++)
	{
		attackerKillrGatherer(zombies[i]);

		if(!rGatherers[i].die)
		{
			rGatherers[i].update();
			rGatherers[i].draw(0.5);
		}

		if(bulletKillAttacker(zombies[i],i) && zombies[i].lives==1)
		{
		}

		if(bulletKillAttacker(zombies[i],i) && zombies[i].lives==2)
		{
			cout<<"Lives"<<endl;
			zombies[i].decLives();
		}

		attackerKillDefender(zombies[i]);

		if(zombies[i].getX()<leftBoundary+0.5)
		{
			//gameStatus--;
			defenders[i].defenderDies();
			rGatherers[i].killRGatherer();
			bullets[i].setForbidden();
		}

		if(zombies[i].lives>0)
		{
			if(!pauseGame && !youWon)
				zombies[i].update();
			if(!zombies[i].die)
				zombies[i].draw();
			//drawText(-1,5,"You Won");
		}
		else
		{
			if(int(attackerTimer)%250==0)
				zombies[i].resetPosition();
		}
		if(bullets[i].visible)
		{
			if(!bulletKillAttacker(zombies[i],i))
			{
				if(!pauseGame && !youWon)
					bullets[i].update();
				bullets[i].draw();
			}
			else
			{
				if(int(bulletTimer)%200==0)
				{
					bullets[i].resetPosition();
				}
			}
		}

		if(defenders[i].visible)
		{
			if(!pauseGame && !youWon)
				defenders[i].update();
			defenders[i].draw();
		}
	}
	glPopMatrix();
	glFlush();

}

void timerFunction(int t)
{
	if(pressedV)
	{
		if(!pauseGame)
			cameraView++;
		if(cameraView==360)
			pressedV=false;
	}
	attackerAppear++;
	attackerTimer=1;
	bulletTimer=1;
	rotAng += 0.01;
	newModuleTimer+=50;
	glutPostRedisplay();
	glutTimerFunc(50,timerFunction,0);
}

void drawLand(void)
{
	glBegin(GL_LINES);
	glColor3d(0,0,0); 
	for(int y=bottomRow;y<=3;y++){
		for(int x=-4;x<=4;x++){
			glPushMatrix();
			glColor3d(0,0,0); 
			glVertex3d(x,y,0);
			glVertex3d(x+1,y,0);
			glPopMatrix();
		}
	}
	for(int x=-4;x<=5;x++){
		for(int y=bottomRow;y+1<=3;y++){
			glPushMatrix();
			glColor3d(0,0,0);
			glVertex3d(x,y,0);
			glVertex3d(x,y+1,0);
			glPopMatrix();
		}
	}
	glEnd();

}

void drawTile(int x,int y,int z)
{
	glBegin(GL_QUADS);  
	glColor3f(0,0,1);
	glVertex3f(x+0.9, y+0.9, 0);
	glVertex3f(x+0.1, y+0.9, 0);
	glVertex3f(x+0.1, y+0.1,0);
	glVertex3f( x+0.9, y+0.1,0);
	glEnd();
}

void insertDefender(float x,float y,float z)
{
	for(int i=0;i<5;i++)
	{ 
		if(defenders[i].die && build>0)
		{
			defenders[i]=defender(x,y,1);
			defenderNumber++;
			build--;
			bullets[i]=bullet(x,y-0.3,1);
			break;
		}
	}
}

void insertRGatherer(float x,float y,float z)
{
	cout<<"insert R"<<endl;
	for(int i=0;i<5;i++)
	{ 
		if(rGatherers[i].die)
		{
			rGathererNumber++;
			build++;
			cout<<"build"<<endl;
			rGatherers[i]=rGatherer(x,y,0);
			cout<<"build22"<<endl;			
			break;
		}
	}
}

void renderBitmapString(float x, float y, float z, void *font, char *string) {
	char *c;
	glRasterPos3f(x, y, z);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}

void initialize()

{

	for(int i =0;i<5;i++)
	{
		if(zombies[i].die)
			zombies[i]= attacker(rightBoundary,i-1.5,0);
		zombies[i].visible = true;
	}
}


void Key(unsigned char key,int x,int y)
{
	if(key=='p')
		pauseGame=!pauseGame;
	else if(key == 'v' )
		pressedV=true;
	else if(!selectRow && key<54 && key>48)
	{

		switch(key)
		{
		case 49: X=1;break;
		case 50: X=2;break;
		case 51: X=3;break;
		case 52: X=4;break;
		default: X=5;break;
		}
		selectRow=true;
	}
	else if(selectRow && !selectCol && key<58 && key>48)
	{
		switch(key)
		{

		case 49: Y=1;break;
		case 50: Y=2;break;
		case 51: Y=3;break;
		case 52: Y=4;break;
		case 53: Y=5;break;
		case 54: Y=6;break;
		case 55: Y=7;break;
		case 56: Y=8;break;
		default: Y=9;break;

		}
		selectCol=true;
	}

	else if (selectCol && selectRow)
	{	
		switch(key)
		{
		case 'd':	insertDefender(Y-4.5,X-2.5,0);break;

		case 'r':	insertRGatherer(Y-4.5,X-2.5,0);break;

		case 'c': 
			for(int i =0;i<5;i++)
			{
				if(fabs((defenders[i].x+4.5)-X) < 1 && fabs((defenders[i].y+2.5)-Y) < 1 && defenders[i].visible)
				{
					defenders[i].defenderDies();
					defenders[i].resetPosition();
					bullets[i].hide();
					bullets[i].setPosition(-10,-10,1);
					defenderNumber--;
					break;
				}
				else if(fabs((rGatherers[i].x+4.5)-X) < 1 && fabs((rGatherers[i].y +2.5)-Y) < 1 && !rGatherers[i].die)
				{
					rGatherers[i].killRGatherer();
					rGathererNumber--;					
					break;
				}

			};break;
		}
		selectCol=false;
		selectRow=false;
	}

}

// KeyboardUp handler function
//   similar to the Keyboard handler function except that it is called only when the key is released
void Switch_Key(int key, int x, int y) {
	if(!pressedV){
		if (key == GLUT_KEY_UP)
			cameraX++;
		else if(key == GLUT_KEY_DOWN)
			cameraX--;
		else if(key == GLUT_KEY_RIGHT)
			cameraY++;
		else if(key == GLUT_KEY_LEFT)
			cameraY--;
	}
}

void drawText(float x, float y, const char *str) {
	int l = (int)strlen(str);
	glRasterPos2f(x, y);
	for(int i = 0; i < l; i++) {
		glColor3ub(0, 255, 255);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
	}
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(300, 300);
	glutInitWindowPosition(150, 150);
	glutCreateWindow(" Zombies VS. Plants ");
	glutKeyboardFunc(Key);      // sets the Keyboard handler function; called when a key is pressed
	glutSpecialFunc(Switch_Key);  // sets the KeyboardUp handler function; called when a key is released
	initialize();
	if(!pauseGame){
		glutDisplayFunc(Display);
		glutTimerFunc(0,timerFunction,0);
	}

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f /*view angle */, 300 / 300, 0.1f, 300.0f);
	//	aspect ratio	   w /  h    Z near, Z far  
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0f, 0.0f, 15.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	// gluLookAt(x,y,z for eye camera . x,y,z for center . x,y,z orentation camera)
	glutMainLoop();
}