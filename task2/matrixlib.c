#include "matrixlib.h"

int check_input(int err, char * c) {                        //проверка правильности ввода
    if(err == 1) {                                          // проверка на число
        if(isdigit(*c)) {
            return 0;
        } else {
            printf("\nWrong input format. Enter number: ");
            return 1;
        }
    }
    if(err == 2) {                                          //проверка на ЦЕЛОЕ число
        if(!isdigit(*c)) {
            printf("\nWrong input format. Enter number: ");
            return 1;
        } else {
            if(atof(c) - atoi(c) > 0) {
                printf("\nWrong input format. Enter number: ");
                return 1;
            } else {
                return 0;
            }
        }
    }
}

void instruction(void) {                                     //описание библиотеки ??
    printf("\n-----------\n%s\n-----------\n", "List of avaliable commands:");
    printf("1. output the matrix\n");
    printf("2. addition of 2 matrices\n");
    printf("3. multiplication of 2 matrices\n");
    printf("4. multiplying a matrix by a number\n");
    printf("5. calculation of the determinant of the matrix\n");
    printf("6. exit\n");
    printf("-----------\n");
}

double * input_m(int n, int m) {                                //ввод матрицы
    printf("-----------\nEntering a matrix: \n");
    double * arr;
    char c[256];
    int flag = 0;
    arr = malloc(n * m * sizeof(double));
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            do{
                scanf("%s", &c);
                flag = check_input(1, c);
            }
            while (flag == 1);
            flag = 0;
            arr[i * n + j] = atof(c);
        }
    }
    return arr;
}

void output_m(double * m_arr, int n, int m) {                       //вывод матрицы
    printf("-----------\nPrinting a matrix: \n");
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            printf("%.2lf\t", m_arr[i * n + j]);
        }
        printf("\n");
    }
}

void rows_cols(int * n, int * m) {                                  //ввод кол-ва столбцов и строк для доп. матрицы
    char c[265]; int flag = 0;
    printf("\nEnter the number of rows: ");
    do{
        scanf("%s", &c);
        flag = check_input(2, c);
    }
    while (flag == 1);
    * n  = atof(c);
    printf("and the number of columns: ");
    do{
        scanf("%s", &c);
        flag = check_input(2, c);
    }
    while (flag == 1);
    * m  = atof(c);
}

double * sum_m(double * m1_arr, int n1, int m1) {                       //суммирование матриц
    int n2, m2;
    printf("-----------\nMatrix addition\n");

    rows_cols(&n2, &m2);

    if ((n1 != n2) || (m1 != m2)) {                                     //проверка на совместимость
        printf("\n! The sizes of the matrices do not match !");
        return NULL;
    }
    double * m2_arr = input_m(n2, m2);

    int n = n1;
    int m = m1;

    double * sum_arr;
    sum_arr = malloc(n * m * sizeof(double));
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            sum_arr[i * n + j] = m1_arr[i * n + j] + m2_arr[i * n + j];
        }
    }
    free(m2_arr);

    return sum_arr;
}

double * mul_m(double * m1_arr, int n1, int m1, int * k) {                      //перемножение матриц
    printf("-----------\nMultiplying a matrix by a matrix:\n");

    int m2, k2;
    rows_cols(&m2, &k2);

    if(m1 != m2) {
        printf("\n! The sizes of the matrices do not match !");
        return NULL;
    }
    
    double * m2_arr = input_m(m2, k2);
    int mm = m1;

    double * m_arr;
    m_arr = malloc(n1 * k2 * sizeof(double));
    int sum = 0;
    for(int i = 0; i < n1; i++) {
        for(int k = 0; k < k2; k++) {
            for(int j = 0; j < mm; j++) {
                sum += m1_arr[i * n1 + j] * m2_arr[j * mm + k];
            }
            m_arr[i * n1 + k] = sum;
            sum = 0;
        }
    }
    * k = k2;

    m1_arr = realloc(m1_arr, n1 * k2 * sizeof(double));                         //перераспределение памяти + копирование в исходную матрицу 
    for(int i = 0; i < n1; i++){
        for(int j = 0; j < k2; j++){
            m1_arr[i * n1 + j] = m_arr[i * n1 + j];
        }
    }
    free(m2_arr);
    free(m_arr);

    return m1_arr;
}

double * num_m(double * m_arr, int n, int m) {
    printf("-----------\nMultiplying a matrix by a number:\n");
    double * num_arr;
    char c; int flag = 0;
    num_arr = malloc(n * m * sizeof(double));

    double k;
    printf("\nEnter number: ");
    do{
        scanf("%s", &c);
        flag = check_input(1, &c);
    }
    while (flag == 1);
    k = atof(&c);

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++){
            num_arr[i * n + j] = m_arr[i * n + j] * k;
        }
    }
    return num_arr;
}

double determinant_gauss(double * m_arr, int n, int m) {
    printf("-----------\nCalculation of the determinant of the matrix by the Gauss method:\n");
    if (m != n) {
        printf("\n! the matrix is not square !");
        exit;
    }

    double * additional_m = malloc(n * m * sizeof(double));                 //новая матрица, в которой будем производить элем. преобр., чтобы не портить исходную

    for(int ii = 0; ii < n; ii++){
        for(int jj = 0; jj < m; jj++){
            additional_m[ii * n + jj] = m_arr[ii * n + jj];
        }
    }

    int minim = INT_MAX, i = 0 , j = 0, l, sq = n;
    double k = 0, det = 1;

    while(i < sq && j < sq) {
        for(int ii = i; ii < sq; ii++) {                                   //находим минимальный элемент в j-ом столбце
            if(abs(additional_m[ii * sq + j]) < minim) {
                minim = abs(additional_m[ii * sq + j]);
                l = ii;
            }
        }
        minim = INT_MAX;

        int temp;
        if(l != i) {                                                        //меняем местами строку с мин. первым эл-ом и текущую, если нужно
            det *= -1;
            for(int jj = 0; jj < sq; jj++) {
                temp = additional_m[i * sq + jj];
                additional_m[i * sq + jj] = additional_m[l * sq + jj];
                additional_m[l * sq + jj] = temp;
            }
        }

        for(int ii = i + 1; ii < sq; ii++) {                                //обнуляем э-лы j-ого столбца в строках с номером > i
            k = additional_m[ii * sq + j] / additional_m[i * sq + j];
            additional_m[ii * sq + j] = 0;

            for(int jj = j + 1; jj < sq; jj++) {                            //из остальных эл-ов вычитаем i-ую строку
                additional_m[ii * sq + jj] -= k * additional_m[i * sq + jj];
            }
        }
        i++;
        j++;
    }

    for(int ii = 0; ii < 3; ii++) {
        det *= additional_m[ii * sq + ii];
    }
    free(additional_m);

    return(det);
}