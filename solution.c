#include "solution.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>



double u(solution_ctx ctx, int i);


double u(solution_ctx ctx, int i)
{
    double res;
    res = ctx->P[i * 2 + 1] / cos(ctx->alpha * ctx->P[i * 2]);
    if (cos(ctx->alpha * ctx->P[i * 2]) > 0)
    {
        if (fabs(res) <= 24)
        {
            return res;
        }
        if (res > 24)
        {
            return 24;
        }
        if (res < -24)
        {
            return -24;
        }
    }
    else
    {
        if (res > 0)
        {
            return -24;
        }
        else
        {
            return 24;
        }
    }
}


double solution_u_v(solution_ctx ctx, int i, int j)
{
    return ctx->P[i * 2 + j];
}

void solution_destroy(solution_ctx ctx)
{
    free(ctx->P);
    free(ctx->F);
    free(ctx);
}

solution_ctx solution_create(int n, double a, double b, double alpha, double gamma_1, double gamma_2)
{
    solution_ctx res_ptr;

    res_ptr = (solution_ctx)malloc(sizeof(*res_ptr));

    res_ptr->n = n;
    res_ptr->a = a;
    res_ptr->b = b;
    res_ptr->k = 0;
    res_ptr->alpha = alpha;
    res_ptr->h_x = (b - a) / (n - 1);
    res_ptr->gamma_1 = gamma_1;
    res_ptr->gamma_2 = gamma_2;

    res_ptr->P = (double*)malloc(2 * n * sizeof(double));
    res_ptr->F = (double*)malloc(4 * n * sizeof(double));




    if (solution_equation(res_ptr) != 0)
    {
        solution_destroy(res_ptr);
        return NULL;
    }


    return res_ptr;
}


