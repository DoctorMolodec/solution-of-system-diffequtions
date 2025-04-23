#include <cmath>
#include <QWidget>
#include "solution.h"


class Canvas : public QWidget
{
    Q_OBJECT
public:
    int n;
    double alpha;
    double a, b;
    double h_x;
    solution_ctx ctx;
    double u_v(int i, int j);
    const char* function_name1;
    const char* function_name2;
    double sup, inf;
    double gamma_1, gamma_2;
    QColor colors[2];





    double h();

    Canvas(QWidget* parent, int n, double a, double b, double alpha, double gamma_1, double gamma_2);


    void paintEvent(QPaintEvent* event);

    ~Canvas();

public slots:
    int new_solutions(int n, double a, double b, double alpha, double gamma_1, double gamma_2);

    void draw_f();

    void minus_alpha();

    void plus_alpha();

    void increase_points();

    void decrease_points();


};
