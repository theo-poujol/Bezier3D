//R. Raffin, M1 Informatique, "Surfaces 3D"
//tiré de CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#include "myopenglwidget.h"
#include <QDebug>
#include <QSurfaceFormat>
#include <QMatrix4x4>

#include <iostream>

#include "segment.h"

static const QString vertexShaderFile   = "basic.vsh";
static const QString fragmentShaderFile = "basic.fsh";


myOpenGLWidget::myOpenGLWidget(QWidget *parent) :
	QOpenGLWidget(parent)
{
	qDebug() << "init myOpenGLWidget" ;

	QSurfaceFormat sf;
	sf.setDepthBufferSize(24);
	sf.setSamples(16);  // nb de sample par pixels : suréchantillonnage por l'antialiasing, en décalant à chaque fois le sommet
						// cf https://www.khronos.org/opengl/wiki/Multisampling et https://stackoverflow.com/a/14474260
	setFormat(sf);

	setEnabled(true);  // événements clavier et souris
	setFocusPolicy(Qt::StrongFocus); // accepte focus
	setFocus();                      // donne le focus

	m_timer = new QTimer(this);
	m_timer->setInterval(50);  // msec
	connect (m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

myOpenGLWidget::~myOpenGLWidget()
{
	qDebug() << "destroy GLArea";

	delete m_timer;

	// Contrairement aux méthodes virtuelles initializeGL, resizeGL et repaintGL,
	// dans le destructeur le contexte GL n'est pas automatiquement rendu courant.
	makeCurrent();
	tearGLObjects();
	doneCurrent();
}


void myOpenGLWidget::initializeGL()
{
	qDebug() << __FUNCTION__ ;
	initializeOpenGLFunctions();
	glEnable(GL_DEPTH_TEST);

	makeGLObjects();

	//shaders
	m_program = new QOpenGLShaderProgram(this);
	m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderFile);  // compile
	m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderFile);

	if (! m_program->link()) {  // édition de lien des shaders dans le shader program
		qWarning("Failed to compile and link shader program:");
		qWarning() << m_program->log();
	}
}

void myOpenGLWidget::doProjection()
{
	//m_mod.setToIdentity();
	//modelMatrix.ortho( -aratio, aratio, -1.0f, 1.0f, -1.0f, 1.0f );
}


void myOpenGLWidget::makeGLObjects()
{

    if (isDrawing) {                  //   x     y     z    r    g    b
        control_pts.push_back(*new Point(-1.0, -0.5, +1.5, 1, 1, 1));
        control_pts.push_back(*new Point(-0.3, -0.5, +1.5, 1, 1, 1));
        control_pts.push_back(*new Point(+0.3, -0.5, +1.5, 1, 1, 1));
        control_pts.push_back(*new Point(+1.0, -0.5, +1.5, 1, 1, 1));
        control_pts.push_back(*new Point(-1.0, -0.5, +0.5, 1, 1, 1));
        control_pts.push_back(*new Point(-0.3, +1.5, +0.5, 1, 1, 1));
        control_pts.push_back(*new Point(+0.3, +1.5, +0.5, 1, 1, 1));
        control_pts.push_back(*new Point(+1.0, -0.5, +0.5, 1, 1, 1));
        control_pts.push_back(*new Point(-1.0, -0.5, -0.5, 1, 1, 1));
        control_pts.push_back(*new Point(-0.3, +0.5, -0.5, 1, 1, 1));
        control_pts.push_back(*new Point(+0.3, +0.5, -0.5, 1, 1, 1));
        control_pts.push_back(*new Point(+1.0, -0.5, -0.5, 1, 1, 1));
        control_pts.push_back(*new Point(-1.0, -0.5, -1.5, 1, 1, 1));
        control_pts.push_back(*new Point(-0.3, -0.5, -1.5, 1, 1, 1));
        control_pts.push_back(*new Point(+0.3, -0.5, -1.5, 1, 1, 1));
        control_pts.push_back(*new Point(+1.0, -0.5, -1.5, 1, 1, 1));
        control_x=4;
        control_y=4;
        E = new Point(0.0, 0.0, 0.0,0,1,1);
        F = new Point(0.0, 0.0, 0.0,0,1,1);
        G = new Point(0.0, 0.0, 0.0,1,0,1);
        curve = new ParametricCurve(control_pts,control_x,control_y, 0.0, 0.8, 0.0);
        isDrawing=false;
    }

    else{

        if(isEditing){
            *E = curve->getPoint(nb_points);
            E->setRgb(0,1,1);
            F->setX(dx);
            F->setY(dy);
            F->setZ(dz);
            (*F) += (*E);
        }
    }

    int decal=0;
        QVector<float> vertData;
        if(showInterval){
            G= curve->bezierArea(u, v, control_x, control_y);
            G->setRgb(1,0,1);
            G->createPointSingle(&vertData);   decal+=1;
        }
        curve->createCurve(&vertData);     curve->setStart(decal);    decal+=curve->getSize();


        if(isEditing){
            E->createPointSingle(&vertData);
            F->createPointSingle(&vertData);
            decal+=2;
        }
        m_vbo.create();
        m_vbo.bind();
        //qDebug() << "vertData " << vertData.count () << " " << vertData.data ();
        m_vbo.allocate(vertData.constData(), vertData.count() * sizeof(float));
}


