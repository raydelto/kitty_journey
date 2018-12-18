#define GLUT_BUILDING_LIB

#include "project.h"

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);  
	glutInitWindowPosition(0, 0);
    glutCreateWindow("Final Project");
	
	glutIdleFunc(idle);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyBoard);
	glutSpecialFunc(catMove);

	init();
	glutMainLoop();
}


void init()
{
	/** setup perspective view with a 8/6 aspect ratio */
	glEnable(GL_DEPTH_TEST); 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 800.0 / 600.0, 1.0, 40.0); 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_NORMALIZE);	

	/** enable and configure the first light source */
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffu);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpec);

	/** enable and configure the second light source */
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmb1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffu1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpec);

	/** enable and configure fog*/
	glFogi(GL_FOG_MODE, GL_EXP2);
	GLfloat fogColor[4] = {0.5f, 0.5f, 0.5f, 1.0f};
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, 0.05f);
	glHint(GL_FOG_HINT, GL_DONT_CARE);
	glFogf(GL_FOG_START, 1.0f);
	glFogf(GL_FOG_END, 5.0f);
	glEnable(GL_FOG);
	
	/** use smooth shading */ 
	glEnable(GL_SMOOTH);
	glShadeModel(GL_SMOOTH);

	setupWorldObjects();
}

