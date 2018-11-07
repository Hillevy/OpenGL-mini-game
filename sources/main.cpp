																									 /*
/##################################################################################################\
# Description : #                                                                                  #
#################                                                                                  #
#                                                                                                  #
#  Ce fichier est le point d'entrée du moteur ( le "main()" ).                                     #
#                                                                                                  #
#                                                                                                  #
\##################################################################################################/
																									 */
#include "window.h"
#include "input.h"
#include "timer.h"
#include "ogl.h"
#include "load.h"
#include "camera.h"
#include "quaternion.h"
#include "point.h"
#include "maths_3d.h"

#include "player.h"
#include "terrain.h"
#include "textureManager.h"
#include "microtimer.h"
#include "enemy.h"



/****************************************************************************\
*                                                                            *
*                            Variables EXTERNES                              *
*                                                                            *
\****************************************************************************/


/****************************************************************************\
*                                                                            *
*                            Variables GLOBALES                              *
*                                                                            *
\****************************************************************************/
WINDOW		*win = NULL;
MY_INPUT    *inp = NULL;
CAMERA		*cam = NULL;

TextureManager *txm = NULL;
Player *player = new Player(0, "0");
Enemy *enemy = new Enemy();
Terrain *myTerrain = NULL;

Texture *texture_ground = NULL;
Texture *texture_key = NULL;
Texture *texture_portal = NULL;
Texture *texture_hedge = NULL;
Texture *texture_enemy = NULL;
Texture *texture_board = NULL;
Texture *textures_skybox[6];

float posX = 30;
float posY = 0;
float posZ = 180;
float angleX = 0;
float angleY = 0;
float angleZ = 0;

float anglePortal = 0;

GLuint mazeDL;
GLuint groundDL;


/****************************************************************************\
*                                                                            *
*                             Variables LOCALES                              *
*                                                                            *
\****************************************************************************/

void grass() {
	glPushMatrix();
	glTranslatef(120, -15, 15);
	glScalef(110, 0, 180);
	glRotatef(90, 1, 0, 0); 
	txm->bindTexture(texture_ground);
	txm->beginTexturing();

	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f); glVertex3f(-1, 0, 0);
	glTexCoord2d(0.0f, 15.0f); glVertex3f(-1, 2, 0);
	glTexCoord2d(15.0f, 15.0f); glVertex3f(1, 2, 0);
	glTexCoord2d(15.0f, 0.0f); glVertex3f(1, 0, 0);
	glEnd();
	txm->endTexturing();
	glPopMatrix();
}

void board(int l, int c) {
	glPushMatrix();
	glTranslatef(-myTerrain->GetBlockSize()/2 + c * myTerrain->GetBlockSize(), -8, l*myTerrain->GetBlockSize());
	glScalef(0.5f, 0.5f, 0.5f);
	txm->bindTexture(texture_board);
	txm->beginTexturing();
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f); glVertex3f(-15, -15, 0);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(-15, 15, 0);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(15, 15, 0);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(15, -15, 0);
	glEnd();
	txm->endTexturing();
	glPopMatrix();
}

void key(int l, int c) {
	glPushMatrix();
	glTranslatef(c*myTerrain->GetBlockSize(), 5, l*myTerrain->GetBlockSize());
	txm->bindTexture(texture_key);
	txm->beginTexturing();
	player->InitializeKeysPosition(c*myTerrain->GetBlockSize(), 5, l*myTerrain->GetBlockSize());
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f); glVertex3f(-15, -15, 0);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(-15, 15, 0);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(15, 15, 0);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(15, -15, 0);
	glEnd();
	glRotatef(90, 0, 1, 0);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f); glVertex3f(-15, -15, 0);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(-15, 15, 0);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(15, 15, 0);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(15, -15, 0);
	glEnd();
	txm->endTexturing();	
	glPopMatrix();
}

void greenPortal(int l, int c) {
	txm->bindTexture(texture_portal);
	txm->beginTexturing();

	glTranslatef(c*myTerrain->GetBlockSize(), 5, l*myTerrain->GetBlockSize());

	glBegin(GL_QUADS);
	glColor3f(0, 255, 0);
	glTexCoord2d(0.0f, 0.0f); glVertex3f(-15, -15, 0);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(-15, 15, 0);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(15, 15, 0);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(15, -15, 0);
	glColor3f(255, 255, 255);
	glEnd();

	txm->endTexturing();
}

