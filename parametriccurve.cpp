#include "parametriccurve.h"

ParametricCurve::ParametricCurve(QVector<Point> control_pts,int control_x,int control_y, float red, float green, float blue) {

    this->control_pts = QVector<Point>(control_pts);
    this->control_x = control_x;
    this->control_y = control_y;

    this->red = red;
    this->green = green;
    this->blue = blue;

    setControlPointColor();
    createControlSegment();
}


QVector<float> ParametricCurve::subVec(QVector<float> p1, QVector<float> p2) {
    QVector<float> res;
    res.push_back(p1.at(0) - p2.at(0));
    res.push_back(p1.at(1) - p2.at(1));
    res.push_back(p1.at(2) - p2.at(2));
    return res;
}

QVector<float> ParametricCurve::mulVec(QVector<float> p1, QVector<float> p2) {
    QVector<float> res;
    res.push_back(p1.at(1) * p2.at(2) - p1.at(2) * p2.at(1));
    res.push_back(p1.at(2) * p2.at(0) - p1.at(0) * p2.at(2));
    res.push_back(p1.at(0) * p2.at(1) - p1.at(1) * p2.at(0));
    return res;
}

float ParametricCurve::scalar(QVector<float> p1, QVector<float> p2) {
    return p1.at(0)*p2.at(0) + p1.at(1)*p2.at(1) + p1.at(2)*p2.at(2);
}

float ParametricCurve::getAngle(float a) {
    QVector<float> p1 = bezier(a);
    QVector<float> p2 = bezier(a+1/precision);
    QVector<float> inc_rate = increaseRate(a);
    QVector<float> pointT = subVec(p2, p1);
    return acos(fabs(scalar(inc_rate, pointT)));
}

QVector<float> ParametricCurve::increaseRate(float a) {
    double h = 1/1000000.0;
    QVector<float> derivation_pt;
    QVector<float> p1;
    QVector<float> p2;
    p1 = bezier(a+h);
    p2 = bezier(a);

    for(int j = 0; j<3;j++) {
        derivation_pt.push_back((p1.at(j)-p2.at(j))/h);
    }

    return derivation_pt;
}

QVector<float> ParametricCurve::bezier(float a) {
    QVector<float> new_pt;
    float t = a/precision;
    new_pt.push_back(control_pts[0].getX()*pow(1-t, 3)+3 * control_pts[1].getX()*t*pow(1-t,2)+ 3*control_pts[2].getX()*pow(t,2)*(1-t)+control_pts[3].getX()*pow(t,3));
    new_pt.push_back(control_pts[0].getY()*pow(1-t, 3)+3 * control_pts[1].getY()*t*pow(1-t,2)+ 3*control_pts[2].getY()*pow(t,2)*(1-t)+control_pts[3].getY()*pow(t,3));
    new_pt.push_back(control_pts[0].getZ()*pow(1-t, 3)+3 * control_pts[1].getZ()*t*pow(1-t,2)+ 3*control_pts[2].getZ()*pow(t,2)*(1-t)+control_pts[3].getZ()*pow(t,3));
    return new_pt;
}


int ParametricCurve::factorial(int f) {
    return (f == 1 || f == 0) ? 1 : factorial(f - 1) * f;
}


float ParametricCurve::bersteinPolynomial(float u, float i, float n) {
    float firstPart = factorial(n)/(factorial(i) * factorial(n-i));
    float secondPart = pow(u,i) *pow(1-u,n-i);
    return firstPart*secondPart;
}

Point* ParametricCurve::bezierArea(float u, float v, int n, int m) {
    Point *p= new Point(0, 0, 0, this->red, this->green, this->blue);

    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            *p += (bersteinPolynomial(u, i, n-1) * bersteinPolynomial(v, j, m-1)) * control_pts[n*j+i];
        }
    }
    return p;
}
