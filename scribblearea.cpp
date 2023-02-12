#include "scribblearea.h"
#include <QtGui>
#include <iostream>
#include<QMessageBox>

ScribbleArea::ScribbleArea(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_AcceptTouchEvents);
    setAttribute(Qt::WA_StaticContents);

    myPenColors
            << QColor("green")
            << QColor("purple")
            << QColor("red")
            << QColor("blue")
            << QColor("yellow")

            << QColor("pink")
            << QColor("orange")
            << QColor("brown")
            << QColor("grey")
            << QColor("black");
    openImage("test");

}

bool ScribbleArea::openImage(const QString &fileName)
{
//    QImage loadedImage;
//    if (!loadedImage.load(fileName)){
//        QFile file(fileName);
//        if(!file.open(QIODevice::ReadOnly)){
//            QMessageBox::warning(this,QString("提示"),QString("打开图片文件失败！%1").arg(fileName));
//            return false;
//        }
//        if(!loadedImage.loadFromData(file.readAll()))
//        {

//            std::cout<<"loadedImage.load(fileName) false"<<std::endl;
//            return false;
//        }
//    }

//    QSize newSize = loadedImage.size().expandedTo(size());
//    resizeImage(&loadedImage, newSize);

    QImage loadedImage(size(), QImage::Format_RGB32);
    loadedImage.fill(qRgb(255, 255, 255));
    image = loadedImage;

    modified = false;
    update();
    return true;
}

bool ScribbleArea::saveImage(const QString &fileName, const char *fileFormat)
{
    QImage visibleImage = image;
    resizeImage(&visibleImage, size());

    if (visibleImage.save(fileName, fileFormat)) {
        modified = false;
        return true;
    } else {
        return false;
    }
}

void ScribbleArea::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

void ScribbleArea::clearImage()
{
    image.fill(qRgb(255, 255, 255));
    modified = true;
    update();
}
void ScribbleArea::print()
{
#ifndef QT_NO_PRINTER
//    std::cout<<"un used ……"<<std::endl;
//    QPrinter printer(QPrinter::HighResolution);

//    QPrintDialog *printDialog = new QPrintDialog(&printer, this);
//    if (printDialog->exec() == QDialog::Accepted) {
//        QPainter painter(&printer);
//        QRect rect = painter.viewport();
//        QSize size = image.size();
//        size.scale(rect.size(), Qt::KeepAspectRatio);
//        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
//        painter.setWindow(image.rect());
//        painter.drawImage(0, 0, image);
//    }
#endif // QT_NO_PRINTER
}
void ScribbleArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    int w=this->width();
    int h=this->height();
    const QRect rect = event->rect();
    painter.drawImage(rect.topLeft(), image, rect);
}

bool ScribbleArea::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::TouchBegin:
    case QEvent::TouchUpdate:
    case QEvent::TouchEnd:
    {
        QList<QTouchEvent::TouchPoint> touchPoints = static_cast<QTouchEvent *>(event)->touchPoints();
        foreach (const QTouchEvent::TouchPoint &touchPoint, touchPoints) {
            switch (touchPoint.state()) {
            case Qt::TouchPointStationary:
                // don't do anything if this touch point hasn't moved
                continue;
            default:
                {
                    QRectF rect = touchPoint.rect();
                    if (rect.isEmpty()) {

                        qreal diameter = qreal(30) * touchPoint.pressure();

                        rect.setSize(QSizeF(diameter, diameter));
                        rect.setX(touchPoint.pos().x() - 0);
                        rect.setY(touchPoint.pos().y() - 0);
                    }

                    QPainter painter(&image);
                    painter.setPen(Qt::NoPen);
                    painter.setBrush(myPenColors.at(touchPoint.id() % myPenColors.count()));
                    painter.drawEllipse(rect);
                    painter.end();

                    modified = true;
                    int rad = 2;
                    update(rect.toRect().adjusted(-rad,-rad, +rad, +rad));
                }
                break;
            }
        }
        break;
    }
    default:
        return QWidget::event(event);
    }
    return true;
}