void setupWorldObjects()
{
	/* to setup the different "buildings" of my world*/
	float boxesScales[7][3] = {{1.0, 3.0, 1.0}, {1.0, 2.0, 1.0}, {1.0, 7.0, 1.0}, {1.0, 2.0, 1.0},
	{1.0, 4.0, 2.0}, {1.0, 2.0, 1.0}, {1.0, 2.0, 1.0}};
	float boxesTranslations[7][3] = {{10.0, 0.0, 10.0}, {-10.0, 0.0, 10.0}, {0.0, 0.0, 0.0}, 
	{8.0, 0.0, 7.0}, {-7.0, 0.0, -6.0}, {5.0, 0.0, -4.0}, {0.0, 0.0, 4.0}};
	
	/* some material properties that do not change from object to object*/

	float specular[4] = {1.0, 1.0, 1.0, 1.0};
	float emission[4] = {0.00, 0.0, 0.0, 1.0};
	float shini[1] = {40};
	
	ground = new TexturedRectangle();
	ground->SetTexture(GROUND_PATH);
	ground->SetVertices(worldCoords);

	/* make buildings */
	for (int i = 0; i < BOXES; i++)
	{
		if (i == 0)
			boxes[i] = new TexturedCube(BOXES_PATH);
		else 
			boxes[i] = new TexturedCube(boxes[i - 1]);

		boxesTransformations[i] = new Transformation();
		boxesTransformations[i]->SetTranslation(boxesTranslations[i][0], 
			boxesTranslations[i][1], boxesTranslations[i][2]);
		boxesTransformations[i]->SetScale(boxesScales[i][0], boxesScales[i][1], boxesScales[i][2]);
		boxesBoundingBoxes[i] = CollisionDetector::GetBoundingBox(boxesTransformations[i]->GetTransformation(0),
			boxesTransformations[i]->GetTransformation(1)); // create bounding boxes 
		ground->AddChild(boxesTransformations[i]);
		boxesTransformations[i]->AddChild(boxes[i]);

	}
	for (int i = 0; i < CATS; i++) // setup cats 
	{
		eyes[i][0] = new Material();
		eyes[i][1] = new Material();
		teethMaterial[i] = new Material();
		whiskersMaterial[i] = new Material();
		if (i == 0) // if its the first cat, load everything from the disc
		{
			body[0] = new GraphicalObject(BODY_PATH);
			rightLeg[0] = new GraphicalObject(RIGHT_LEG_PATH);
			leftLeg[0] = new GraphicalObject(LEFT_LEG_PATH);
			rightHand[0] = new GraphicalObject(RIGHT_HAND_PATH);
			leftHand[0] = new GraphicalObject(LEFT_HAND_PATH);
			rightEye[0] = new GraphicalObject(RIGHT_EYE_PATH);
			leftEye[0] = new GraphicalObject(LEFT_EYE_PATH);
			nose[0] = new GraphicalObject(NOSE_PATH);
			whiskers[0] = new GraphicalObject(WHISKERS_PATH);
			tail[0] = new GraphicalObject(TAIL_PATH);
			teeth[0] = new GraphicalObject(TEETH_PATH);
		}
		else 
		{ //otherwise, just clone the first cat 
			body[i] = new GraphicalObject(body[0]);
			rightLeg[i] = new GraphicalObject(rightLeg[0]);
			leftLeg[i] = new GraphicalObject(leftLeg[0]);
			rightHand[i] = new GraphicalObject(rightHand[0]);
			leftHand[i] = new GraphicalObject(leftHand[0]);
			rightEye[i] = new GraphicalObject(rightEye[0]);
			leftEye[i] = new GraphicalObject(leftEye[0]);
			nose[i] = new GraphicalObject(nose[0]);
			whiskers[i] = new GraphicalObject(whiskers[0]);
			tail[i] = new GraphicalObject(tail[0]);
			teeth[i] = new GraphicalObject(teeth[0]);
		}
		
		for (int j = 0; j < 2; j++)
		{
			eyes[i][j]->SetValue(GL_AMBIENT, redMaterialColor); // initially red because 
			eyes[i][j]->SetValue(GL_DIFFUSE, redMaterialColor); // initially its at night and 
			eyes[i][j]->SetValue(GL_SPECULAR, specular);      //THESE cats are red eyed at night
			eyes[i][j]->SetValue(GL_SHININESS, shini);
			eyes[i][j]->SetValue(GL_EMISSION, emission);
			
			if (j == 0)
			{
				teethMaterial[i]->SetValue(GL_AMBIENT, redMaterialColor);
				teethMaterial[i]->SetValue(GL_DIFFUSE, redMaterialColor);
				teethMaterial[i]->SetValue(GL_SPECULAR, specular);
				teethMaterial[i]->SetValue(GL_SHININESS, shini);
				teethMaterial[i]->SetValue(GL_EMISSION, emission);

				whiskersMaterial[i]->SetValue(GL_AMBIENT, redMaterialColor);
				whiskersMaterial[i]->SetValue(GL_DIFFUSE, redMaterialColor);
				whiskersMaterial[i]->SetValue(GL_SPECULAR, specular);
				whiskersMaterial[i]->SetValue(GL_SHININESS, shini);
				whiskersMaterial[i]->SetValue(GL_EMISSION, emission);
			}
		}
		if (i == 0)
		{
			body[0]->LoadTexture(PLAYER_BODY_TEXTURE_PATH);
			rightLeg[0]->LoadTexture(PLAYER_HANDLEGTAIL_TEXTURE_PATH);
			leftLeg[0]->SetTexture(rightLeg[0]->GetTexture());
			rightHand[0]->SetTexture(rightLeg[0]->GetTexture());
			leftHand[0]->SetTexture(rightLeg[0]->GetTexture());
			nose[0]->SetTexture(body[0]->GetTexture());
			tail[0]->SetTexture(body[0]->GetTexture());
			
			/* setup() the first cat to assign them a display list and 
			   have the possibility of being cloned correctly. 
			   See GraphicalObject class. */
			body[0]->Setup(); 
			rightLeg[0]->Setup();
			leftLeg[0]->Setup();
			rightHand[0]->Setup();
			leftHand[0]->Setup();
			rightEye[0]->Setup();
			leftEye[0]->Setup();
			nose[0]->Setup();
			whiskers[0]->Setup();
			tail[0]->Setup();
			teeth[0]->Setup();

		}
		else
		{
			body[i]->SetTexture(rightLeg[0]->GetTexture());
			rightLeg[i]->SetTexture(body[0]->GetTexture());
			leftLeg[i]->SetTexture(body[0]->GetTexture());
			rightHand[i]->SetTexture(body[0]->GetTexture());
			leftHand[i]->SetTexture(body[0]->GetTexture());
			nose[i]->SetTexture(rightLeg[0]->GetTexture());
			tail[i]->SetTexture(rightLeg[0]->GetTexture());
		}

		/** setup all the animation transformations that will be 
		 *  worked with in the idle function and the player cat
		 *  keyboard function
		 */
		rightLegAnimation[i] = new Transformation();
		rightLegAnimation[i]->SetTranslation(-0.01, 0.01, 0.0);
		rightLegAnimation[i]->SetRotation(-10.0, 1.0, 0.0, 0.0);
		rightLegAnimation[i]->SetTranslation(0.01, -0.01, 0.0);
		rightLegAnimation[i]->AddChild(rightLeg[i]);
		body[i]->AddChild(rightLegAnimation[i]);

		leftLegAnimation[i] = new Transformation();
		leftLegAnimation[i]->SetTranslation(0.01, 0.01, 0.0);
		leftLegAnimation[i]->SetRotation(10.0, 1.0, 0.0, 0.0);
		leftLegAnimation[i]->SetTranslation(-0.01, -0.01, 0.0);
		leftLegAnimation[i]->AddChild(leftLeg[i]);
		body[i]->AddChild(leftLegAnimation[i]);

		rightHandAnimation[i] = new Transformation();
		rightHandAnimation[i]->SetTranslation(0.03, 0.0, 0.0);
		rightHandAnimation[i]->SetRotation(0.0, 0.0, 0.0, 1.0);
		rightHandAnimation[i]->SetTranslation(-0.03, 0.0, 0.0);
		rightHandAnimation[i]->AddChild(rightHand[i]);
		body[i]->AddChild(rightHandAnimation[i]);

		leftHandAnimation[i] = new Transformation();
		leftHandAnimation[i]->SetTranslation(-0.03, 0.0, 0.0);
		leftHandAnimation[i]->SetRotation(0.0, 0.0, 0.0, 1.0);
		leftHandAnimation[i]->SetTranslation(0.03, 0.0, 0.0);
		leftHandAnimation[i]->AddChild(leftHand[i]);
		body[i]->AddChild(leftHandAnimation[i]);
		
		eyes[i][0]->AddChild(rightEye[i]);
		body[i]->AddChild(eyes[i][0]);
		eyes[i][1]->AddChild(leftEye[i]);
		body[i]->AddChild(eyes[i][1]);
		body[i]->AddChild(nose[i]);
		whiskersMaterial[i]->AddChild(whiskers[i]);
		body[i]->AddChild(whiskersMaterial[i]);
		body[i]->AddChild(tail[i]);
		teethMaterial[i]->AddChild(teeth[i]);
		body[i]->AddChild(teethMaterial[i]);
	}
	/* get all the cats on a staring up position and pointing to
	   a certain direction */
	catsTransformations[0] = new Transformation();
	catsTransformations[0]->SetTranslation(0.0, 0.5, 18.0);
	catsTransformations[0]->SetRotation(270.0, 1.0, 0.0, 0.0);
	catsTransformations[0]->SetRotation(0.0, 0.0, 0.0, 1.0);
	ground->AddChild(catsTransformations[0]);
	catsTransformations[0]->AddChild(body[0]);
	
	catsTransformations[1] = new Transformation();
	catsTransformations[1]->SetTranslation(-18.0, 0.5, 0.0);
	catsTransformations[1]->SetRotation(270.0, 1.0, 0.0, 0.0);
	catsTransformations[1]->SetRotation(270.0, 0.0, 0.0, 1.0);
	ground->AddChild(catsTransformations[1]);
	catsTransformations[1]->AddChild(body[1]);

	catsTransformations[2] = new Transformation();
	catsTransformations[2]->SetTranslation(18.0, 0.5, 0.0);
	catsTransformations[2]->SetRotation(270.0, 1.0, 0.0, 0.0);
	catsTransformations[2]->SetRotation(90.0, 0.0, 0.0, 1.0);
	ground->AddChild(catsTransformations[2]);
	catsTransformations[2]->AddChild(body[2]);
	
	/* make the camera point to cat0 and be behind it*/
	camera = new PlayerCamera(catsTransformations[0]->GetTransformation(0), 0.0);
	camera->AddChild(ground);

	

}