void portal(int l, int c) {
	txm->bindTexture(texture_portal);
	txm->beginTexturing();

	glTranslatef(c*myTerrain->GetBlockSize(), 5, l*myTerrain->GetBlockSize());

	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f); glVertex3f(-15, -15, 0);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(-15, 15, 0);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(15, 15, 0);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(15, -15, 0);
	glEnd();

	txm->endTexturing();
}

void CreateEnemy(int l, int c) {
	glPushMatrix();
	//glTranslatef(c*myTerrain->GetBlockSize(), 0, l*myTerrain->GetBlockSize());
	glTranslatef(c, 0, l);
	glScalef(0.5f, 0.5f, 0.5f);
	txm->bindTexture(texture_enemy);
	txm->beginTexturing();
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f); glVertex3f(-15, -15, 0);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(-15, 15, 0);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(15, 15, 0);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(15, -15, 0);
	glEnd();
	glRotatef(90,0,1,0);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f); glVertex3f(-15, -15, 0);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(-15, 15, 0);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(15, 15, 0);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(15, -15, 0);
	glEnd();
	txm->endTexturing();
	glPopMatrix();
}

void ClampPosition(int i, float x, float z) {
	switch (i)
	{
	case 1:
		player->Clamp(x, z, cam->position, posX, posZ);
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	default:
		break;
	}
}

void CreateTexturedQuad()
{
	txm->bindTexture(texture_hedge);
	txm->beginTexturing();
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2f(1, 1); glVertex3f(0.5f, 0.5f, 0);
	glTexCoord2f(1, -1); glVertex3f(0.5f, -0.5f, 0);
	glTexCoord2f(-1, -1); glVertex3f(-0.5f, -0.5f, 0);
	glTexCoord2f(-1, 1); glVertex3f(-0.5f, 0.5f, 0);
	glEnd();
	glPopMatrix();
	txm->endTexturing();
}

void CreateBlock2(int l, int c, float offsetX, float offsetZ, float rotation) {
	glPushMatrix();
	//Global positionning of the block
	glTranslatef(c*myTerrain->GetBlockSize(), 0, l*myTerrain->GetBlockSize());
	//glTranslatef(-27, 0, -150);

	glPushMatrix();
	glTranslatef(offsetX, 0, offsetZ);
	glRotatef(rotation, 0, 1, 0);
	glScalef(myTerrain->GetBlockSize(), myTerrain->GetBlockSize(), myTerrain->GetBlockSize());
	CreateTexturedQuad();
	glPopMatrix();

	glPopMatrix();
}

void GenerateTerrain() { //23*15
	ifstream terrainFile;
	const char* path = "Level3.txt";
	terrainFile.open(path);
	if (terrainFile.is_open()) {
		char c;
		int countCharacters = 0;
		while (!terrainFile.eof()) {
			terrainFile.get(c);
			myTerrain->GetBlockPosition(countCharacters, 9, 14);
			switch (c)
			{
			case '0': //Ground
				break;
			case '1': //Horizontal vers le bas
				CreateBlock2(myTerrain->GetCurrentLine(), myTerrain->GetCurrentColumn(), 0, myTerrain->GetBlockSize() / 2, 0);
				//glPushMatrix();
				//glTranslatef(myTerrain->GetCurrentColumn()*myTerrain->GetBlockSize(), 0, myTerrain->GetCurrentLine()*myTerrain->GetBlockSize() + myTerrain->GetBlockSize() / 2);
				////glRotatef(rotation, 0, 1, 0);
				//glScalef(myTerrain->GetBlockSize()/2, myTerrain->GetBlockSize(), myTerrain->GetBlockSize()/2);
				//CreateTexturedQuad();
				//glPopMatrix();
				//player->Clamp(myTerrain->GetCurrentColumn()*myTerrain->GetBlockSize(), myTerrain->GetCurrentLine()*myTerrain->GetBlockSize() + myTerrain->GetBlockSize() / 2, cam->position, posX, posZ);
				break;
			case '2': //Vertical sur la droite
				CreateBlock2(myTerrain->GetCurrentLine(), myTerrain->GetCurrentColumn(), myTerrain->GetBlockSize() / 2, 0, 90);
				break;
			case '3': //Horizontal vers le haut
				CreateBlock2(myTerrain->GetCurrentLine(), myTerrain->GetCurrentColumn(), 0, -myTerrain->GetBlockSize() / 2, 0);
				break;
			case '4': //Vertical sur la gauche
				CreateBlock2(myTerrain->GetCurrentLine(), myTerrain->GetCurrentColumn(), -myTerrain->GetBlockSize() / 2, 0, 90);
				break;
			case '#': //Caractère de fin de fichier
				terrainFile.close();
				break;
			default:
				break;
			}
			countCharacters += 1;
		}

	}
	else {
		perror(path);
	}
	terrainFile.close();
}