void myOpenGLWidget::tearGLObjects()
{
	m_vbo.destroy();
}


void myOpenGLWidget::resizeGL(int w, int h)
{
	qDebug() << __FUNCTION__ << w << h;

	//C'est fait par défaut
	glViewport(0, 0, w, h);

	m_ratio = (double) w / h;
	//doProjection();
}

void myOpenGLWidget::paintGL()
{
    qDebug() << __FUNCTION__ ;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_program->bind(); // active le shader program


        /// Ajout RR pour gérer les 3 matrices utiles
        /// à mettre dans doProjection() pour clarifier
        /// -----------------------------------------
            m_modelView.setToIdentity();
            m_modelView.lookAt(QVector3D(0.0f, 0.0f, 3.0f),    // Camera Position
                             QVector3D(0.0f, 0.0f, 0.0f),    // Point camera looks towards
                             QVector3D(0.0f, 1.0f, 0.0f));   // Up vector

            m_projection.setToIdentity ();
            m_projection.perspective(70.0, width() / height(), 0.1, 100.0); //ou m_ratio


            //m_model.translate(0, 0, -3.0);
            m_modelView.translate(m_x,m_y,m_z);
            // Rotation de la scène pour l'animation
            m_modelView.rotate(m_angleX, 1, 0, 0);
            m_modelView.rotate(m_angleY, 0, 1, 0);
            m_modelView.rotate(m_angleZ, 0, 0, 1);

            QMatrix4x4 m = m_projection * m_modelView * m_model;
            QMatrix3x3 normal_mat = m_modelView.normalMatrix();
        ///----------------------------

        m_program->setUniformValue("matrix", m);
        m_program->setUniformValue("norMatrix", normal_mat);

        m_program->setAttributeBuffer("posAttr", GL_FLOAT, 0 * sizeof(GLfloat), 3, 9 * sizeof(GLfloat));
        m_program->setAttributeBuffer("colAttr", GL_FLOAT, 3 * sizeof(GLfloat), 3, 9 * sizeof(GLfloat));
        m_program->setAttributeBuffer("norAttr", GL_FLOAT, 6 * sizeof(GLfloat), 3, 9 * sizeof(GLfloat));
        m_program->enableAttributeArray("posAttr");
        m_program->enableAttributeArray("colAttr");
        m_program->enableAttributeArray("norAttr");


        glPointSize (10.0f);
        if(showInterval){
            glDrawArrays(GL_POINTS, 0, 1);
        }
        if(isEditing){
            glDrawArrays(GL_POINTS, curve->getStart()+curve->getSize(), 1);
            glDrawArrays(GL_POINTS, curve->getStart()+curve->getSize()+1, 1);
        }
        glPointSize (8.0f);
        glLineWidth(4.0f);

        if(showControlsPts){
            glDrawArrays(GL_POINTS, curve->getStart(), curve->getSizeCurveParam());
            glDrawArrays(GL_LINES, curve->getStart(), curve->getSizeCurveParam());
        }

        glLineWidth(2.0f);
        if(showGrid){
            glDrawArrays(GL_LINES, curve->getStart()+curve->getSizeCurveParam(), curve->getSize()-curve->getSizeCurveParam());
        }
        else{
            glDrawArrays(GL_TRIANGLES, curve->getStart()+curve->getSizeCurveParam(), curve->getSize()-curve->getSizeCurveParam());
        }



        m_program->disableAttributeArray("posAttr");
        m_program->disableAttributeArray("colAttr");

        m_program->release();
}

