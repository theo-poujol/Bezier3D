#ifndef PARAMETRICCURVE_H
#define PARAMETRICCURVE_H

#include "point.h"
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

    /** FONCTIONS VECTEURS **/

    QVector<float> subVec(QVector<float> p1, QVector<float> p2);
    QVector<float> mulVec(QVector<float> p1, QVector<float> p2);
    float scalar(std::vector<float> p1, std::vector<float> p2);

    /** SETTERS **/
    void setControlPointColor();
    void setStart(int start);
    void setPrecision(int pas);
    void setPoint(int numPoint,Point p);

    /** GETTERS **/
    Point getPoint(int numPoint);
    int getStart(), getSize(), getSizeCurveParam();
};

#endif // PARAMETRICCURVE_H
