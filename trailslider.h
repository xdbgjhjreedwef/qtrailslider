#ifndef TRAILSLIDER_H
#define TRAILSLIDER_H

#include <QSlider>
#include <QPainter>
#include <QDebug>
#include <QStyle>
#include <QStyleOptionSlider>
#include <QPaintEvent>

#include <QProxyStyle>

class SliderProxy : public QProxyStyle
{
public:
    int pixelMetric ( PixelMetric metric, const QStyleOption * option = 0, const QWidget * widget = 0 ) const;
};

class TrailSlider : public QSlider
{
    Q_OBJECT
public:
    explicit TrailSlider(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *ev);
    //void sliderChange(QAbstractSlider::SliderChange change);
    void setTrail(const int newValue);
    int trail() const{return previous;}
protected:
    void setTrack();
    qreal valueToPixel(qreal val) const;
private:
    int previous;
signals:
    //reimp
    int valueChanged(const int);
};

#endif // TRAILSLIDER_H
