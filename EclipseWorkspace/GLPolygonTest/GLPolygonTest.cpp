/*
 *  GLPolygonTest.cpp
 *
 *  Created on: 2017-04-28
 *  Author: Igor Bender
 *
 */
#include <stdlib.h>
#include <GL/glut.h>

#include <GpcPolygon.h>
//
//
//
extern "C" void renderScene(void);
extern "C" void changeSize(int w, int h);

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitWindowPosition(10, 10);
    glutInitWindowSize(400, 400);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutCreateWindow(argv[0]);
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutMainLoop();

    return 0;
}

void renderScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glOrtho(-1.5, 1.5, -1.5, 1.5, -1.5, 1.5);

    double Vert1[8];
    Vert1[0] = -1.0;
    Vert1[1] = -1.0;
    Vert1[2] = 1.0;
    Vert1[3] = -1.0;
    Vert1[4] = 1.0;
    Vert1[5] = 1.0;
    Vert1[6] = -1.0;
    Vert1[7] = 1.0;
    GpcPolygon Poly1(4, Vert1);
    Vert1[0] = 0.3;
    Vert1[1] = 0.3;
    Vert1[2] = 1.25;
    Vert1[3] = 0.3;
    Vert1[4] = 1.25;
    Vert1[5] = 1.25;
    Vert1[6] = 0.3;
    Vert1[7] = 1.25;

    Poly1 += GpcPolygon(4, Vert1);

    double Vert2[8];
    Vert2[0] = -0.5;
    Vert2[1] = -0.5;
    Vert2[2] = 0.5;
    Vert2[3] = -0.5;
    Vert2[4] = 0.5;
    Vert2[5] = 0.5;
    Vert2[6] = -0.5;
    Vert2[7] = 0.5;
    GpcPolygon Poly2(4, Vert2);

    Poly1 ^= (Poly1 & Poly2);

//    gpc_tristrip Triangles;
//    gpc_polygon_to_tristrip(Poly1.getPolyPtr(), &Triangles);

    GpcTrianglesStrips Triangles(Poly1);

//    for (int s= 0; s < Triangles.num_strips; s++)
    for (int s = 0;  s < Triangles.getNumOfStrips();  s++)
    {
        glBegin(GL_TRIANGLE_STRIP);
//        for(int v= 0; v < Triangles.strip[s].num_vertices; v++)
//            glVertex2d(Triangles.strip[s].vertex[v].x, Triangles.strip[s].vertex[v].y);
        for(int v = 0;  v < Triangles.getNumOfStripVerteces(s);  v++)
        {
            gpc_vertex TmpVertex;
            Triangles.getVertex(s, v, TmpVertex);
            glVertex2d(TmpVertex.x, TmpVertex.y);
        }
        glEnd();
    }
//    glBegin(GL_POLYGON);
//        glVertex2d(-0.5, -0.5);
//        glVertex2d(-0.5, 0.5);
//        glVertex2d(0.5, 0.5);
//        glVertex2d(0.5, -0.5);
//       //glRectd(-0.5, -0.5, 0.5, 0.5);
//    glEnd();

//    gpc_free_tristrip(&Triangles);

    glFlush();
}

void changeSize(int w, int h)
{
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
//    if(h == 0)
//        h = 1;
//    float ratio = 1.0 * w / h;

    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);
//    glClear(GL_COLOR_BUFFER_BIT);
//    glOrtho(-1.5, 1.5, -1.5, 1.5, -1.5, 1.5);
//    glBegin(GL_POLYGON);
//        glVertex2d(-0.5, -0.5);
//        glVertex2d(-0.5, 0.5);
//        glVertex2d(0.5, 0.5);
//        glVertex2d(0.5, -0.5);
//       //glRectd(-0.5, -0.5, 0.5, 0.5);
//    glEnd();
//    glFlush();

    // Set the correct perspective.
//    gluPerspective(45,ratio,1,1000);
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
    // gluLookAt(0.0, 0.0, 5.0,
              // 0.0, 0.0, -1.0,
              // 0.0f, 1.0f, 0.0f);

}
