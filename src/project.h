#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include "../util/glutil.h"
#include <cstdlib>
#include <cmath>
#include "ModelLoader.h"
#include "Material.h"
#include "TexturedRectangle.h"
#include "TexturedCube.h"
#include "Transformation.h"
#include "CollisionDetector.h"
#include "PlayerCamera.h"

#define PI 3.141592653589793

/**************************************************
    RELATIVE PATHS TO THE DIFFERENT BODY PARTS
 **************************************************/
char* BODY_PATH = "./models/cat/body.sgf";
char* RIGHT_LEG_PATH = "./models/cat/leg_right.sgf";
char* LEFT_LEG_PATH = "./models/cat/leg_left.sgf";
char* RIGHT_HAND_PATH = "./models/cat/hand_right.sgf";
char* LEFT_HAND_PATH = "./models/cat/hand_left.sgf";
char* TAIL_PATH = "./models/cat/tail.sgf";
char* TEETH_PATH = "./models/cat/teeth.sgf";
char* WHISKERS_PATH = "./models/cat/whiskers.sgf";
char* RIGHT_EYE_PATH = "./models/cat/eye_right.sgf";
char* LEFT_EYE_PATH = "./models/cat/eye_left.sgf";
char* NOSE_PATH = "./models/cat/nose.sgf";
char* GROUND_PATH = "./textures/brick.rgb";
char* BOXES_PATH = "./textures/rock.rgb";

/**************************************************
    RELATIVE PATHS TO THE TEXTURES OF THE CATS
 **************************************************/
char* PLAYER_BODY_TEXTURE_PATH = "./textures/ground.rgb";
char* PLAYER_HANDLEGTAIL_TEXTURE_PATH = "./textures/rock.rgb";
char* ENEMY_BODY_TEXTURE_PATH = "./textures/rock.rgb";
char* ENEMY_HANDLEGTAIL_TEXTURE_PATH = "./textures/ground.rgb";


/**************************************************
 CONSTANTS AND VARIABLES USED FOR DAY-NIGHT AND 
                    CATS ANIMATIONS
 **************************************************/
const int BOXES = 7;
const int CATS = 3;
int animationPaused = 0;
double timeOfDay = 0.0;
double currTime = 0.0;
float dayColor[] = {0.0, 0.0, 0.0};
float redMaterialColor[] = {1.0, 0.0, 0.0, 1.0};

/* to either rotate to a certain director or another */
int rightHandFactor = -1;
int leftHandFactor = 1;
int rightLegFactor = -1;
int leftLegFactor = 1;

/**************************************************
      SCENE GRAPHS OBJECTS USED IN THE PROGRAM
 **************************************************/
GraphicalObject* body[CATS];
GraphicalObject* rightLeg[CATS];
GraphicalObject* leftLeg[CATS];
GraphicalObject* rightHand[CATS];
GraphicalObject* leftHand[CATS];
GraphicalObject* tail[CATS];
GraphicalObject* rightEye[CATS];
GraphicalObject* leftEye[CATS];
GraphicalObject* whiskers[CATS];
GraphicalObject* teeth[CATS];
GraphicalObject* nose[CATS];

Material* eyes[3][2];
Material* teethMaterial[3];
Material* whiskersMaterial[3];

Transformation* rightHandAnimation[CATS];
Transformation* leftHandAnimation[CATS];
Transformation* rightLegAnimation[CATS];
Transformation* leftLegAnimation[CATS];

TexturedRectangle* ground;
TexturedCube* boxes[BOXES];
Transformation* boxesTransformations[BOXES];
Transformation* catsTransformations[CATS];
BoundingBox* boxesBoundingBoxes[BOXES];

PlayerCamera* camera;

/**************************************************
        PROPERTIES FOR THE TWO LIGHT SOURCES
 **************************************************/
GLfloat lightPos[] = {-2.0, 2.0, -2.0, 1.0};
GLfloat lightAmb[] = {1.0, 1.0, 1.0, 1.0};
GLfloat lightDiffu[] = {1.0, 1.0, 1.0, 1.0};
GLfloat lightSpec[] = {1.0, 1.0, 1.0, 1.0};

GLfloat lightPos1[] = {2.0, 2.0, 2.0, 1.0};
GLfloat lightAmb1[] = {1.0, 0.0, 0.0, 1.0};
GLfloat lightDiffu1[] = {1.0, 0.0, 0.0, 1.0};

float worldCoords[4][3] = {{-20.0, 0.0, -20.0}, {-20.0, 0.0, 20.0}, {20.0, 0.0, 20.0}, {20.0, 0.0, -20.0}};


/**************************************************
               PROTOTYPE DECLARATIONS
 **************************************************/

void init(); // to configure the basic things of the program 
void setupWorldObjects(); // to configure the scene tree
void animateCats(); // to animate the cats legs and arms
void moveEnemyCats(); // to displace the enemy cats along the world
void display(); // the gl display 
void reshape(int width, int height); // the reshape function 
void idle(); // the idle function 
void keyBoard(unsigned char key, int x, int y); // the keyboard function
void catMove(int key, int x, int y); // the player cat keyboard-controlled function
