// ========================================================================= //
// Authors: Felix Schuwirth                                                  //
// mailto:felix.schuwirth@igd.fraunhofer.de                                  //
//                                                                           //
// GRIS - Graphisch Interaktive Systeme                                      //
// Technische Universität Darmstadt                                          //
// Fraunhoferstrasse 5                                                       //
// D-64283 Darmstadt, Germany                                                //
//                                                                           //
// Creation Date: 12.01.2017                                                 //
// Content: Simple and extendable openGL program offering                    //
//   * basic navigation and basic pipeline rendering                         //
// ========================================================================= //

#include "main.h"		// this header

#include <stdlib.h>		// standard library
#include <cmath>		// fmod
#include <stdio.h>		// cout
#include <iostream>		// cout
#include "RenderingSurface.h"

// ==============
// === BASICS ===
// ==============

int main(int argc, char** argv)
{
	// initialize openGL window
	glutInit(&argc, argv);
	glutInitWindowPosition(300, 200);
	glutInitWindowSize(600, 400);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("TU Darmstadt, GMCAD P3"); 
	// link functions to certain openGL events
	glutDisplayFunc(renderScene);
	glutReshapeFunc(reshape);  
	glutMouseFunc(mousePressed);
	glutMotionFunc(mouseMoved);
	glutKeyboardFunc(keyPressed);
	// further initializations
	setDefaults();
	createNURBSs();
	calculatePoints();
	initializeGL();
	// activate main loop
	glutMainLoop();
	return 0;
}

void setDefaults()
{
	// scene Information
	transX = -1.0f;
	transY = -1.0f;
	transZ = -4.0f;
	angleX = 0.0f;
	angleY = 0.0f;
	// mouse information
	mouseX = 0;
	mouseY = 0;
	mouseButton = 0;
	mouseSensitivy = 1.0f;	

	enableCtrl = true;
	enableEval = 0;
	nurbsSelect = 0;
	nrPoints = 30;
	u = 0.5f;
	v = 0.5f;
}