void myOpenGLWidget::keyPressEvent(QKeyEvent *ev)
{
	qDebug() << __FUNCTION__ << ev->text();

    switch(ev->key()) {
            case Qt::Key_6 :
                rotateRight();
                break;
            case Qt::Key_4 :
                rotateLeft();
                break;
            case Qt::Key_5 :
                rotateBackward();
                break;
            case Qt::Key_8 :
                rotateForward();
                break;
            case Qt::Key_7 :
                m_angleZ += 1;
                if (m_angleZ >= 360) m_angleZ -= 360;
                update();
                break;
            case Qt::Key_9 :
                m_angleZ -= 1;
                if (m_angleZ <= -1) m_angleZ += 360;
                update();
                break;
            case Qt::Key_A :
                toggleControlPolygon();
                break;
            case Qt::Key_E :
                toggleSurface();
                break;
            case Qt::Key_S :
                translateBackward();
                break;
            case Qt::Key_Z :
                translateForward();
                break;
            case Qt::Key_Q :
                translateLeft();
                break;
            case Qt::Key_D :
                translateRight();
                break;
            case Qt::Key_F:
                if(isEditing){
                    dy-=0.1;
                    makeGLObjects();
                }else{
                    m_y+=0.1f;
                }
                    update();
                    break;
            case Qt::Key_R:
                if(isEditing){
                    dy+=0.1;
                    makeGLObjects();
                }else{
                    m_y-=0.1f;
                }
                    update();
                    break;
            case Qt::Key_Space:
                editMode();
                break;
            case Qt::Key_Plus :
                nextPoint();
                break;
            case Qt::Key_Minus :
                previousPoint();
                break;
            case Qt::Key_Right :
                nextPoint_x();
                break;
            case Qt::Key_Left :
                previousPoint_x();
                break;
            case Qt::Key_Up :
                nextPoint_y();
                break;
            case Qt::Key_Down :
                previousPoint_y();
                break;
            case Qt::Key_Escape :
                if (isEditing){
                    dx=0;
                    dy=0;
                    dz=0;
                    makeGLObjects();
                    update();
                }else{
                    qDebug()<< "x= " <<m_x<< "y= " <<m_y<< "z= " <<m_z;
                    qDebug()<< "ax= " <<m_angleX<< "ay= " <<m_angleY<< "az= " <<m_angleZ;
                    qDebug() << "press enter to rest";
                }
                break;
            case Qt::Key_Return :
                if (isEditing){
                    applyPointChange();
                }else{
                    reset();
                }
                break;
        }
}

void myOpenGLWidget::keyReleaseEvent(QKeyEvent *ev)
{
	qDebug() << __FUNCTION__ << ev->text();
}

void myOpenGLWidget::mousePressEvent(QMouseEvent *ev)
{
	qDebug() << __FUNCTION__ << ev->x() << ev->y() << ev->button();
}

void myOpenGLWidget::mouseReleaseEvent(QMouseEvent *ev)
{
	qDebug() << __FUNCTION__ << ev->x() << ev->y() << ev->button();
}

void myOpenGLWidget::mouseMoveEvent(QMouseEvent *ev)
{
	qDebug() << __FUNCTION__ << ev->x() << ev->y();
}

void myOpenGLWidget::onTimeout()
{
	qDebug() << __FUNCTION__ ;

	update();
}