void keyBoard(unsigned char key, int x, int y)
{
	const char ESCAPE_CODE = 27;
	const char Q_CODE = 113;
	const char SPACE_CODE = 32;

	if ( (key == ESCAPE_CODE) || (key == Q_CODE) )
	{
		exit(EXIT_SUCCESS);
	}
	else if (key == SPACE_CODE)
	{
		animationPaused = !animationPaused;
		timeOfDay = 0.0; // to correct the day-night animation when paused/resumed
	}

}
void catMove(int key, int x, int y)
{
	if (animationPaused)
		return;

	const int POSITION_INDEX = 0;
	const int ROTATION_INDEX = 2;

	int change = 1;

	float newPosition[3]; // to save the potential new position in a temp variable
	                      // as to prevent from collision
	float* position = catsTransformations[0]->GetTransformation(POSITION_INDEX);
	float* rotation = catsTransformations[0]->GetTransformation(ROTATION_INDEX);
	for (int i = 0; i < 3; i++) newPosition[i] = position[i];
	
	/** calculate potential position and angles */
	if (key == GLUT_KEY_UP)
	{
		newPosition[0] += -0.10 * sin(rotation[0] * PI / 180.0);
		newPosition[2] += -0.10 * cos(rotation[0] * PI / 180.0);
	}
	else if (key == GLUT_KEY_DOWN)
	{
		newPosition[0] += 0.10 * sin(rotation[0] * PI / 180.0);
		newPosition[2] += 0.10 * cos(rotation[0] * PI / 180.0);
	}
	else if (key == GLUT_KEY_RIGHT)
	{
		rotation[0] -= 5.0;
		camera->UpdatePlayerAngle(rotation[0]);
	}
	else if (key == GLUT_KEY_LEFT)
	{
		rotation[0] += 5.0;
		camera->UpdatePlayerAngle(rotation[0]);
	}

	/* See whether or not the new calculated position is clear from other
	   objects. That is, first it must not collide with the buildings,
	   then with the other cats, and then it must not be outside the bounds
	   of the world. 
	 */
	BoundingBox* cat0 = CollisionDetector::GetBoundingBox(newPosition, 0.5, 0.5);
	BoundingBox* cat1 = CollisionDetector::GetBoundingBox(catsTransformations[1]->GetTransformation(0), 0.5, 0.5);
	BoundingBox* cat2 = CollisionDetector::GetBoundingBox(catsTransformations[2]->GetTransformation(0), 0.5, 0.5);
	for (int i = 0; i <= BOXES; i++)
	{
		if (CollisionDetector::Collide(cat0, boxesBoundingBoxes[i]))
		{
			change = 0;
			break;
		}
	}

	if (CollisionDetector::Collide(cat0, cat1) || CollisionDetector::Collide(cat0, cat2))
	{
		change = 0;
	}
	
	// within ground bounds
	if (!(newPosition[0] > -19.0 && newPosition[0] < 19.0 && newPosition[2] > -19.0 && newPosition[2] < 19.0))
	{
		change = 0;
	}
	delete cat0; delete cat1; delete cat2;
	
	if (change) // the new position is good 
	{
	    for (int i = 0; i < 3; i++) position[i] = newPosition[i];
		camera->UpdateView(position); // tell camera the new position
	}

}

