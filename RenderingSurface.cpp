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
	glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_LINES);
	for (size_t i = 0; i < points.size(); i++)
	{
		Vec4f p = points.at(i).homogenized();
		glVertex3f(p.x, p.y, p.z);
		Vec3f n = normals.at(i).normalized();
		glVertex3f(p.x + n.x, p.y + n.y, p.z + n.z);
	}
	glEnd();
	// =====================================================
}
void drawNURBSSurfaceCtrlP(const NURBS_Surface &surface)
{
	// TODO: draw control polygon an points (homogenized)
	// =====================================================
	glColor3f(0.9f, 0.01f, 0.99f);

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
	for (size_t i = 0; i < size_u; i++)
	{
		glBegin(GL_LINE_STRIP);
		for (size_t j = 0; j < size_v; j++)
		{
			Vec4f p = surface.controlPoints.at(i).at(j).homogenized();
			glVertex3f(p.x, p.y, p.z);
		}
		glEnd();
	}
	

	// =====================================================
}

void drawNURBSSurface(std::vector<Vec4f> &points, const std::vector<Vec3f> &normals, const size_t numPointsU, const size_t numPointsV, bool enableSurf, bool enableWire)
{

	if (enableWire)
	{
		glDisable(GL_LIGHTING);
		glColor3f(0.0f,0.0f,1.0f);
		// TODO: draw surface wire mesh
		// =====================================================
		
		for (size_t i = 0; i < numPointsU; i++)
		{
			glBegin(GL_LINE_STRIP);
			for (size_t j = 0; j < numPointsV; j++)
			{
				Vec4f p = points.at(i * numPointsV + j).homogenized();
				glVertex3f(p.x, p.y, p.z);
			}
			glEnd();
		}
		for (size_t i = 0; i < numPointsV; i++)
		{
			glBegin(GL_LINE_STRIP);
			for (size_t j = 0; j < numPointsU; j++)
			{
				Vec4f p = points.at(j * numPointsV + i).homogenized();
				glVertex3f(p.x, p.y, p.z);
			}
			glEnd();
		}

		// =====================================================

	}

	if (enableSurf)
	{
		glEnable(GL_LIGHTING);
		glColor3f(0.99f, 0.99f, 0.99f);
		// TODO: draw surface with quads
		// =====================================================

		for (size_t i = 0; i < numPointsU - 1; i++)
		{
			glBegin(GL_TRIANGLES);
			for (size_t j = 0; j < numPointsV - 1; j++)
			{
				size_t n1 = i * numPointsV + j;
				size_t n2 = (i + 1)* numPointsV + j;
				size_t n3 = i * numPointsV + (j + 1);
				size_t n4 = (i + 1) * numPointsV + (j + 1);
				Vec4f p1 = points.at(n1).homogenized();
				Vec4f p2 = points.at(n2).homogenized();
				Vec4f p3 = points.at(n3).homogenized();
				Vec4f p4 = points.at(n4).homogenized();

				Vec3f n = normals.at(n1);
				n = n + normals.at(n2);
				n = n + normals.at(n3);
				n.normalize();

				glNormal3f(n.x, n.y, n.z);
				glVertex3f(p1.x, p1.y, p1.z);
				glVertex3f(p2.x, p2.y, p2.z);
				glVertex3f(p3.x, p3.y, p3.z);

				n = normals.at(n2);
				n = n + normals.at(n3);
				n = n + normals.at(n4);
				n.normalize();

				glVertex3f(p2.x, p2.y, p2.z);
				glVertex3f(p3.x, p3.y, p3.z);
				glVertex3f(p4.x, p4.y, p4.z);
			}

			glEnd();
		}
		
		
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

	const size_t size_u = surface.controlPoints.size();
	const size_t size_v = surface.controlPoints.at(0).size();

	if (vFirst)
	{
		// 1. draw the nurbs curves and its control polygons first in v direction
		std::vector<Vec4f> points_v;
		for (size_t i = 0; i < size_v; i++)
		{
			std::vector<Vec4f> points_u;
			for (size_t j = 0; j < size_u; j++)
			{
				points_u.push_back(surface.controlPoints.at(j).at(i));
			}

			NURBSCurve curve = NURBSCurve(points_u, surface.knotVectorV, surface.degree);


			drawNURBSCtrlPolygon_H(curve, colorPolyV);

			drawNURBS_H(curve, colorCurveV);

			points_v.push_back(curve.evaluteDeBoor(v, Vec4f()));
		}

		// 2. then the resulting curve and its control polygon at v in u direction.
		   
		NURBSCurve curve = NURBSCurve(points_v, surface.knotVectorU, surface.degree);

		//renderNURBSEvaluation(curve, v);

		drawNURBSCtrlPolygon_H(curve, colorPolyU);

		drawNURBS_H(curve, colorCurveU);


		// 3. draw the evaluated surface point
	
		glColor3fv(&colorPoint.x);
		glBegin(GL_POINTS);
		{
			Vec4f p = curve.evaluteDeBoor(u, Vec4f()).homogenized();
			glVertex3f(p.x, p.y, p.z);
		}
		glEnd();

	}
	else
	{
		// 1. draw the nurbs curves and its control polygons first in u direction

		std::vector<Vec4f> points_u;
		for (size_t i = 0; i < size_u; i++)
		{
			NURBSCurve curve = NURBSCurve(surface.controlPoints.at(i), surface.knotVectorU, surface.degree);

			//renderNURBSEvaluation(curve, u);
			

			drawNURBSCtrlPolygon_H(curve, colorPolyU);

			drawNURBS_H(curve, colorCurveU);

			points_u.push_back(curve.evaluteDeBoor(u, Vec4f()));
		}


		// 2. then the resulting curve and its control polygon at u in v direction.

		NURBSCurve curve = NURBSCurve(points_u, surface.knotVectorV, surface.degree);

		//renderNURBSEvaluation(curve, v);

		drawNURBSCtrlPolygon_H(curve, colorPolyV);

		drawNURBS_H(curve, colorCurveV);


		// 3. draw the evaluated surface point
		

		glColor3fv(&colorPoint.x);
		glBegin(GL_POINTS);
		{
			Vec4f p = curve.evaluteDeBoor(v, Vec4f()).homogenized();
			glVertex3f(p.x, p.y, p.z);
		}
		glEnd();
	}

	// =====================================================

	
}
