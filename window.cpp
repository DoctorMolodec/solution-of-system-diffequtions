#include "window.h"
#include <algorithm>
#include <QPainter>
#include <iostream>
#include <string>
#include <math.h>

Canvas::Canvas(QWidget* parent, int n, double a, double b, double alpha, double gamma_1, double gamma_2)
    : QWidget(parent), n(n), a(a), b(b), alpha(alpha), gamma_1(gamma_1), gamma_2(gamma_2),
    ctx(solution_create(n, a, b, alpha, gamma_1, gamma_2))
{

    colors[0] = QColor(250, 0, 0);;
    colors[1] = QColor(0, 250, 0);
    h_x = (b - a) / (n - 1);

    draw_f();
    update();
}



void Canvas::draw_f()
{
    if (new_solutions(n, a, b, alpha, gamma_1, gamma_2) != 0)
    {
        function_name1 = "ERROR;";
        function_name2 = "ERROR;";

    }
    function_name1 = "u' + alpha * v = cos x;";
    function_name2 = "v' - alpha * u = 0;";


    update();
}

void Canvas::increase_points()
{
    n = n * 2;
    if (new_solutions(n, a, b, alpha, gamma_1, gamma_2) != 0)
    {
        function_name1 = "ERROR;";
        function_name2 = "ERROR;";

    }
    h_x = (b - a) / (n - 1);
    update();
}

void Canvas::decrease_points()
{
    if (n < 10)
        return;
    n = n / 2;
    if (new_solutions(n, a, b, alpha, gamma_1, gamma_2) != 0)
    {
        function_name1 = "ERROR;";
        function_name2 = "ERROR;";

    }
    h_x = (b - a) / (n - 1);
    update();
}



void Canvas::plus_alpha()
{

    
    alpha += 0.01;
    gamma_1 = ctx->gamma_1;
    gamma_2 = ctx->gamma_2;
    if (new_solutions(n, a, b, alpha, gamma_1, gamma_2) != 0)
    {
        function_name1 = "ERROR;";
        function_name2 = "ERROR;";

    }

    

}


    update();

}

void Canvas::minus_alpha()
{
    if (alpha < -1.4)
        return;
    alpha -= 0.01;
    gamma_1 = ctx->gamma_1;
    gamma_2 = ctx->gamma_2;
    if (new_solutions(n, a, b, alpha, gamma_1, gamma_2) != 0)
    {
        function_name1 = "ERROR;";
        function_name2 = "ERROR;";

    }
    update();
}

double Canvas::u_v(int i, int j)
{
    return solution_u_v(ctx, i, j);
}








double Canvas::h()
{
    return (b - a) / (double)width();
}

void Canvas::paintEvent(QPaintEvent*)
{
    double y1 = 0, y2, x1, x2, graph_sup, graph_inf;
    char s1[10];

    inf = u_v(0, 0);
    sup = u_v(0, 0);

    for (int j = 0; j < 2; j++)
        for (int i = 0; i < n; i++)
        {
            y1 = u_v(i, j);
            if (y1 >= sup)
            {
                sup = y1;
            }
            if (y1 <= inf)
            {
                inf = y1;
            }
        }

    if ((sup < 1e-15) && (fabs(inf) < 1e-15))
    {
        graph_sup = 10;
        graph_inf = -10;
    }
    else
    {
        graph_sup = sup;
        graph_inf = inf;
    }
    QPainter painter(this);
    painter.save();
    painter.translate(width() / 2, height() / 2);
    painter.scale(width() / (b - a), -height() / (graph_sup - graph_inf) * 0.9);
    painter.translate(-(a + b) / 2, -(graph_inf + graph_sup) / 2);


    QPen pen;
    pen.setWidth(0);
    pen.setColor("black");
    painter.setPen(pen);
    painter.drawLine(a - 1, 0.0, b + 1, 0.0);



    for (int j = 0; j < 2; j++)
    {
        pen.setColor(colors[j]);
        painter.setPen(pen);
        x1 = a;
        for (int i = 0; i < n - 1; i++)
        {
            x2 = x1 + h_x;
            y1 = ctx->P[2 * i + j];
            y2 = ctx->P[2 * (i + 1) + j];
            painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
            x1 = x2;
        }

    }







    painter.restore();






    painter.drawText(20.0, 20.0, function_name1);
    painter.drawText(20.0, 40.0, function_name2);

    snprintf(s1, sizeof s1, "%lf", inf);
    painter.drawText(250.0, 20.0, s1);
    painter.drawText(220.0, 20.0, "inf:");
    snprintf(s1, sizeof s1, "%lf", sup);
    painter.drawText(360.0, 20.0, s1);
    painter.drawText(330.0, 20.0, "sup:");
    snprintf(s1, sizeof s1, "%d", n);
    painter.drawText(250.0, 40.0, s1);
    painter.drawText(220.0, 40.0, "n:");
    snprintf(s1, sizeof s1, "%lf", a);
    painter.drawText(260.0, 60.0, s1);
    snprintf(s1, sizeof s1, "%lf", b);
    painter.drawText(330.0, 60.0, s1);
    painter.drawText(220.0, 60.0, "[a;b]:");
    snprintf(s1, sizeof s1, "%lf", ctx->P[2 * (n - 1)]);
    painter.drawText(520.0, 20.0, "u(4) =");
    painter.drawText(560.0, 20.0, s1);
    snprintf(s1, sizeof s1, "%lf", ctx->P[2 * (n - 1) + 1]);
    painter.drawText(520.0, 40.0, "v(4) =");
    painter.drawText(560.0, 40.0, s1);
    snprintf(s1, sizeof s1, "%lf", ctx->P[2 * (n - 2)]);
    painter.drawText(520.0, 60.0, "u(4-) =");
    painter.drawText(560.0, 60.0, s1);
    snprintf(s1, sizeof s1, "%lf", ctx->P[2 * (n - 2) + 1]);
    painter.drawText(520.0, 80.0, "v(4-) =");
    painter.drawText(560.0, 80.0, s1);
    snprintf(s1, sizeof s1, "%lf", ctx->P[0]);
    painter.drawText(520.0, 100.0, "u(0) =");
    painter.drawText(560.0, 100.0, s1);
    snprintf(s1, sizeof s1, "%lf", ctx->P[1]);
    painter.drawText(520.0, 120.0, "v(0) =");
    painter.drawText(560.0, 120.0, s1);
    snprintf(s1, sizeof s1, "%lf", alpha);
    painter.drawText(520.0, 140.0, "alpha =");
    painter.drawText(570.0, 140.0, s1);

}

int Canvas::new_solutions(int n, double a, double b, double alpha, double gamma_1, double gamma_2)
{
    solution_ctx t = solution_create(n, a, b, alpha, gamma_1, gamma_2);
    if (t == NULL)
    {
        return -1;
    }
    solution_destroy(ctx);
    ctx = t;
    return 0;

}

Canvas::~Canvas()
{
    solution_destroy(ctx);
}