void animateCats()
{
	const int ROTATION_INDEX = 1;
	float* rightHandRotations[CATS];
	float* leftHandRotations[CATS];
	float* rightLegRotations[CATS];
	float* leftLegRotations[CATS];

	for (int i = 0; i < CATS; i++)
	{
		rightHandRotations[i] = rightHandAnimation[i]->GetTransformation(ROTATION_INDEX);
		leftHandRotations[i] = leftHandAnimation[i]->GetTransformation(ROTATION_INDEX);
		rightLegRotations[i] = rightLegAnimation[i]->GetTransformation(ROTATION_INDEX);
		leftLegRotations[i] = leftLegAnimation[i]->GetTransformation(ROTATION_INDEX);
	}

	// if either rotation reached its maximum angle, rotate "back" 
	if (rightHandRotations[0][0] <= 0.0 || rightHandRotations[0][0] >= 25.0)
		rightHandFactor *= -1; 
	if (leftHandRotations[0][0] <= -25.0 || leftHandRotations[0][0] >= 0.0)
		leftHandFactor *= -1;
	if (rightLegRotations[0][0] <= -10.0 || rightLegRotations[0][0] >= 10.0)
		rightLegFactor *= -1;
	if (leftLegRotations[0][0] <= -10.0 || leftLegRotations[0][0] >= 10.0)
		leftLegFactor *= -1;
	
	for (int i = 0; i < CATS; i++)
	{ // update angles
		rightHandRotations[i][0] += rightHandFactor;
		leftHandRotations[i][0] += leftHandFactor;
		rightLegRotations[i][0] += rightLegFactor;
		leftLegRotations[i][0] += leftLegFactor;
	}
}

