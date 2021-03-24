#include <cmath>

#include "segment.h"

Segment::Segment()
{
	pointList = new Point[2];

}

Segment::~Segment()
{
	delete [] pointList;
	pointList = nullptr;
}

Segment::Segment(const Segment & s)
{
	pointList = new Point[2];

	for (unsigned i=0; i<2; ++i)
		pointList[i] = s.pointList[i];

}

Segment::Segment(Point p1, Point p2) {
    pointList = new Point[2];
    pointList[0]=p1;
    pointList[1]=p2;
}

void Segment::setStart(const Point & p)
{
	pointList[0] = p;
}

void Segment::setEnd(const Point & p)
{
	pointList[1] = p;
}

void Segment::setN(unsigned r, const Point & p)
{
	if (r>1)
		r=1;
	pointList[r] = p;
}

Point Segment::getN(unsigned r) const
{
	if (r>1)
		r=1;

	return pointList[r];
}

Point Segment::getStart() const
{
	return getN(0);
}


Point Segment::getEnd() const
{
	return getN(1);
}

float Segment::length() const
{
	float res=0.0f;

	for (unsigned i=0; i<3; ++i)
		res += pow((pointList[1]).getN(i) - (pointList[0]).getN(i), 2.0f);

	return sqrt(res);
}

void Segment::createSegment(QVector<GLfloat> *vertData)
{
    GLfloat vertices[6];
    GLfloat rgb[6];

    //VStart
    vertices[0] = getStart().getX();
    vertices[1] = getStart().getY();
    vertices[2] = getStart().getZ();

    //VEnd
    vertices[3] = getEnd().getX();
    vertices[4] = getEnd().getY();
    vertices[5] = getEnd().getZ();

    //CStart
    rgb[0] = getStart().getRgb()[0];
    rgb[1] = getStart().getRgb()[1];
    rgb[2] = getStart().getRgb()[2];

    //CEnd
    rgb[3] = getEnd().getRgb()[0];
    rgb[4] = getEnd().getRgb()[1];
    rgb[5] = getEnd().getRgb()[2];

    for (int i = 0; i < 2; ++i) { //2 sommets
        // coordonnées sommets
        for (int j = 0; j < 3; j++) //3 coords par sommet
            vertData->append(vertices[i*3+j]);
        // couleurs sommets
        for (int j = 0; j < 3; j++) //1 RGB par sommet
            vertData->append(rgb[i*3+j]*10);

        for (int j = 0; j < 3; j++) //1 RGB par sommet
            vertData->append(0);
    }
}


Segment& Segment::operator= (const Segment &s)
{
	for (unsigned i=0; i<2; ++i)
		pointList[i] = s.pointList[i];

return *this;
}