void initializeGL()
{
	// black screen
	glClearColor(0, 0, 0, 0);
	// enable depth buffer
	glEnable(GL_DEPTH_TEST);
	// Use Point Smoothing
	glEnable(GL_POINT_SMOOTH);
	glPointSize(10.0f);
	glLineWidth(2.0f);
	// set shading model
	glShadeModel(GL_SMOOTH);
	// set lighting (white light)
	GLfloat global_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };  
	GLfloat ambientLight[] =   { 0.1f, 0.1f, 0.1f, 1.0f };
	GLfloat diffuseLight[] =   { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specularLight[] =  { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat shininess = 0.9f * 128.0f;
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightf(GL_LIGHT0, GL_SHININESS, shininess);
	glEnable(GL_LIGHT0);
	GLfloat lp[] = { 10.0f, 20.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lp);
	// enable lighting by glColor
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	// key bindings => cout
	coutHelp();
}

void createNURBSs() {
	NURBSs.clear();
	resolutionU.clear();
	resolutionV.clear();

	// objects (test surface via empty constructor)
	NURBS_Surface nurbs1 = NURBS_Surface();
	NURBSs.push_back(nurbs1);
	resolutionU.push_back(0.05f);
	resolutionV.push_back(0.05f);

	NURBS_Surface nurbs2;
	{
		std::vector<std::vector<Vec4f>> controlPoints;
		std::vector<float> knotVectorU;
		std::vector<float> knotVectorV;
		unsigned int degree = 2;;
		std::vector<Vec4f> pRow1;
		pRow1.push_back(Vec4f(0.0f, 2.0f, 0.0f, 1.0f));
		pRow1.push_back(Vec4f(1.0f, 2.0f, 0.0f, 1.0f));
		pRow1.push_back(Vec4f(1.0f, 0.0f, 0.0f, 1.0f) * 2.0f);
		controlPoints.push_back(pRow1);

		std::vector<Vec4f> pRow2;
		pRow2.push_back(Vec4f(0.0f, 2.0f, -1.0f, 1.0f));
		pRow2.push_back(Vec4f(2.0f, 2.0f, -1.0f, 1.0f) * 6.0f);
		pRow2.push_back(Vec4f(2.0f, 0.0f, -1.0f, 1.0f) * 2.0f);
		controlPoints.push_back(pRow2);

		std::vector<Vec4f> pRow3;
		pRow3.push_back(Vec4f(0.0f, 1.0f, -2.0f, 1.0f));
		pRow3.push_back(Vec4f(5.0f, 1.0f, -2.0f, 1.0f));
		pRow3.push_back(Vec4f(7.0f, 4.0f, -2.0f, 1.0f) * 2.0f);
		controlPoints.push_back(pRow3);

		knotVectorU.push_back(0.0f);
		knotVectorU.push_back(0.0f);
		knotVectorU.push_back(0.0f);
		knotVectorU.push_back(1.0f);
		knotVectorU.push_back(1.0f);
		knotVectorU.push_back(1.0f);

		knotVectorV.push_back(0.0f);
		knotVectorV.push_back(0.0f);
		knotVectorV.push_back(0.0f);
		knotVectorV.push_back(1.0f);
		knotVectorV.push_back(1.0f);
		knotVectorV.push_back(1.0f);


		nurbs2 = NURBS_Surface(controlPoints, knotVectorU, knotVectorV, degree);
		NURBSs.push_back(nurbs2);
		resolutionU.push_back(0.01f);
		resolutionV.push_back(0.01f);
	}

	NURBS_Surface nurbs3;
	{
		std::vector<std::vector<Vec4f>> controlPoints;
		std::vector<float> knotVectorU;
		std::vector<float> knotVectorV;
		unsigned int degree = 2;;


		std::vector<Vec4f> pRow0;
		pRow0.push_back(Vec4f(0.0f, 0.5f, 0.5f, 1.0f) * 1.0f);
		pRow0.push_back(Vec4f(-0.0f, 0.5f, 0.5f, 1.0f) * 0.7071f);
		pRow0.push_back(Vec4f(-0.0f, 0.5f, 0.5f, 1.0f) * 1.0f);
		pRow0.push_back(Vec4f(-0.0f, 0.5f, 0.5f, 1.0f) * 0.7071f);
		pRow0.push_back(Vec4f(0.0f, 0.5f, 0.5f, 1.0f) * 1.0f);
		pRow0.push_back(Vec4f(0.0f, 0.5f, 0.5f, 1.0f) * 0.7071f);
		pRow0.push_back(Vec4f(0.0f, 0.5f, 0.5f, 1.0f) * 1.0f);
		pRow0.push_back(Vec4f(0.0f, 0.5f, 0.5f, 1.0f) * 0.7071f);
		pRow0.push_back(Vec4f(0.0f, 0.5f, 0.5f, 1.0f) * 1.0f);
		controlPoints.push_back(pRow0);

		std::vector<Vec4f> pRow2;
		pRow2.push_back(Vec4f(0.0f, 0.0f, 0.5f, 1.0f) * 1.0f);
		pRow2.push_back(Vec4f(-0.5f, 0.0f, 0.5f, 1.0f) * 0.7071f);
		pRow2.push_back(Vec4f(-0.5f, 0.5f, 0.5f, 1.0f) * 1.0f);
		pRow2.push_back(Vec4f(-0.5f, 1.0f, 0.5f, 1.0f) * 0.7071f);
		pRow2.push_back(Vec4f(0.0f, 1.0f, 0.5f, 1.0f) * 1.0f);
		pRow2.push_back(Vec4f(0.5f, 1.0f, 0.5f, 1.0f) * 0.7071f);
		pRow2.push_back(Vec4f(0.5f, 0.5f, 0.5f, 1.0f) * 1.0f);
		pRow2.push_back(Vec4f(0.5f, 0.0f, 0.5f, 1.0f) * 0.7071f);
		pRow2.push_back(Vec4f(0.0f, 0.0f, 0.5f, 1.0f) * 1.0f);
		controlPoints.push_back(pRow2);


		std::vector<Vec4f> pRow1;
		pRow1.push_back(Vec4f(0.0f, 0.0f, 0.0f, 1.0f) * 1.0f);
		pRow1.push_back(Vec4f(-0.5f, 0.0f, 0.0f, 1.0f) * 0.7071f);
		pRow1.push_back(Vec4f(-0.5f, 0.5f, 0.0f, 1.0f) * 1.0f);
		pRow1.push_back(Vec4f(-0.5f, 1.0f, 0.0f, 1.0f) * 0.7071f);
		pRow1.push_back(Vec4f(0.0f, 1.0f, 0.0f, 1.0f) * 1.0f);
		pRow1.push_back(Vec4f(0.5f, 1.0f, 0.0f, 1.0f) * 0.7071f);
		pRow1.push_back(Vec4f(0.5f, 0.5f, 0.0f, 1.0f) * 1.0f);
		pRow1.push_back(Vec4f(0.5f, 0.0f, 0.0f, 1.0f) * 0.7071f);
		pRow1.push_back(Vec4f(0.0f, 0.0f, 0.0f, 1.0f) * 1.0f);
		controlPoints.push_back(pRow1);



		std::vector<Vec4f> pRow3;
		pRow3.push_back(Vec4f(0.0f, 0.0f, -0.5f, 1.0f) * 1.0f);
		pRow3.push_back(Vec4f(-0.5f, 0.0f, -0.5f, 1.0f) * 0.7071f);
		pRow3.push_back(Vec4f(-0.5f, 0.5f, -0.5f, 1.0f) * 1.0f);
		pRow3.push_back(Vec4f(-0.5f, 1.0f, -0.5f, 1.0f) * 0.7071f);
		pRow3.push_back(Vec4f(0.0f, 1.0f, -0.5f, 1.0f) * 1.0f);
		pRow3.push_back(Vec4f(0.5f, 1.0f, -0.5f, 1.0f) * 0.7071f);
		pRow3.push_back(Vec4f(0.5f, 0.5f, -0.5f, 1.0f) * 1.0f);
		pRow3.push_back(Vec4f(0.5f, 0.0f, -0.5f, 1.0f) * 0.7071f);
		pRow3.push_back(Vec4f(0.0f, 0.0f, -0.5f, 1.0f) * 1.0f);
		controlPoints.push_back(pRow3);


		std::vector<Vec4f> pRow4;
		pRow4.push_back(Vec4f(0.0f, 0.5f, -0.5f, 1.0f) * 1.0f);
		pRow4.push_back(Vec4f(-0.0f, 0.5f, -0.5f, 1.0f) * 0.7071f);
		pRow4.push_back(Vec4f(-0.0f, 0.5f, -0.5f, 1.0f) * 1.0f);
		pRow4.push_back(Vec4f(-0.0f, 0.5f, -0.5f, 1.0f) * 0.7071f);
		pRow4.push_back(Vec4f(0.0f, 0.5f, -0.5f, 1.0f) * 1.0f);
		pRow4.push_back(Vec4f(0.0f, 0.5f, -0.5f, 1.0f) * 0.7071f);
		pRow4.push_back(Vec4f(0.0f, 0.5f, -0.5f, 1.0f) * 1.0f);
		pRow4.push_back(Vec4f(0.0f, 0.5f, -0.5f, 1.0f) * 0.7071f);
		pRow4.push_back(Vec4f(0.0f, 0.5f, -0.5f, 1.0f) * 1.0f);
		controlPoints.push_back(pRow4);

		knotVectorU.push_back(0.0f);
		knotVectorU.push_back(0.0f);
		knotVectorU.push_back(0.0f);
		knotVectorU.push_back(0.25f);
		knotVectorU.push_back(0.25f);
		knotVectorU.push_back(0.5f);
		knotVectorU.push_back(0.5f);
		knotVectorU.push_back(0.75f);
		knotVectorU.push_back(0.75f);
		knotVectorU.push_back(1.0f);
		knotVectorU.push_back(1.0f);
		knotVectorU.push_back(1.0f);




		knotVectorV.push_back(0.0f);
		knotVectorV.push_back(0.0f);
		knotVectorV.push_back(0.0f);
		knotVectorV.push_back(0.5f);
		knotVectorV.push_back(0.5f);
		knotVectorV.push_back(1.0f);
		knotVectorV.push_back(1.0f);
		knotVectorV.push_back(1.0f);


		nurbs2 = NURBS_Surface(controlPoints, knotVectorU, knotVectorV, degree);
		NURBSs.push_back(nurbs2);
		resolutionU.push_back(0.005f);
		resolutionV.push_back(0.005f);
	
	}
	
	
	
}

void calculatePoints()
{
	// TODO: create two NURBS surfaces with different degrees k >= 2
	// calculate the points and their normals
	// emplace the resulting NURBS, points and normals into the vectors
	// =====================================================
	
	points.clear();
	normals.clear();
	numPointsU = 0;
	numPointsV = 0;
	NURBS_Surface nurbs = NURBSs.at(nurbsSelect);

	std::cout << "Calculating ";

	for (float u = 0; u <= 1.0f; u += resolutionU.at(nurbsSelect))
	{
		numPointsU++;
		numPointsV = 0;
		std::cout << ".";
		std::vector<Vec4f> temppoints;
		for (float v = 0; v <= 1.0f; v += resolutionV.at(nurbsSelect))
		{
			numPointsV++;
			Vec4f tangentU;
			Vec4f tangentV;
			points.push_back(nurbs.evaluteDeBoor(u, v, tangentU, tangentV));

			// the crossproduct
			Vec4f tu = tangentU.homogenized();
			Vec4f tv = tangentV.homogenized();

			Vec3f normal = Vec3f(tu.y * tv.z - tu.z * tv.y, tu.z * tv.x - tu.x * tv.z, tu.x * tv.y - tu.y * tv.x);
			normals.push_back(normal);
		}
	}
	std::cout << " Done !" << std::endl;
	// =====================================================
	
}

void reshape(GLint width, GLint height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.0, (float)width / (float)height, 0.1f, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// =================
// === RENDERING ===
// =================

void drawCS()
{
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	// red X
	glColor3f(1, 0, 0); 
	glVertex3f(0, 0, 0);
	glVertex3f(1, 0, 0);
	// green Y
	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1, 0);
	// blue Z
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1);
	glEnd();
}

