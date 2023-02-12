#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QWidget>

class ScribbleArea : public QWidget
{
    Q_OBJECT
public:
    explicit ScribbleArea(QWidget *parent = nullptr);

    bool openImage(const QString &fileName);
    bool saveImage(const QString &fileName, const char *fileFormat);
    bool isModified() const { return modified; }

public slots:
    void clearImage();
    void print();

protected:
    void paintEvent(QPaintEvent *event);
    bool event(QEvent *event);

private:
    void resizeImage(QImage *image, const QSize &newSize);

    bool modified;
    QList<QColor> myPenColors;
    QImage image;

signals:

};

#endif // SCRIBBLEAREA_H
