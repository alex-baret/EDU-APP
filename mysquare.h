#ifndef MYSQUARE_H
#define MYSQUARE_H
#include <QPainter>
#include <QGraphicsItem>
#include <QDebug>
#include <QGraphicsRectItem>
#include <QtWidgets>
#include <QtGui>
#include <QGraphicsPixmapItem>
#include <QPixmap>


class MySquare : public QGraphicsObject
{
    Q_OBJECT

signals:
    void sendNewHeightSquare(float x, float height , std::string name);
    void detectCollision(MySquare* name);
    void notifySceneToResize();
    void incorrectAnswerChosen();

public:
    MySquare(std::string imagePath, std::string name, int initialX, int initialY , int width, int height);
    QRectF boundingRectangle;
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPixmap image;
    bool pressed;
    std::string name;
    bool canDrop = true;
    bool hasDropped;

    int initialXLoc;
    int initialYLoc;

   int width = 50;
   int height = 62;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // MYSQUARE_H
