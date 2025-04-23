#include <iostream>
#include <QMainWindow>
#include <QApplication>
#include <QMenuBar>
#include "window.h"

int main(int argc, char* argv[])
{
    int n;
    double a, b, alpha, gamma_1, gamma_2;
    n = 10000;
    b = 4;
    a = 0;
    alpha = 0;
    gamma_1 = -11 / (4 * cos(4) - sin(4)) * sin(4);
    gamma_2 = -11 / (4 * cos(4) - sin(4)) * sin(4);
    QApplication app(argc, argv);
    QMainWindow* window = new QMainWindow;
    QMenuBar* bar = new QMenuBar(window);
    window->resize(1100, 800);

    Canvas* canvas;

    canvas = new Canvas(window, n, a, b, alpha, gamma_1, gamma_2);

    QAction* action;



    action = bar->addAction("Plus alpha", canvas, SLOT(plus_alpha()));
    action->setShortcut(QString("2"));

    action = bar->addAction("Minus alpha", canvas, SLOT(minus_alpha()));
    action->setShortcut(QString("3"));

    action = bar->addAction("Increase amount of points", canvas, SLOT(increase_points()));
    action->setShortcut(QString("4"));

    action = bar->addAction("Decrease amount of points", canvas, SLOT(decrease_points()));
    action->setShortcut(QString("5"));

    window->setMenuBar(bar);



    window->setCentralWidget(canvas);
    window->show();
    app.exec();

    delete canvas;

    delete window;

    return 0;
}
