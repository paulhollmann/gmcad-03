#define RADPERDEG 0.0174533

#include "RenderingSurface.h"
#include "RenderingCurve.h"

#include <GL/glut.h>
#include <NURBS_Curve.h>
#include <NURBS_Surface.h>
#include <algorithm>

void drawSurfacePoints(const std::vector<Vec4f> &points)
{
	// TODO: draw points of the surface
	// note: Vec4f provides a method to homogenize a vector
	// =====================================================
	

	// =====================================================
}
void drawNormals(const std::vector<Vec4f> &points, const std::vector<Vec3f> &normals)
{
	// TODO: draw normals as lines (homogenized)
	// note: Vec4f provides a method to homogenize a vector
	// =====================================================
	

	// =====================================================
}
void drawNURBSSurfaceCtrlP(const NURBS_Surface &surface)
{
	// TODO: draw control polygon an points (homogenized)
	// =====================================================
	glColor3f(0.5f, 0.5f, 0.5f);
	const size_t size_u = surface.controlPoints.size();
	const size_t size_v = surface.controlPoints.at(0).size();
	for (size_t i = 0; i < size_v; i++)
	{
		glBegin(GL_LINE_STRIP);
		for (size_t j = 0; j < size_u; j++)
		{
			Vec4f p = surface.controlPoints.at(j).at(i).homogenized();
			glVertex3f(p.x, p.y, p.z);
		}
		glEnd();
	}
	for (size_t i = 0; i < size_v; i++)
	{
		glBegin(GL_LINE_STRIP);
		for (size_t j = 0; j < size_u; j++)
		{
			Vec4f p = surface.controlPoints.at(i).at(j).homogenized();
			glVertex3f(p.x, p.y, p.z);
		}
		glEnd();
	}
	

	// =====================================================
}

void drawNURBSSurface(std::vector<Vec4f> &points, const std::vector<Vec3f> &normals, const int numPointsU, const int numPointsV, bool enableSurf, bool enableWire)
{

	if (enableWire)
	{
		glDisable(GL_LIGHTING);
		glColor3f(0.0f,0.0f,1.0f);
		// TODO: draw surface wire mesh
		// =====================================================
		

		// =====================================================

	}

	if (enableSurf)
	{
		glEnable(GL_LIGHTING);
		glColor3f(0.99f, 0.99f, 0.99f);
		// TODO: draw surface with quads
		// =====================================================
		
		
		// =====================================================
	}
}
void evaluateNURBSSurface(const NURBS_Surface &surface,float u, float v, bool vFirst /*= true*/)
{
	Vec3f colorPolyU =  {1.0f, 0.7f, 0.4f};
	Vec3f colorPolyV =  {0.6f, 1.0f, 0.6f};
	Vec3f colorCurveU = {1.0f, 0.5f, 0.0f};
	Vec3f colorCurveV = {0.0f, 0.8f, 0.0f};
	Vec3f colorPoint =  {0.0f, 0.5f, 1.0f};
	// TODO: implement the visualization of the point evaluation starting with u and v direction whether vFirst is true or not
	// note: use the NURBSCurve class and the CurveRendering functions 'drawNURBSCtrlPolygon_H' 'drawNURBS_H'
	// =====================================================

	if (vFirst)
	{
		// 1. draw the nurbs curves and its control polygons first in v direction
		   
		// 2. then the resulting curve and its control polygon at v in u direction.
		   
		// 3. draw the evaluated surface point
		
	}
	else
	{
		// 1. draw the nurbs curves and its control polygons first in u direction

		// 2. then the resulting curve and its control polygon at u in v direction.

		// 3. draw the evaluated surface point
		
	}

	// =====================================================

	
}
