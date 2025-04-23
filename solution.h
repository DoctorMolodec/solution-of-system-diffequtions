#ifndef __SOLUTION_H
#define __SOLUTION_H

struct solution_ctx_inner;
typedef struct solution_ctx_inner* solution_ctx;

struct solution_ctx_inner {

    int	n;
    double	a, b;
    int k;
    double* P;
    double* F;

    double alpha;
    double h_x;
    double gamma_1;
    double gamma_2;


};


#ifdef __cplusplus
extern "C" {
#endif

    void solution_destroy(solution_ctx ctx);
    solution_ctx solution_create(int n, double a, double b, double alpha, double gamma_1, double gamma_2);
    int solution_equation(solution_ctx ctx);
    double solution_u_v(solution_ctx ctx, int i, int j);

#ifdef __cplusplus
}
#endif

#endif