void SpawnElements() {
	ifstream terrainFile;
	const char* path = "Spawn3.txt";
	terrainFile.open(path);
	if (terrainFile.is_open()) {
		char c;
		int countCharacters = 0;
		while (!terrainFile.eof()) {
			terrainFile.get(c);
			myTerrain->GetBlockPosition(countCharacters, 9, 14);
			int currentPos[3] = { myTerrain->GetCurrentColumn() * myTerrain->GetBlockSize(), 5, myTerrain->GetCurrentLine() * myTerrain->GetBlockSize() };
			int direction[] = { 1,0, 0};
			switch (c)
			{
			case '0': //None
				break;
			case '1': //Portal
				if (!(player->GetPortalIsActive())) {
					portal(myTerrain->GetCurrentLine(), myTerrain->GetCurrentColumn());
				}
				else {
					greenPortal(myTerrain->GetCurrentLine(), myTerrain->GetCurrentColumn());
				}
				player->InitializePortalPosition(myTerrain->GetCurrentColumn()*myTerrain->GetBlockSize(), 5, myTerrain->GetCurrentLine()*myTerrain->GetBlockSize());
				break;
			case '2': //Key
				key(myTerrain->GetCurrentLine(), myTerrain->GetCurrentColumn());
				break;
			case '3': //Enemy
				enemy->InitializeEnemyPosition(myTerrain->GetCurrentColumn()*myTerrain->GetBlockSize(), myTerrain->GetCurrentLine()*myTerrain->GetBlockSize());
				CreateEnemy(enemy->GetEnemyPos()[0], enemy->GetEnemyPos()[2]);
				break;
			case '4': //Wooden board
				board(myTerrain->GetCurrentLine(), myTerrain->GetCurrentColumn());
				break;
			case '#': //Caractère de fin de fichier
				terrainFile.close();
				break;
			default:
				break;
			}
			countCharacters += 1;
		}

	}
	else {
		perror(path);
	}
	terrainFile.close();
}

void drawSkybox() {

	// We're exploding the scaling to make a "unit cube" as the skybox.
	glPushMatrix();
	glTranslatef(0, 200, 0);
	glScalef(750, 750, 750);

	for (int i = 0; i < 5; i++) {
		txm->bindTexture(textures_skybox[i]);
		txm->beginTexturing();

		glBegin(GL_QUADS);
		switch (i)
		{
		case 0:
			glTexCoord2f(0, 1); glVertex3f(-1, 1, -1);
			glTexCoord2f(1, 1); glVertex3f(1, 1, -1);
			glTexCoord2f(1, 0); glVertex3f(1, 1, 1);
			glTexCoord2f(0, 0); glVertex3f(-1, 1, 1);
			break;
		case 1:
			glTexCoord2f(0, 1); glVertex3f(-1, 1, -1);
			glTexCoord2f(0, 0); glVertex3f(-1, -1, -1);
			glTexCoord2f(1, 0); glVertex3f(1, -1, -1);
			glTexCoord2f(1, 1); glVertex3f(1, 1, -1);
			break;
		case 2:
			glTexCoord2f(0, 1); glVertex3f(1, 1, -1);
			glTexCoord2f(0, 0); glVertex3f(1, -1, -1);
			glTexCoord2f(1, 0); glVertex3f(1, -1, 1);
			glTexCoord2f(1, 1); glVertex3f(1, 1, 1);
			break;
		case 3:
			glTexCoord2f(0, 1); glVertex3f(1, 1, 1);
			glTexCoord2f(0, 0); glVertex3f(1, -1, 1);
			glTexCoord2f(1, 0); glVertex3f(-1, -1, 1);
			glTexCoord2f(1, 1); glVertex3f(-1, 1, 1);
			break;
		case 4:
			glTexCoord2f(0, 1); glVertex3f(-1, 1, 1);
			glTexCoord2f(0, 0); glVertex3f(-1, -1, 1);
			glTexCoord2f(1, 0); glVertex3f(-1, -1, -1);
			glTexCoord2f(1, 1); glVertex3f(-1, 1, -1);
			break;
			/* case 5:
			glTexCoord2f(1, 1); glVertex3f(-1, -1, -1);
			glTexCoord2f(0, 1); glVertex3f(-1, -1, 1);
			glTexCoord2f(0, 0); glVertex3f(1, -1, 1);
			glTexCoord2f(1, 0); glVertex3f(1, -1, -1);
			break;
			*/
		default:
			break;
		}
		glEnd();
		txm->endTexturing();
	}
	glPopMatrix();
}

