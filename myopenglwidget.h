#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H


#include "point.h"
#include "parametriccurve.h"

#include <QObject>
#include <QWidget>
#include <QVector>
#include <QKeyEvent>
#include <QTimer>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class myOpenGLWidget : public QOpenGLWidget,
			   protected QOpenGLFunctions
{
	Q_OBJECT

public:
	explicit myOpenGLWidget(QWidget *parent = nullptr);
	~myOpenGLWidget();
    QVector<Point> getControls_pts();
    int getControl_x(), getControl_y();

public slots:

signals:  // On ne les implémente pas, elles seront générées par MOC ;
		  // les paramètres seront passés aux slots connectés.

protected slots:
	void onTimeout();

protected:
	void initializeGL() override;
	void doProjection();
	void resizeGL(int w, int h) override;
	void paintGL() override;
	void keyPressEvent(QKeyEvent *ev) override;
	void keyReleaseEvent(QKeyEvent *ev) override;
	void mousePressEvent(QMouseEvent *ev) override;
	void mouseReleaseEvent(QMouseEvent *ev) override;
	void mouseMoveEvent(QMouseEvent *ev) override;

private:
	double m_angle = 0;
	QTimer *m_timer = nullptr;
	double m_ratio = 1;


	//RR matrices utiles
	QMatrix4x4 m_modelView;
	QMatrix4x4 m_projection;
	QMatrix4x4 m_model;

	QOpenGLShaderProgram *m_program;
	QOpenGLBuffer m_vbo;

    Point *E,*F,*G;
    QVector<Point> control_pts;
    int control_x, control_y, nb_points;
    ParametricCurve *curve;
    bool    isEditing       = false,
            isDrawing       = true,
            showInterval    = false,
            showGrid        = false,
            showControlsPts = true;

    double m_angleX = 0, m_angleY = 0, m_angleZ = 0;
    double m_x=0, m_y=0, m_z=0;
    double dx=0,dy=0,dz=0;
    double u=0,v=0;

	void makeGLObjects();
	void tearGLObjects();

private slots:
    void setPasHomogene(int value);
    void setU(double value);
    void setV(double value);
    void showIntervalParametrique(bool show);

    void translateForward();
    void translateLeft();
    void translateRight();
    void translateBackward();

    void rotateForward();
    void rotateLeft();
    void rotateRight();
    void rotateBackward();

    void reset();
    void editMode();

    void previousPoint();
    void nextPoint();
    void previousPoint_x();
    void nextPoint_x();
    void previousPoint_y();
    void nextPoint_y();
    void applyPointChange();

    void toggleControlPolygon();
    void toggleSurface();
};


#endif // MYOPENGLWIDGET_H