void moveEnemyCats()
{
	const int POSITION_INDEX = 0;
	const int ROTATION_INDEX = 2;
	float* enemyPositions[2];
	float* enemyRotations[2];
	float tempNewPosition[3]; // potential new position to allocate enemycati
	float positionRanges[][2] = {{-18.0, -2.0}, {18.0, 2.0}};
	float angleRanges[][2] = {{270, 450}, {90, 270}};
	float positionOffset[] = {0.05, -0.05};
	float rotationOffset = 5.0;
	BoundingBox* playerBox = CollisionDetector::GetBoundingBox(catsTransformations[0]->GetTransformation(POSITION_INDEX),
		0.5, 0.5);
	
	BoundingBox* currentEnemyBox = NULL;
	
	for (int i = 0; i < 2; i++)
	{
		enemyPositions[i] = catsTransformations[i + 1]->GetTransformation(POSITION_INDEX);
		enemyRotations[i] = catsTransformations[i + 1]->GetTransformation(ROTATION_INDEX);
		tempNewPosition[0] = enemyPositions[i][0]; tempNewPosition[1] = enemyPositions[i][1];
		tempNewPosition[2] = enemyPositions[i][2];

		if (enemyRotations[i][0] == angleRanges[i][0] && floor(enemyPositions[i][0]) != positionRanges[i][1])
		{
			tempNewPosition[0] += positionOffset[i];	
		}
		else if (floor(enemyPositions[i][0]) == positionRanges[i][1] && enemyRotations[i][0] != angleRanges[i][1])
		{
			enemyRotations[i][0] += rotationOffset;
		}
		else if (enemyRotations[i][0] == angleRanges[i][1] && floor(enemyPositions[i][0]) != positionRanges[i][0])
		{
			tempNewPosition[0] += (-1 * positionOffset[i]);
		}
		else if (floor(enemyPositions[i][0]) == positionRanges[i][0] && enemyRotations[i][0] != angleRanges[i][0])
		{
			enemyRotations[i][0] += (-1 * rotationOffset);
		}
		
		currentEnemyBox = CollisionDetector::GetBoundingBox(tempNewPosition, 0.5, 0.5);
		if (!CollisionDetector::Collide(playerBox, currentEnemyBox))
		{ // the enemy cats move in a straight line and can only collapse with
		  // the player cat. If no collision, move enemy cat i to potential position
			enemyPositions[i][0] = tempNewPosition[0];
			enemyPositions[i][1] = tempNewPosition[1];
			enemyPositions[i][2] = tempNewPosition[2];
		}
		delete currentEnemyBox;
	}
	delete playerBox;
}
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glClearColor(dayColor[0], dayColor[1], dayColor[2], 1.0);
	camera->Render();
	glPopMatrix();
	glutSwapBuffers();
}