void CrossPortal() {
	if (player->GetPortalIsActive() && player->ReachPortal(posX, posY, posZ)) {
		posX = 120;
		posZ = 12*30;
		angleY = 0;
	}
}

GLuint generateMazeDisplayList()
{
	GLuint id = 0;
	id = glGenLists(1);
	if (!id) return id;  // failed to create a list, return 0
	glNewList(id, GL_COMPILE);
	GenerateTerrain();
	glEndList();
	return id;
}

GLuint generateGroundDisplayList()
{
	GLuint id = 0;
	id = glGenLists(1);
	if (!id) return id;  // failed to create a list, return 0
	glNewList(id, GL_COMPILE);
	grass();
	glEndList();
	return id;
}



/********************************************************************\
*                                                                    *
*  Démarre l'application (avant la main_loop). Renvoie false si      *
*  il y a une erreur.                                                *
*                                                                    *
\********************************************************************/
bool start()
{
	// initialisations de base
	// -----------------------
	win = new WINDOW();									// prépare la fenêtre
	win->create( 800, 600, 16 , 60 , false );			// crée la fenêtre

	cam = new CAMERA();

	inp = new MY_INPUT(win);								// initialise la gestion clavier souris
	create_context(*win);								// crée le contexte OpenGL sur la fenêtre
	init_font(*win, "Courier");							// initialise la gestion de texte avec OpenGL


	// initialisations d'OpenGL
	// ------------------------
	glClearColor(0.0f,0.0f,0.0f,1.0f);					// set clear color for color buffer (RGBA, black)
	glViewport(0,0,win->Xres,win->Yres);				// zone de rendu (tout l'écran)
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LESS);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// mapping quality = best
	glFrontFace(GL_CCW);								// front of face is defined counter clock wise
	glPolygonMode(GL_FRONT, GL_FILL);					// front of a face is filled
	glPolygonMode(GL_BACK,  GL_LINE);					// back of a face is made of lines
	glCullFace(GL_BACK);								// cull back face only
	glDisable(GL_CULL_FACE);						    // disable back face culling
	glAlphaFunc(GL_GREATER, 0.05);

	win->set_title("Reach the portal");

	/* Load ressources */

	texture_ground = new Texture();
	texture_ground->load_texture("textures/ground.tga", NULL);

	texture_key = new Texture();
	texture_key->load_texture("textures/key.tga", "textures/key_mask.tga");

	texture_portal = new Texture();
	texture_portal->load_texture("textures/portal.tga", "textures/portal_mask.tga");

	texture_hedge = new Texture();
	texture_hedge->load_texture("textures/grass.tga", NULL);

	texture_enemy = new Texture();
	texture_enemy->load_texture("textures/enemy.tga", "textures/enemy_mask.tga");

	texture_board = new Texture();
	texture_board->load_texture("textures/board.tga", "textures/board_mask.tga");

	myTerrain = new Terrain(30);
	txm = new TextureManager();

	mazeDL = generateMazeDisplayList();
	groundDL = generateGroundDisplayList();

	// Skybox textures
	for (int i = 0; i < 6; i++) {
		textures_skybox[i] = new Texture();
	}
	textures_skybox[0]->load_texture("textures/skybox/up.tga", NULL);
	textures_skybox[1]->load_texture("textures/skybox/front.tga", NULL);
	textures_skybox[2]->load_texture("textures/skybox/left.tga", NULL);
	textures_skybox[3]->load_texture("textures/skybox/back.tga", NULL);
	textures_skybox[4]->load_texture("textures/skybox/right.tga", NULL);
	textures_skybox[5]->load_texture("textures/skybox/down.tga", NULL);

	return true;
}

