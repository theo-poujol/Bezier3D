#include "parametriccurve.h"
#include "math.h"
#include <cmath>
#include <QVector3D>

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

void ParametricCurve::createPointList() {
    pointList.clear();
    for(int i= 0; i <= precision; i++){
        for(int j = 0; j <= precision; j++){
            //qDebug() << "début des surfaces i : " << i << " j : " << j;
            Point *tmp = bezierArea(i/precision, j/precision, control_x, control_y);
            pointList.push_back(*tmp);
        }
    }
}

void ParametricCurve::setStart(int start) {
    this->start = start;
}


int ParametricCurve::getStart() {
    return this->start;
}

void ParametricCurve::setPrecision(int pas) {
    precision=pas;
    update();
}

int ParametricCurve::getSize() {
    return (nb_segment+control_seg.size())*2;
}

int ParametricCurve::getSizeCurveParam() {
    return control_seg.size()*2;
}

Point ParametricCurve::getPoint(int numPoint) {
    return control_pts[numPoint];
}

void ParametricCurve::setPoint(int numPoint,Point p) {
    control_pts[numPoint]=p;
    update();
}


void ParametricCurve::setControlPointColor() {

    for (int i=0;i<control_pts.size();++i) {
        control_pts[i].setRgb(1.0, 0.0, 0.0);
    }
}


void ParametricCurve::swapGridSurface(bool showGrid) {
    this->showGrid=showGrid;
    update();
}

void ParametricCurve::createControlSegment() {
    control_seg.clear();
    for (int j=0;j<control_y;++j) {
        for(int i=0;i<control_x-1;++i){
            control_seg.push_back(*new Segment(control_pts[control_x*j+i],control_pts[control_x*j+i+1]));
        }
    }
    for (int j=0;j<control_x;++j) {
        for(int i=0;i<control_y-1;++i){
            control_seg.push_back(*new Segment(control_pts[i*control_x+j],control_pts[(i+1)*control_x+j]));
        }
    }
}

void ParametricCurve::update() {
    setControlPointColor();
    createControlSegment();
    isNeedCompute=true;
}


void ParametricCurve::createCurve(QVector<GLfloat> *vertData) {
    for(int i = 0; i < control_seg.size(); i++){
        control_seg[i].createSegment(vertData);
    }
    if(isNeedCompute){
        isNeedCompute=false;
        createPointList();

        if(showGrid){
            segmentList.clear();
            for(int i= 0; i <= precision; i++){
                for(int j = 0; j < precision; j++){
                    Segment *tmp = new Segment(pointList[i*(precision+1)+j], pointList[i*(precision+1)+j+1]);
                    segmentList.push_back(*tmp);
                }
            }
            for(int i= 0; i <= precision; i++){
                for(int j = 0; j < precision; j++){
                    Segment *tmp = new Segment(pointList[i+j*(precision+1)], pointList[i+(j+1)*(precision+1)]);
                    segmentList.push_back(*tmp);
                }
            }
            nb_segment = 2*precision*(precision+1);
        }
    }
    if(showGrid){
        for(int i = 0; i < nb_segment; i++){
            segmentList[i].createSegment(vertData);
        }
    }
    else{
        for(int i= 0; i < precision; i++){
            for(int j = 0; j < precision; j++){
                int A=(i+0)*(precision+1)+(j+0);
                int B=(i+0)*(precision+1)+(j+1);
                int C=(i+1)*(precision+1)+(j+0);
                int D=(i+1)*(precision+1)+(j+1);

                QVector3D vAB(pointList[B].getX()-pointList[A].getX(),pointList[B].getY()-pointList[A].getY(),pointList[B].getZ()-pointList[A].getZ());
                QVector3D vAC(pointList[C].getX()-pointList[A].getX(),pointList[C].getY()-pointList[A].getY(),pointList[C].getZ()-pointList[A].getZ());
                QVector3D vDB(pointList[B].getX()-pointList[D].getX(),pointList[B].getY()-pointList[D].getY(),pointList[B].getZ()-pointList[D].getZ());
                QVector3D vDC(pointList[C].getX()-pointList[D].getX(),pointList[C].getY()-pointList[D].getY(),pointList[C].getZ()-pointList[D].getZ());

                QVector3D nABC = -QVector3D::normal(vAC, vAB);
                QVector3D nBDC = -QVector3D::normal(vDB, vDC);
                QVector3D nBC = (nABC+nBDC)/2;

                pointList[A].createPoint(vertData);
                vertData->append(nABC.x());vertData->append(nABC.y());vertData->append(nABC.z());

                pointList[B].createPoint(vertData);
                vertData->append(nBC.x());vertData->append(nBC.y());vertData->append(nBC.z());

                pointList[C].createPoint(vertData);
                vertData->append(nBC.x());vertData->append(nBC.y());vertData->append(nBC.z());

                pointList[B].createPoint(vertData);
                vertData->append(nBC.x());vertData->append(nBC.y());vertData->append(nBC.z());

                pointList[C].createPoint(vertData);
                vertData->append(nBC.x());vertData->append(nBC.y());vertData->append(nBC.z());

                pointList[D].createPoint(vertData);
                vertData->append(nBDC.x());vertData->append(nBDC.y());vertData->append(nBDC.z());
            }
        }
        nb_segment=3*precision*precision;
    }
}





