void reshape(int width, int height)
{
	float worldAspect = 800.0 / 600.0;
	float windowAspect = width / height;
    int newWidth = 0.0;
	int newHeight = 0.0;
	if (worldAspect == windowAspect) // same width and height
	{
		newWidth = width;  
		newHeight = height;
	}
	else if (windowAspect < worldAspect) // horizontal viewing rectangle
	{
		newWidth = height * worldAspect;
		newHeight = height;
	}
	else     // vertical viewing rectangle
	{
		newWidth = width;
		newHeight = width / worldAspect;
	}

	glViewport(0, 0, newWidth, newHeight);
}

void idle(void)
{
	if (!animationPaused)
	{
		if (timeOfDay == 0.0)
		{
			timeOfDay = Wallclock(); // is a new day. Start off from wallclock()
		}
		else
		{
			double temp = Wallclock();
			currTime += temp - timeOfDay; // get delta
			timeOfDay = temp;
			if (currTime >= 6.0) // 6AM
			{
				glDisable(GL_FOG); //fog only in the night!!
				dayColor[2] = 0.30;
				for (int i = 0; i < CATS; i++)
				{
				    for (int j = 0; j < 2; j++)
					{
						eyes[i][j]->SetValue(GL_AMBIENT, leftEye[0]->GetColor());
						eyes[i][j]->SetValue(GL_DIFFUSE, leftEye[0]->GetColor());
					}
					whiskersMaterial[i]->SetValue(GL_AMBIENT, whiskers[0]->GetColor());
					whiskersMaterial[i]->SetValue(GL_DIFFUSE, whiskers[0]->GetColor());
					teethMaterial[i]->SetValue(GL_AMBIENT, teeth[0]->GetColor());
					teethMaterial[i]->SetValue(GL_DIFFUSE, teeth[0]->GetColor());
				}
			}
			if (currTime >= 7.0) // 7 AM
				dayColor[2] = 0.40;
			if (currTime >= 8.0)  // 8 AM
				dayColor[2] = 0.60;
			if (currTime >= 18.0) // 6 PM
				dayColor[2] = 0.40;
			if (currTime >= 19.0) // 7 PM
				dayColor[2] = 0.30;
			if (currTime >= 20.0) // 8 PM 
			{
				dayColor[2] = 0.0;
				glEnable(GL_FOG); //its night time, fog time. 
				for (int i = 0; i < CATS; i++)
				{
				    for (int j = 0; j < 2; j++)
					{
						eyes[i][j]->SetValue(GL_AMBIENT, redMaterialColor); // turn scary
						eyes[i][j]->SetValue(GL_DIFFUSE, redMaterialColor);
					}
					whiskersMaterial[i]->SetValue(GL_AMBIENT, redMaterialColor);
					whiskersMaterial[i]->SetValue(GL_DIFFUSE, redMaterialColor);
					teethMaterial[i]->SetValue(GL_AMBIENT, redMaterialColor);
					teethMaterial[i]->SetValue(GL_DIFFUSE, redMaterialColor);
				}
			}
			if (currTime >= 24.0) // end of day, reset
			{
				timeOfDay = 0.0;
				currTime = 0.0;
			}

		}

	    animateCats();
	    moveEnemyCats();
	}
	glutPostRedisplay();
}