/********************************************************************\
*                                                                    *
* Fonctions customs                                                  *
*                                                                    *
\********************************************************************/



/********************************************************************\
*                                                                    *
*  les formes                                               *
*                                                                    *
\********************************************************************/

/********************************************************************\
*                                                                    *
*  Boucle principale, appelée pour construire une image, gérer les   *
*  entrées.                                                          *
*                                                                    *
\********************************************************************/
void main_loop()
{

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//				gestion des touches	et du temps													//
	//////////////////////////////////////////////////////////////////////////////////////////////////

	inp->refresh();
	inp->get_mouse_movement();


	if (inp->keys[KEY_CODE_ESCAPE])
	{
		PostMessage(win->handle,WM_CLOSE,0,0);	// Stoppe la "pompe à message" en y envoyant le message "QUIT"
	}



	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	//////////////////////////////////////////////////////////////////////////////////////////////////
	//						ça commence ici															//
	//////////////////////////////////////////////////////////////////////////////////////////////////

	// votre code OpenGL ici

	glMatrixMode(GL_PROJECTION);  //la matrice de projection sera celle selectionnee
	//composition d'une matrice de projection
	glLoadIdentity(); //on choisit la matrice identité
	gluPerspective(60,(double)win->Xres/(double)win->Yres,10,12000);   //mise en place d une proj angle de vue 60 deg near 10 far 30000
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//camera
	point dir = cam->direction - cam->position;
	if (inp->keys[KEY_CODE_UP]) {
		posX += 2*dir.x;
		posZ += 2*dir.z;
	}
	if (inp->keys[KEY_CODE_DOWN]) {
		posX -= 2*dir.x;
		posZ -= 2*dir.z;
	}
	if (inp->keys[KEY_CODE_RIGHT]) {
		posX += produit_vectoriel(dir, cam->orientation).x;
		posZ += produit_vectoriel(dir, cam->orientation).z;
	}
	if (inp->keys[KEY_CODE_LEFT]) {
		posX -= produit_vectoriel(dir, cam->orientation).x;
		posZ -= produit_vectoriel(dir, cam->orientation).z;
	}

	float size = 10;
	float size2 = 0.3;

	//mouvement de la souris
	angleX -= 0.1*(float)inp->Yrelmouse;       
	angleY -= 0.1*(float)inp->Xrelmouse;
	if (angleY > 360) angleY -= 360; if (angleY < 0) angleY += 360;
	if (angleX > 60) angleX = 60; if (angleX < -60) angleX = -60;
	
	cam->update(point(posX, posY, posZ), angleX, angleY, angleZ);
	gluLookAt(cam->position.x, cam->position.y, cam->position.z,
		cam->direction.x, cam->direction.y, cam->direction.z,
		cam->orientation.x, cam->orientation.y, cam->orientation.z);
	
	//write_2_screen("Welcome to the OpenGL laboratory");

	// **** Mon code ici ****

	drawSkybox();
	glCallList(mazeDL);
	glCallList(groundDL);
	SpawnElements();

	player->PickUpKey(posX, posY, posZ);
	player->ActivatePortal();
	CrossPortal();
	player->Hit(posX, posZ, enemy->GetEnemyPos());

	player->HitUI();
	if (player->Die()) {
		posX = 30;
		posY = 0;
		posZ = 180;
		angleY = 0;
	}
	player->PrintScore();
	player->PrintHealth();
	


	//////////////////////////////////////////////////////////////////////////////////////////////////
	//						ça finit ici															//
	//////////////////////////////////////////////////////////////////////////////////////////////////
	swap_buffer(win);	// affiche l'image composée à l'écran
}


/********************************************************************\
*                                                                    *
* Arrête l'application                                               *
*                                                                    *
\********************************************************************/
void stop()
{
	delete inp;

	if (win)
	{
		kill_font();
		kill_context(*win);
		delete win;
	}
}