void drawObjects()
{
	if (NURBSs.empty() || nurbsSelect >= NURBSs.size() || nurbsSelect < 0)
		return;

	NURBS_Surface nurbs = NURBSs.at(nurbsSelect);


	if(nurbs.controlPoints.size() > 1)
	{

		if(enableEval > 0)
			evaluateNURBSSurface(nurbs, u, v, enableEval == 1);
		if(enableCtrl)
			drawNURBSSurfaceCtrlP(nurbs);
		// TODO: draw nurbs surface
		// ========================
		if(enableNormals)
			drawNormals(points, normals);
		if (enableWireframe || enableSurf)
			drawNURBSSurface(points, normals, numPointsU, numPointsV, enableSurf, enableWireframe);

		// ========================
	}
}

void renderScene()
{
	// clear and set camera
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	// translate scene in viewing direction
	glTranslatef(transX, transY, transZ);  
	// rotate scene
	glRotatef(angleX, 0.0f, 1.0f, 0.0f);
	glRotatef(angleY, 1.0f, 0.0f, 0.0f);
	// draw coordinate system without lighting
	drawCS();
	drawObjects();
	// swap Buffers
	glFlush();
	glutSwapBuffers();
}

// =================
// === CALLBACKS ===
// =================

void keyPressed(unsigned char key, int x, int y)
{
	switch (key)
	{
		// esc => exit
	case 27:
		exit(0);
		break;
		// help file
	case 'h' :
	case 'H' :
		coutHelp();
		break;
		// reset view
	case 'r' :
	case 'R' :
		setDefaults();
		glutPostRedisplay();	// use this whenever 3d data changed to redraw the scene
		std::cout << "view reset\n";
		break;
	case 'c':
	case 'C':
		enableCtrl = !enableCtrl;
		glutPostRedisplay();
		std::cout << "Ctrl Polygon: "<< (enableCtrl ? "enabled" : "disabled")<<"\n";
		break;
	case 'e':
	case 'E':
		enableEval = (enableEval + 1) % 3;
		glutPostRedisplay();
		if (enableEval == 0)	std::cout << "Evaluation disabled\n";
		if (enableEval == 1)	std::cout << "Evaluation enabled ( v first)\n";
		if (enableEval == 2)	std::cout << "Evaluation enabled ( u first)\n";
		break;
	case 'a':
	case 'A':
		nurbsSelect = (nurbsSelect + 1) % NURBSs.size();
		calculatePoints();
		glutPostRedisplay();
		break;
		// TODO: place custom functions on button events here to present your results
		// ==========================================================================
	case 'n':
	case 'N':
		enableNormals = !enableNormals;
		glutPostRedisplay();
		std::cout << "Surface normals: " << (enableNormals ? "enabled" : "disabled") << "\n";
		break;
	case 'w':
	case 'W':
		enableWireframe = !enableWireframe;
		glutPostRedisplay();
		std::cout << "Surface wireframe: " << (enableWireframe ? "enabled" : "disabled") << "\n";
		break;
	case 's':
	case 'S':
		enableSurf = !enableSurf;
		glutPostRedisplay();
		std::cout << "Surface normals: " << (enableSurf ? "enabled" : "disabled") << "\n";
		break;
	case '8':
		u += 0.1f;
		if (u > 1) u = 1.0f;
		glutPostRedisplay();
		break;
	case '2':
		u -= 0.1f;
		if (u < 0) u = 0.0f;
		glutPostRedisplay();
		break;
	case '6':
		v += 0.1f;
		if (v > 1) v = 1.0f;
		glutPostRedisplay();
		break;
	case '4':
		v -= 0.1f;
		if (v < 0) v = 0.0f;
		glutPostRedisplay();
		break;


		// ==========================================================================
	}
}

