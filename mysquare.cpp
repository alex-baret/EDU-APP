#include "mysquare.h"
#include <iostream>


/**
 * Constructor for custom item.
 * @brief MySquare::MySquare
 */
MySquare::MySquare(std::string imagePath, std::string name , int initialX, int initialY, int width, int height)
{
    pressed = false;
    QString QImagePath =  QString::fromStdString(imagePath); //converts image path to QString
    image.load(QImagePath); //loads QString image path to this square's pixmap

    this->name = name;

    this->initialXLoc = initialX;
    this->initialYLoc = initialY;

    this->width = width;
    this->height = height;


    this->setToolTip(QString::fromStdString(name));
}

/**
 * Returns the boundaries of the custom shape.
 * @brief MySquare::boundingRect
 * @return
 */
QRectF MySquare::boundingRect() const
{
    return QRectF(this->initialXLoc,this->initialYLoc,this->width,this->height); //outer-most edge of the shape
}

/**
 * Paints the custom item.
 * @brief MySquare::paint
 * @param painter
 * @param option
 * @param widget
 */
void MySquare::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(initialXLoc,initialYLoc,width,height,image);
    if(pressed)
    {
        setFlag(GraphicsItemFlag::ItemIsMovable,true);
    }
}

/**
 * Mouse pressed event handler, repaints object and notifies other listeners that the mouse was pressed.
 * @brief MySquare::mousePressEvent
 * @param event
 */
void MySquare::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //emit notifySceneToResize();
    pressed = true;

    update(); //forces object to repaint
    QGraphicsItem::mousePressEvent(event);
}

/**
 * Mouse pressed event handler, repaints object and notifies other listeners that the mouse was pressed.
 * @brief MySquare::mouseReleaseEvent
 * @param event
 */
void MySquare::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
//    emit notifySceneToResize();
    pressed = false;
    update(); //forces object to repaint
    QGraphicsItem::mouseReleaseEvent(event);


    float x = this->x();
    float y = this->y();

    //hardcoding in boundaries of scene size. NOTE: if you adjust the scene size you must update these values
    if(x < 400 && x > -675 && y < 320 && y > -320){
        emit detectCollision(this);
        //only drop the object if it can
        if(canDrop){
            emit sendNewHeightSquare(x, y , name);
            hasDropped = true;
            emit incorrectAnswerChosen();
        }
    }else{
        //set position to initial position
        this->setPos(initialXLoc, initialYLoc);
    }
}