int solution_equation(solution_ctx ctx)
{
    double delta_gamma_1, delta_gamma_2, S_1, S_2, eps = 1e-10, cos_1;
    int m = 0;
    double u_1, u_2;
    ctx->P[(ctx->n - 1) * 2] = 0;
    ctx->P[(ctx->n - 2) * 2] = 0;

    S_2 = 10;

    do
    {
        ctx->P[(ctx->n - 1) * 2] = 0;
        ctx->P[(ctx->n - 2) * 2] = 0;
        ctx->P[(ctx->n - 1) * 2 + 1] = ctx->gamma_1;
        ctx->P[(ctx->n - 2) * 2 + 1] = ctx->gamma_2;


        ctx->F[(ctx->n - 1) * 4] = 0;//y_n_gamma_1
        ctx->F[(ctx->n - 2) * 4] = 0;//y_n_1_gamma_1
        ctx->F[(ctx->n - 1) * 4 + 1] = 1;//z_n_gamma_1
        ctx->F[(ctx->n - 2) * 4 + 1] = 0;//z_n_1_gamma_1

        ctx->F[(ctx->n - 1) * 4 + 2] = 0;
        ctx->F[(ctx->n - 2) * 4 + 2] = 0;
        ctx->F[(ctx->n - 1) * 4 + 3] = 0;
        ctx->F[(ctx->n - 2) * 4 + 3] = 1;

        for (int i = ctx->n - 2; i > 0; i--)
        {
            cos_1 = cos(ctx->alpha * ctx->P[i * 2]);
            if (cos_1 > 0)
            {
                if (fabs(ctx->P[i * 2 + 1] / cos(ctx->alpha * ctx->P[i * 2])) < 24)
                {
                    u_1 = (ctx->F[i * 4 + 1] * cos_1 + ctx->P[i * 2 + 1] * ctx->alpha * sin(ctx->alpha * ctx->P[i * 2]) * ctx->F[i * 4]) / pow(cos_1, 2);
                    u_2 = (ctx->F[i * 4 + 3] * cos_1 + ctx->P[i * 2 + 1] * ctx->alpha * sin(ctx->alpha * ctx->P[i * 2]) * ctx->F[i * 4 + 2]) / pow(cos_1, 2);
                }
                else
                {
                    u_1 = 0;
                    u_2 = 0;
                }
            }
            else
            {
                u_1 = 0;
                u_2 = 0;
            }

           

            ctx->F[(i - 1) * 4] = u_1 * ctx->h_x * ctx->h_x + 2 * ctx->F[i * 4] - ctx->F[(i + 1) * 4];
            ctx->F[(i - 1) * 4 + 1] = (-u_1 * (1 + ctx->alpha * u(ctx, i) * sin(ctx->alpha * ctx->P[i * 2])) - ctx->alpha * ctx->alpha / 2 * u(ctx, i) * u(ctx, i) * cos_1 * ctx->F[i * 4]) * ctx->h_x * ctx->h_x + 2 * ctx->F[i * 4 + 1] - ctx->F[(i + 1) * 4 + 1];

            ctx->F[(i - 1) * 4 + 2] = u_2 * ctx->h_x * ctx->h_x + 2 * ctx->F[i * 4 + 2] - ctx->F[(i + 1) * 4 + 2];
            ctx->F[(i - 1) * 4 + 3] = (-u_2 * (1 + ctx->alpha * u(ctx, i) * sin(ctx->alpha * ctx->P[i * 2])) - ctx->alpha * ctx->alpha / 2 * u(ctx, i) * u(ctx, i) * cos_1 * ctx->F[i * 4 + 2]) * ctx->h_x * ctx->h_x + 2 * ctx->F[i * 4 + 3] - ctx->F[(i + 1) * 4 + 3];

            ctx->P[(i - 1) * 2] = u(ctx, i) * ctx->h_x * ctx->h_x - ctx->P[(i + 1) * 2] + 2 * ctx->P[i * 2];
            ctx->P[(i - 1) * 2 + 1] = u(ctx, i) * ctx->h_x * ctx->h_x * (-ctx->alpha / 2 * u(ctx, i) * sin(ctx->alpha * ctx->P[i * 2]) - 1) - ctx->P[(i + 1) * 2 + 1] + 2 * ctx->P[i * 2 + 1];


        }


        if (fabs(ctx->F[0] * ctx->F[3] - ctx->F[2] * ctx->F[1]) > 1e-10)
        {
            if (fabs(ctx->F[0]) > 1e-10)
            {
                delta_gamma_2 = (-ctx->F[0] * ctx->P[1] + ctx->F[1] * (ctx->P[0] - 11)) / (ctx->F[0] * ctx->F[3] - ctx->F[2] * ctx->F[1]);
                delta_gamma_1 = (-ctx->F[2] * delta_gamma_2 - (ctx->P[0] - 11)) / ctx->F[0];
            }
            else
            {
                delta_gamma_2 = -(ctx->P[0] - 11) / ctx->F[2];
                delta_gamma_1 = (-ctx->P[1] - delta_gamma_2 * ctx->F[3]) / ctx->F[1];
                ctx->P[(ctx->n - 1) * 2 + 1] = 0;
                ctx->P[(ctx->n - 2) * 2 + 1] = 0;

            }
        }
        else
        {
            ctx->P[(ctx->n - 1) * 2] = delta_gamma_1;
            ctx->P[(ctx->n - 2) * 2] = delta_gamma_2;
            ctx->P[(ctx->n - 1) * 2 + 1] = ctx->F[1 + (ctx->n - 31) * 4];
            ctx->P[(ctx->n - 2) * 2 + 1] = ctx->F[3 + (ctx->n - 31) * 4];
            

            return 0;
        }

        

        S_1 = sqrt((ctx->P[0] - 11) * (ctx->P[0] - 11) + (ctx->P[1]) * (ctx->P[1]));
        if (S_1 > 100)
        {
            return 0;
        }

        ctx->gamma_1 = ctx->gamma_1 + delta_gamma_1;
        ctx->gamma_2 = ctx->gamma_2 + delta_gamma_2;

        


    } while (S_1 > eps);
   

    return 0;


}





