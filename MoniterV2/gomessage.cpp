#include "gomessage.h"
#include "ui_gomessage.h"
#include <QTimer>
#include <QPalette>
goMessage::goMessage(const QString& mess, int t, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::goMessage)
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint
                   | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint
                   | Qt::WindowMaximizeButtonHint);

    ui->setupUi(this);
    ui->label->setText(mess);
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::red);
    ui->label->setPalette(pa);
    setModal(true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    ui->label->setAttribute(Qt::WA_TranslucentBackground, true);
    QTimer::singleShot(t, this, SLOT(timeOver()));
}
void goMessage::timeOver()
{
    this->close();
    delete this;
}
goMessage::~goMessage()
{
    delete ui;
}