void mousePressed(int button, int state, int x, int y)
{
	mouseButton = button;
	mouseX = x; 
	mouseY = y;
}

void mouseMoved(int x, int y)
{
	// rotate (cap angleY within [-85°, +85°])
	if (mouseButton == GLUT_LEFT_BUTTON)
	{
		angleX = fmod(angleX + (x-mouseX) * mouseSensitivy, 360.0f);
		angleY += (y-mouseY) * mouseSensitivy;
		if (angleY > 85) angleY = 85;
		if (angleY < -85) angleY = -85;
		glutPostRedisplay();
	}
	// zoom (here translation in z)
	if (mouseButton == GLUT_RIGHT_BUTTON)
	{
		transZ -= 0.2f * (y-mouseY) * mouseSensitivy;
		glutPostRedisplay();
	}
	// translation in xy
	if (mouseButton == GLUT_MIDDLE_BUTTON) 
	{
		transX += 0.2f * (x-mouseX) * mouseSensitivy;
		transY -= 0.2f * (y-mouseY) * mouseSensitivy;
		glutPostRedisplay();
	}
	// update mouse for next relative movement
	mouseX = x;
	mouseY = y;
}

// ===============
// === VARIOUS ===
// ===============

void coutHelp()
{
	std::cout << std::endl;
	std::cout << "====== KEY BINDINGS ======" << std::endl;
	std::cout << "ESC: exit" << std::endl;
	std::cout << "H: show this (H)elp file" << std::endl;
	std::cout << "R: (R)eset view" << std::endl;
	std::cout << "C: toggle surface (C)ontrol polygon" << std::endl;
	std::cout << "N: toggle surface (N)normals" << std::endl;
	std::cout << "W: toggle surface (W)ireframe" << std::endl;
	std::cout << "S: toggle surface (S)urf" << std::endl;
	std::cout << "E: switch (E)valuation visualization (none,u-first,v-fist)" << std::endl << "[ 8: u+ | 2: u- |  6: v+ | 4: v- ]" << std::endl;
	std::cout << "A: switch between NURBS surfaces" << std::endl;
	// TODO: update help text according to your changes
	// ================================================


	// ================================================
	std::cout << "==========================" << std::endl;
	std::cout << std::endl;
}