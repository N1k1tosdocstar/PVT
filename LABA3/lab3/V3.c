#include <stdio.h>
#include <math.h>
#include <omp.h>

double f(double x) {
    return sqrt(x*(3-x))/(x+1);
}

double rectangle_integration(double a, double b, int n) {
    double sum = 0.0;
    double h = (b - a) / n;
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < n; i++) {
        double x = a + (i + 0.5) * h;
        sum += f(x);
    }
    return sum * h;
}

int main() {
    double a = 1.0;
    double b = 1.2;
    double epsilon = 1E-5;
    double exact_value = rectangle_integration(a, b, 1000000);
    int num_threads[] = {2, 4, 6, 8};

    printf("Количество потоков\tВремя ожидания (секунды)\tПри значение n\n");

    for (int i = 0; i < 4; i++) {
        omp_set_num_threads(num_threads[i]);
        int n = 1000000; // Значение n
        double start_time = omp_get_wtime();
        double approx_value = rectangle_integration(a, b, n);
        double elapsed_time = omp_get_wtime() - start_time;
        printf("%d\t\t\t%.6f\t\t\t\t%d\n", num_threads[i], elapsed_time, n);
        
        if(fabs((approx_value - exact_value) / exact_value) < epsilon) {
            printf("Интеграция сходится с epsilon = %f\n", epsilon);
        } else {
            printf("Интеграция не сходится с epsilon = %f\n", epsilon);
        }
    }

    return 0;
}