/********************************************************************\
*                                                                    *
* Point d'entrée de notre programme pour Windows ("WIN32")           *
*                                                                    *
\********************************************************************/
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR szCmdLine,int iCmdShow)
{
	MSG  msg;

	win	= NULL;
	inp	= NULL;

	if (start() == false)								// initialise la fenêtre et OpenGL
	{
		debug("start() : échec !");
		stop();
		return 1;
	}

	// main loop //
	// --------- //
	//   __
	//  /  \_
	//  |  \/
	//  \__/

	while (true)
	{
		if (PeekMessage(&msg, NULL,0,0,PM_NOREMOVE))	// s'il y a un message, appelle WndProc() pour le traiter
		{
			if (!GetMessage(&msg,NULL,0,0))				// "pompe à message"
				break;
			TranslateMessage(&msg);
			DispatchMessage (&msg);
		}
		else
		{
			main_loop();								// sinon, appelle main_loop()
		}
	}


	stop();												// arrête OpenGL et ferme la fenêtre

	return 0;
}



/********************************************************************\
*                                                                    *
*  Boucle des messages                                               *
*                                                                    *
\********************************************************************/
LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{

  switch (msg)
	{

	  case WM_MOVE:        win->Xpos = (dword) LOWORD(lParam);
						   win->Ypos = (dword) HIWORD(lParam);
						   return 0;

	  case WM_CLOSE:       PostQuitMessage(0); // dit à GetMessage() de renvoyer 0
						   return 0;

	  case WM_SYSCOMMAND:  // évite l'économiseur d'écran
						   switch (wParam)
							 {
							   case SC_SCREENSAVE:
							   case SC_MONITORPOWER:
							   return 0;
							 }
						   break;
/*
	  case WM_CHAR:        la touche est traduite dans ce msg
						   return 0;
*/

	  case WM_KEYDOWN:     inp->set_key_down ((dword)wParam);
						   return 0;

	  case WM_KEYUP:       inp->set_key_up   ((dword)wParam);
						   return 0;

	  case WM_LBUTTONDOWN: inp->set_mouse_left_down();
						   return 0;

	  case WM_LBUTTONUP:   inp->set_mouse_left_up();
						   return 0;

	  case WM_RBUTTONDOWN: inp->set_mouse_right_down();
						   return 0;

	  case WM_RBUTTONUP:   inp->set_mouse_right_up();
						   return 0;

	  case WM_MBUTTONDOWN: inp->set_mouse_middle_down();
						   return 0;

	  case WM_MBUTTONUP:   inp->set_mouse_middle_up();
						   return 0;

   }

  return DefWindowProc(hwnd,msg,wParam,lParam);
}

/* Triangle */

/*glBegin(GL_TRIANGLES);
glColor3f(1,0,0); glVertex3f(-50,-50, -10);
glColor3f(0,1,0); glVertex3f(+50,-50, -10);
glColor3f(0,0,1); glVertex3f(  0,+50, -10);
glEnd();*/

/* Pyramid */
/*
glBegin(GL_QUADS);
glColor3f(1, 0, 0); glVertex3f(-50, -50, -50);
glColor3f(1, 0, 0); glVertex3f(-50, -50, 50);
glColor3f(1, 0, 0); glVertex3f(50, -50, 50);
glColor3f(1, 0, 0); glVertex3f(50, -50, -50);
glEnd();
glBegin(GL_TRIANGLES);
glColor3f(1, 0, 0); glVertex3f(-50, -50, -50);
glColor3f(1, 0, 0); glVertex3f(-50, -50, 50);
glColor3f(0, 0, 1); glVertex3f(0, 50, 0);
glEnd();
glBegin(GL_TRIANGLES);
glColor3f(1, 0, 0); glVertex3f(-50, -50, 50);
glColor3f(1, 0, 0); glVertex3f(50, -50, 50);
glColor3f(0, 0, 1); glVertex3f(0, 50, 0);
glEnd();
glBegin(GL_TRIANGLES);
glColor3f(1, 0, 0); glVertex3f(50, -50, 50);
glColor3f(1, 0, 0); glVertex3f(50, -50, -50);
glColor3f(0, 0, 1); glVertex3f(0, 50, 0);
glEnd();
glBegin(GL_TRIANGLES);
glColor3f(1, 0, 0); glVertex3f(50, -50, -50);
glColor3f(1, 0, 0); glVertex3f(-50, -50, -50);
glColor3f(0, 0, 1); glVertex3f(0, 50, 0);
glEnd();
*/