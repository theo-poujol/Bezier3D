#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent, myOpenGLWidget* glarea) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    this->setWindowTitle("Paramètres et Contrôles");

    connect(this, SIGNAL(pasHomogeneValueChanged(int)), glarea, SLOT(setPasHomogene(int)));
    connect(this, SIGNAL(uValueChanged(double)), glarea, SLOT(setU(double)));
    connect(this, SIGNAL(vValueChanged(double)), glarea, SLOT(setV(double)));

    connect(this, SIGNAL(translationForward()), glarea, SLOT(translateForward()));
    connect(this, SIGNAL(translationLeft()), glarea, SLOT(translateLeft()));
    connect(this, SIGNAL(translationRight()), glarea, SLOT(translateRight()));
    connect(this, SIGNAL(translationBackward()), glarea, SLOT(translateBackward()));

    connect(this, SIGNAL(rotationForward()), glarea, SLOT(rotateForward()));
    connect(this, SIGNAL(rotationLeft()), glarea, SLOT(rotateLeft()));
    connect(this, SIGNAL(rotationRight()), glarea, SLOT(rotateRight()));
    connect(this, SIGNAL(rotationBackward()), glarea, SLOT(rotateBackward()));

    connect(this, SIGNAL(reset()), glarea, SLOT(reset()));

    connect(this, SIGNAL(editMode()), glarea, SLOT(editMode()));
    connect(this, SIGNAL(previousPoint()), glarea, SLOT(previousPoint()));
    connect(this, SIGNAL(nextPoint()), glarea, SLOT(nextPoint()));
    connect(this, SIGNAL(applyPointChange()), glarea, SLOT(applyPointChange()));

    connect(this, SIGNAL(showIntervalParametrique(bool)), glarea, SLOT(showIntervalParametrique(bool)));

    connect(this, SIGNAL(toggleControlPolygon()), glarea, SLOT(toggleControlPolygon()));
    connect(this, SIGNAL(toggleSurface()), glarea, SLOT(toggleSurface()));


    ui->controlTranslationForward->setIcon(icon_up_arrow);
    ui->controlTranslationBackward->setIcon(icon_down_arrow);
    ui->controlTranslationLeft->setIcon(icon_left_arrow);
    ui->controlTranslationRight->setIcon(icon_right_arrow);

    ui->controlRotationForward->setIcon(icon_up_arrow);
    ui->controlRotationBackward->setIcon(icon_down_arrow);
    ui->controlRotationLeft->setIcon(icon_left_arrow);
    ui->controlRotationRight->setIcon(icon_right_arrow);

}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_pasHomogeneSpinbox_valueChanged(int arg1)
{
    emit pasHomogeneValueChanged(arg1);
}


void Settings::on_uSpinbox_valueChanged(double arg1)
{
    emit uValueChanged(arg1);
    ui->uSlider->blockSignals(true);
    ui->uSlider->setValue(arg1*100);
    ui->uSlider->blockSignals(false);
}

void Settings::on_vSpinbox_valueChanged(double arg1)
{
    emit vValueChanged(arg1);
    ui->vSlider->blockSignals(true);
    ui->vSlider->setValue(arg1*100);
    ui->vSlider->blockSignals(false);
}

void Settings::on_uSlider_valueChanged(int value)
{
    double tmp = value * 0.01;
    emit uValueChanged(tmp);
    ui->uSpinbox->blockSignals(true);
    ui->uSpinbox->setValue(tmp);
    ui->uSpinbox->blockSignals(false);
}

void Settings::on_vSlider_valueChanged(int value)
{
    double tmp = value * 0.01;
    emit vValueChanged(tmp);
    ui->vSpinbox->blockSignals(true);
    ui->vSpinbox->setValue(tmp);
    ui->vSpinbox->blockSignals(false);
}

void Settings::on_controlTranslationForward_clicked()
{
    emit translationForward();
}

void Settings::on_controlTranslationLeft_clicked()
{
    emit translationLeft();
}

void Settings::on_controlTranslationRight_clicked()
{
    emit translationRight();
}

void Settings::on_controlTranslationBackward_clicked()
{
    emit translationBackward();
}

void Settings::on_controlRotationForward_clicked()
{
    emit rotationForward();
}

void Settings::on_controlRotationLeft_clicked()
{
    emit rotationLeft();
}

void Settings::on_controlRotationRight_clicked()
{
    emit rotationRight();
}

void Settings::on_controlRotationBackward_clicked()
{
    emit rotationBackward();
}

void Settings::on_resetButton_clicked()
{
    emit reset();
}

void Settings::on_editModeButton_clicked()
{
    emit editMode();
}

void Settings::on_nextPointButton_clicked()
{
    emit nextPoint();
}

void Settings::on_previousPointButton_clicked()
{
    emit previousPoint();
}

void Settings::on_checkShow_clicked(bool checked)
{
    emit showIntervalParametrique(checked);
}

void Settings::on_displayControlPolygon_clicked()
{
    emit toggleControlPolygon();
}

void Settings::on_displaySurface_clicked()
{
    emit toggleSurface();
}

void Settings::on_applyPointChange_clicked()
{
    emit applyPointChange();
}
