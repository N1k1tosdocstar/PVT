#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

double f(double x, double y) {
    return exp(x - y);
}

double compute_integral(int n) {
    double x, y, sum = 0.0;
    int i;
    
    // Определение области интегрирования
    double x_min = -1.0;
    double x_max = 0.0;
    double y_min = 0.0;
    double y_max = 1.0;

    // Настройка генератора случайных чисел
    srand(12345);

    // Вычисление интеграла методом Монте-Карло
    #pragma omp parallel for private(x, y) reduction(+:sum)
    for (i = 0; i < n; i++) {
        x = (double)rand() / RAND_MAX * (x_max - x_min) + x_min;
        y = (double)rand() / RAND_MAX * (y_max - y_min) + y_min;
        sum += f(x, y);
    }

    return sum;
}

int main() {
    double start_time, end_time;
    int n;
    double result;
    
    // Перебор числа точек для экспериментов
    for (n = 10000000; n <= 100000000; n *= 10) {
        printf("Число: %d\n", n);
        
        // Перебор числа потоков
        for (int num_threads = 2; num_threads <= 8; num_threads += 2) {
            printf("Число потоков: %d\n", num_threads);
            
            // Установка числа потоков
            omp_set_num_threads(num_threads);

            // Вычисление интеграла методом Монте-Карло
            start_time = omp_get_wtime(); // начало отсчета времени
            result = compute_integral(n);
            end_time = omp_get_wtime(); // конец отсчета времени
            
            // Вычисление среднего значения
            result /= n;

            printf("Integral: %f\n", result);
            printf("Затраченное время: %f секунд\n", end_time - start_time);
        }
        printf("\n");
    }

    return 0;
}
