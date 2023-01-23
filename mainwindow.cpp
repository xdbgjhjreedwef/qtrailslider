#include "mainwindow.h"
#include "trailslider.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{parent}
{
    TrailSlider *trailslider = new TrailSlider(this);
    setCentralWidget(trailslider);
}
