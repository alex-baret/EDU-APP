#ifndef PATIENT_H
#define PATIENT_H
#include <QPainter>
#include <QGraphicsItem>
#include <QDebug>
#include <QGraphicsRectItem>
#include <QtWidgets>
#include <QtGui>
#include <QGraphicsPixmapItem>
#include <QPixmap>


class Patient : public QGraphicsObject
{
    Q_OBJECT

signals:
    void sendNewHeightSquare(float x, float height , std::string name);
    void detectCollision(std::string name);
public:
    Patient(std::string imagePath, std::string name);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPixmap image;
    bool pressed;
    std::string name;
    bool canDrop = true;
    int stage;

};

#endif // PATIENT_H
