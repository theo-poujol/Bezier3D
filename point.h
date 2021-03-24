/* R. Raffin
 * M1 Informatique, Aix-Marseille Université
 * définition d'une classe Point
 * l'utilisation du tableau de float pour les coordonnées est là pour ne pas trop avoir peur de C++
 * mais c'(est une mauvaise façon d'écrire, préférez les objets de std::, comme std::vector par exemple
 * Cet objet est d'ailleurs directement utilsable comme un float[]
 * Si on veut généraliser le Point, il peut être d'une quelconque dimension (i.e. un nombre quelconque de coordonnées)
 */

#ifndef POINT_H
#define POINT_H

#include <iostream>
#include <QOpenGLWidget>

class Point
{
private:
	float * coords;
    float * rgb;

public:
	Point();
	~Point();
	Point(const Point&);
    Point(float x, float y, float z, float red, float green, float blue);

    /* Opérateurs de calculs entre deux points */
	Point& operator= (const Point &);
    Point& operator+= (const Point &p);

	float getN(unsigned) const;
	float getX() const;
	float getY() const;
	float getZ() const ;
    float * getCoords();
    float * getRgb();
	void get(float *) const;

	void setN(unsigned, const float&);
	void setX(const float&);
	void setY(const float&);
	void setZ(const float&);
	void set(const float *);
    void setColor(const float * t);
    void setColor(float red, float green, float blue);
    void makeObject(QVector<GLfloat> *vertex_data);
    void makeObjectSingle(QVector<GLfloat> *vertex_data);

};

#endif // POINT_H
