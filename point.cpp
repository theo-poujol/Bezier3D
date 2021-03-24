#include "point.h"

Point::Point()
{
    this->coords = new float[3];
    this->rgb = new float[3];
}

Point::~Point()
{
    delete [] this->coords;
    this->coords = nullptr;
    delete [] this->rgb;
    this->rgb = nullptr;
}

Point::Point(const Point& p)
{
	coords = new float[3];
    this->rgb = new float[3];
    for (unsigned i=0; i<3; ++i) {
        coords[i] = p.coords[i];
        this->rgb[i] = p.rgb[i];
    }

}

Point::Point(float x, float y, float z, float red, float green, float blue) {
    this->coords = new float[3];
    this->rgb = new float[3];

    this->coords[0] = x;
    this->coords[1] = y;
    this->coords[2] = z;

    this->rgb[0] = red;
    this->rgb[1] = green;
    this->rgb[2] = blue;
}

float Point::getX() const
{
	return getN(0);
}

float Point::getY() const
{
	return getN(1);
}

float Point::getZ() const
{
	return getN(2);
}

float * Point::getCoords() {
    return this->coords;
}

float * Point::getRgb() {
    return this->rgb;
}

void Point::get(float * t) const
{
	for (unsigned i=0; i<3; ++i)
		t[i] = coords[i];
}

float Point::getN(unsigned r) const
{
	if (r>2)
		r=2;
	return coords[r] ;
}

void Point::setN(unsigned r, const float & v)
{
	if (r>2)
		r=2;
	coords[r] = v;
}

void Point::set(const float * t)
{
	for (unsigned i=0; i<3; ++i)
		coords[i] = t[i];
}

void Point::setRgb(const float *rgb) {
    for(int i = 0; i < 3; i++) {
        this->rgb[i] = rgb[i];
    }
}

void Point::setRgb(float red, float green, float blue) {
    this->rgb[0] = red;
    this->rgb[1] = green;
    this->rgb[2] = blue;
}

void Point::createPoint(QVector<GLfloat> *vertex_data){
    vertex_data->append(getX());
    vertex_data->append(getY());
    vertex_data->append(getZ());

    vertex_data->append(this->rgb[0]);
    vertex_data->append(this->rgb[1]);
    vertex_data->append(this->rgb[2]);
}

void Point::createPointSingle(QVector<GLfloat> *vertex_data){
    vertex_data->append(getX());
    vertex_data->append(getY());
    vertex_data->append(getZ());

    vertex_data->append(this->rgb[0]*10);
    vertex_data->append(this->rgb[1]*10);
    vertex_data->append(this->rgb[2]*10);

    for(int i = 0; i < 3; i++)
        vertex_data->append(0);
}

void Point::setX(const float& v)
{
	setN(0, v);
}
void Point::setY(const float& v)
{
	setN(1, v);
}
void Point::setZ(const float& v)
{
	setN(2, v);
}

Point& Point::operator= (const Point &p)
{
	for (unsigned i=0; i<3; ++i)
		coords[i] = p.coords[i];

    return *this;
}

Point& Point::operator+= (const Point &p) {
    for (unsigned i=0; i<3; ++i) {
        coords[i] += p.coords[i];
    }
    return *this;
}

Point operator+ (const Point &p1, const Point &p2) {
    Point sum_p;
    for (unsigned i=0; i<3; ++i){
        sum_p.coords[i] = p1.coords[i] + p2.coords[i];
        sum_p.rgb[i] = ( p1.rgb[i] + p2.coords[i] )/2;
    }
    return sum_p;
}

Point operator* (const float &f, const Point &p) {
    Point mul_p;
    for (unsigned i=0; i<3; ++i){
        mul_p.coords[i] = f*  p.coords[i];
    }
    return mul_p;
}

