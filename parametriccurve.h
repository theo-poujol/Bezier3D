#ifndef PARAMETRICCURVE_H
#define PARAMETRICCURVE_H

#include "point.h"
#include "segment.h"
#include <QVector>

class ParametricCurve
{
public:

    /* CONSTRUCTEUR */

    ParametricCurve(QVector<Point> control_pts,int control_x,int control_y, float red, float green, float blue);

    /** FONCTIONS DES COURBES **/
    void createCurve(QVector<GLfloat> *vertData);
    void createControlSegment();
    void swapGridSurface(bool showGrid);
    void update();
    void createPointList();

    /** FONCTIONS VECTEURS **/

    QVector<float> subVec(QVector<float> p1, QVector<float> p2);
    QVector<float> mulVec(QVector<float> p1, QVector<float> p2);
    float scalar(QVector<float>  p1, QVector<float>  p2);

    float getAngle(float a);
    QVector<float> increaseRate(float a);
    QVector<float> bezier(float a);
    int factorial(int f);
    float bersteinPolynomial(float u, float i, float n);
    Point* bezierArea(float u, float v, int n, int m);

    /** SETTERS **/
    void setControlPointColor();
    void setStart(int start);
    void setPrecision(int pas);
    void setPoint(int numPoint,Point p);

    /** GETTERS **/
    Point getPoint(int numPoint);
    int getStart(), getSize(), getSizeCurveParam();


private:
    QVector<Point> control_pts, pointList;
    QVector<Segment> control_seg, segmentList;
    int control_x, control_y, nb_segment, sizeCurveParam, start;
    float red, green, blue, precision = 10;
    bool isNeedCompute = true, showGrid = true;
};

#endif // PARAMETRICCURVE_H
