#include "trailslider.h"
#include <QToolTip>
#include <QDebug>

int SliderProxy::pixelMetric( PixelMetric metric, const QStyleOption * option, const QWidget * widget) const
{
    switch(metric) {
    case PM_SliderThickness  : return 80;
    case PM_SliderControlThickness : return 80;
    case PM_SliderLength     : return 50;
    //case PM_SliderSpaceAvailable : return 50;
    default                         : return (QProxyStyle::pixelMetric(metric,option,widget));
    }
}
///in constructor:


TrailSlider::TrailSlider(QWidget *parent) : QSlider(parent)
{
    previous=0;
    setOrientation(Qt::Horizontal);
    connect(this,&QSlider::sliderReleased,this,&TrailSlider::setTrack);
    connect(this,&QSlider::valueChanged,this,&TrailSlider::setTrack);

    SliderProxy *sliderProxy = new SliderProxy();
    setStyle(sliderProxy);
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);//

    qDebug()<<"min h"<<minimumHeight()<<"height"<<height()<<"size policy:"<<sizePolicy();
}

void TrailSlider::paintEvent(QPaintEvent *ev)
{
    (void)ev;
    QStyleOptionSlider option;
    initStyleOption(&option);
    const int backgroundHeight = 15;
    const int leftRightMargin = 3;
    const int handleControlHeight = style()->pixelMetric(QStyle::PM_SliderThickness,&option,this);
    const int handleControlLength = style()->pixelMetric(QStyle::PM_SliderLength,&option,this);
    const int handleHeight = 13;
    const int handleLength = handleControlLength;
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawRoundedRect(1,1,width()-2, height()-2,3,3);
    QRectF backgroundRect = QRectF(leftRightMargin, (height()-backgroundHeight)/2, width() - leftRightMargin * 2, backgroundHeight);
    QPen pen(Qt::gray, 0.8);
    painter.setPen(pen);
    painter.setBrush(QBrush(QColor(Qt::lightGray)));
    painter.drawRoundedRect(backgroundRect, 1, 1);
    // First value handle rect
    const qreal handlePos=valueToPixel(value());
    const qreal trailPos=valueToPixel(previous);//TODO: getset const previos

    QRectF trailRect(trailPos-handleLength/2, (height()-handleHeight) / 2, handleLength, handleHeight);
    painter.setBrush(QBrush(QColor(Qt::darkGray)));
    painter.drawRoundedRect(trailRect, 2, 2);

    QRectF trackRect(trailPos, (height()-4) / 2, handlePos-trailPos, 4);
    painter.setBrush(QBrush(QColor(Qt::green)));
    painter.drawRect(trackRect);

    QRectF handleRect(handlePos-handleLength/2, (height()-handleHeight) / 2, handleLength, handleHeight);
    painter.setBrush(QBrush(QColor(Qt::white)));
    painter.drawRoundedRect(handleRect, 2, 2);

    QFont handleFont = painter.font();
    const int handleFontSize=24;
    QFontMetrics metrics(handleFont);
    handleFont.setPixelSize(handleFontSize);
    painter.setFont(handleFont);

    const int handleFontBoundingRectLength = metrics.boundingRect(QString::number(-9999)).width()*1.5;
    const double handleTextRectPosMultiplier = 1-(double)handleFontBoundingRectLength/width();
    QRectF handleTextRect(handlePos*handleTextRectPosMultiplier,(height()-handleHeight)/2 - handleFontSize-2-2,
                          handleFontBoundingRectLength+2, handleFontSize+2);//text
    painter.setPen(Qt::black);
    painter.setBrush(QBrush(QColor(Qt::white)));
    painter.drawRect(handleTextRect.adjusted(0, 0, -pen.width(), -pen.width()));
    painter.drawText(handleTextRect, Qt::AlignCenter, QString::number(value()));

    const int trailingHandleFontBoundingRectLength = metrics.boundingRect(QString::number(-9999)).width()*1.5;
    const double trailTextRectPosMultiplier = 1-(double)trailingHandleFontBoundingRectLength/width();
    QRectF trailTextRect(trailPos*trailTextRectPosMultiplier,(height()-handleHeight)/2 + handleFontSize-2-2-2,
                          trailingHandleFontBoundingRectLength+2, handleFontSize+2);
    painter.setPen(Qt::white);
    painter.setBrush(QBrush(QColor(Qt::black)));
    painter.drawRect(trailTextRect.adjusted(0, 0, -pen.width(), -pen.width()));
    painter.drawText(trailTextRect, Qt::AlignCenter, QString::number(trail()));
}

void TrailSlider::setTrack()
{
    if(!isSliderDown())
    {
        emit valueChanged(value());
    }
}

void TrailSlider::setTrail(const int newValue)
{
    previous=newValue;
    update();
}

qreal TrailSlider::valueToPixel(qreal val) const
{
    return (qreal)width()*
            (val-(qreal)minimum())/
            (abs(qreal(maximum()))+abs(qreal(minimum())));
}