void myOpenGLWidget::setPasHomogene(int value){
    qDebug() << "Pas homogène: " << value;
    curve->setPrecision(value);
    makeGLObjects();
    update();
}
void myOpenGLWidget::setU(double value){
    u=value;
    qDebug() << "U: " << value;
    makeGLObjects();
    update();
}
void myOpenGLWidget::setV(double value){
    v=value;
    qDebug() << "V: " << value;
    makeGLObjects();
    update();
}
void myOpenGLWidget::translateForward(){
    if(isEditing){
        dz-=0.1;
        makeGLObjects();
    }else{
        m_z+=0.1f;
    }
    update();
}
void myOpenGLWidget::translateLeft(){
    if(isEditing){
        dx-=0.1;
        makeGLObjects();
    }else{
        m_x+=0.1f;
    }
    update();
}
void myOpenGLWidget::translateRight(){
    if(isEditing){
        dx+=0.1;
        makeGLObjects();
    }else{
        m_x-=0.1f;
    }
    update();
}
void myOpenGLWidget::translateBackward(){
    if(isEditing){
        dz+=0.1;
        makeGLObjects();
    }else{
        m_z-=0.1f;
    }
    update();
}
void myOpenGLWidget::rotateForward(){
    m_angleX -= 1;
    if (m_angleX <= -1) m_angleX += 360;
    update();
}
void myOpenGLWidget::rotateLeft(){
    m_angleY -= 1;
    if (m_angleY <= -1) m_angleY += 360;
    update();
}
void myOpenGLWidget::rotateRight(){
    m_angleY += 1;
    if (m_angleY >= 360) m_angleY -= 360;
    update();
}
void myOpenGLWidget::rotateBackward(){
    m_angleX += 1;
    if (m_angleX >= 360) m_angleX -= 360;
    update();
}
void myOpenGLWidget::reset(){
        m_x=0;
        m_y=0;
        m_z=0;
        m_angleX=0;
        m_angleY=0;
        m_angleZ=0;
        update();
}

void myOpenGLWidget::applyPointChange(){
    if (isEditing){
        isEditing=false;
        control_pts[nb_points]=*F;
        curve->setPoint(nb_points,*F);
        dx=0;
        dy=0;
        dz=0;
        makeGLObjects();
        update();
    }
}

void myOpenGLWidget::editMode(){
    isEditing=!isEditing;
    makeGLObjects();
    update();
}

void myOpenGLWidget::previousPoint(){
    if (isEditing){
        dx=0;
        dy=0;
        dz=0;
        nb_points--;
        if(nb_points<0)
            nb_points=control_x*control_y-1;
        makeGLObjects();
        update();
    }
}
void myOpenGLWidget::nextPoint(){
    if (isEditing){
        dx=0;
        dy=0;
        dz=0;
        nb_points++;
        if(nb_points>=control_x*control_y)
            nb_points=0;
        makeGLObjects();
        update();
    }
}

void myOpenGLWidget::previousPoint_x(){
    if (isEditing){
        dx=0;
        dy=0;
        dz=0;
        nb_points--;
        if(nb_points<0 || nb_points%control_x==control_x-1)
            nb_points+=control_x;
        makeGLObjects();
        update();
    }
}
void myOpenGLWidget::nextPoint_x(){
    if (isEditing){
        dx=0;
        dy=0;
        dz=0;
        nb_points++;
        if(nb_points%control_x==0)
            nb_points-=control_x;
        makeGLObjects();
        update();
    }
}

void myOpenGLWidget::previousPoint_y(){
    if (isEditing){
        dx=0;
        dy=0;
        dz=0;
        control_x-=control_x;
        if(nb_points<0)
            nb_points+=control_x*control_y;
        makeGLObjects();
        update();
    }
}
void myOpenGLWidget::nextPoint_y(){
    if (isEditing){
        dx=0;
        dy=0;
        dz=0;
        nb_points+=control_x;
        if(nb_points>=control_x*control_y)
            nb_points-=control_x*control_y;
        makeGLObjects();
        update();
    }
}

void myOpenGLWidget::showIntervalParametrique(bool show){
    showInterval=show;
    makeGLObjects();
    update();
}

void myOpenGLWidget::toggleControlPolygon(){
    showControlsPts= !showControlsPts;
    update();
}

void myOpenGLWidget::toggleSurface(){
    showGrid=!showGrid;
    curve->swapGridSurface(showGrid);
    makeGLObjects();
    update();
}

QVector<Point> myOpenGLWidget::getControls_pts(){
    return this->control_pts;
}

int myOpenGLWidget::getControl_x(){
    return this->control_x;
}

int myOpenGLWidget::getControl_y(){
    return this->control_y;
}
