#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

typedef struct
{
    int atomic_number;
    char name[20];
    int neutrons;
} Element;

/*
Función para leer datos de un archivo y almacenarlos en un array de elementos
*/
int leer_datos(const char *filename, Element elements[], int max_elements)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("No se pudo abrir el archivo");
        return 1;
    }
    int index = 0;
    while (fscanf(file, "%d,%[^,],%d", &elements[index].atomic_number, &elements[index].name[0], &elements[index].neutrons) == 3)
    {
        index++;
        if (index >= max_elements)
            break;
    }
    fclose(file);
    return index;
}

/*
Resuelve la sumatoria que calcula el logaritmo de y
y es el numero de neutrones
*/
double sumatoria_ln_y(int n, Element elements[])
{
    double sum_ln_y = 0;
    for (int i = 0; i < n; i++)
    {
        int elemento = elements[i].neutrons;
        if (elemento != 0)
        {
            sum_ln_y += log(elemento);
        }
    }
    return sum_ln_y;
}

/*
Resuelve la sumatoria que calcula el logaritmo de x
x es el numero atomico
*/
double sumatoria_ln_x(int n, Element elements[])
{
    double sum_ln_x = 0;
    for (int i = 0; i < n; i++)
    {
        int elemento = elements[i].atomic_number;
        if (elemento != 0)
        {
            sum_ln_x += log(elemento);
        }
    }
    return sum_ln_x;
}

/*
Resuelve la sumatoria que calcula el (logaritmo de x) al cuadrado
x es el numero atomico
*/
double sumatoria_ln_x_cuadrado(int n, Element elements[])
{
    double sum_ln_x = 0;
    for (int i = 0; i < n; i++)
    {
        int elemento = elements[i].atomic_number;
        if (elemento != 0)
        {
            sum_ln_x += (log(elemento) * log(elemento));
        }
    }
    return sum_ln_x;
}

/*
Resuelve la sumatoria que calcula el (logaritmo de x) * (logaritmo de y)
x es el numero atomico
y es el numero de neutrones
*/
double sumatoria_ln_x_ln_y(int n, Element elements[])
{
    double sum_ln_x = 0;
    for (int i = 0; i < n; i++)
    {
        int elemento = elements[i].atomic_number;
        int elemento2 = elements[i].neutrons;
        if (elemento != 0 && elemento2 != 0)
        {
            sum_ln_x += (log(elemento) * log(elemento2));
        }
    }
    return sum_ln_x;
}

/*
Función para crear un archivo CSV para graficar los datos con otra herramienta
*/
void crear_archivo_csv(const char *filename, Element elements[], int num_elements, double a, double b)
{
    FILE *file = fopen(filename, "w");
    if (!file)
    {
        perror("No se pudo crear el archivo CSV");
        exit(1);
    }

    fprintf(file, "Numero Atomico,Neutrones Reales,Neutrones Predichos\n");
    for (int i = 0; i < num_elements; i++)
    {
        double predicted_neutrons = a * pow(elements[i].atomic_number, b);
        fprintf(file, "%d,%d,%f\n", elements[i].atomic_number, elements[i].neutrons, predicted_neutrons);
    }

    fclose(file);
}

/*
Función para obtener un determinante de una matrix 2x2
*/
double calcularDeterminante(double matriz[2][2]) {
    return matriz[0][0] * matriz[1][1] - matriz[0][1] * matriz[1][0];
}


/*
Función principal que organiza la ejecución del programa
*/
int main()
{
    Element elements[100];
    int num_elements = leer_datos("elements.txt", elements, 100);
    if (num_elements == 0)
    {
        fprintf(stderr, "Error al leer los datos del archivo.\n");
        return 1;
    }

    double sum_ln_x = sumatoria_ln_x(num_elements, elements); 
    double sum_ln_y = sumatoria_ln_y(num_elements, elements);
    double sum_ln_x_squared = sumatoria_ln_x_cuadrado(num_elements, elements);
    double sum_ln_x_ln_y = sumatoria_ln_x_ln_y(num_elements, elements);

    if (num_elements == 0 || sum_ln_x == 0 || sum_ln_y == 0 || sum_ln_x_squared == 0 || sum_ln_x_ln_y == 0)
    {
        fprintf(stderr, "Error en el cálculo de sumatorias.\n");
        return 1;
    }

    double n = num_elements;
    double denominator = n * sum_ln_x_squared - sum_ln_x * sum_ln_x;
    printf("Denominador = %f\n", denominator);
    if (denominator == 0)
    {
        fprintf(stderr, "Error: el denominador es cero.\n");
        return 1;
    }
    /*
    double ln_a = (sum_ln_y * sum_ln_x_squared - sum_ln_x * sum_ln_x_ln_y) / denominator;
    printf("ln(a): %f\n", ln_a);

    double b = (n * sum_ln_x_ln_y - sum_ln_x * sum_ln_y) / denominator;
    printf("b: %f\n", b);

    double a = exp(ln_a);
    printf("a: %f\n", a);
    */

    /*Configuracion de las matrices*/

    /*Denominador*/
    double matrizPrincipal[2][2] = {
        {n, sum_ln_x},
        {sum_ln_x, sum_ln_x_squared}
    };

    /*ln(a)*/
    double matrizA[2][2] = {
        {sum_ln_y, sum_ln_x},
        {sum_ln_x_ln_y, sum_ln_x_squared}
    };

    /*b*/
    double matrizB[2][2] = {
        {n, sum_ln_x},
        {sum_ln_y, sum_ln_x_ln_y}
    };

    

    /*Resolvemos el sistema para a y b*/
    float det_principal = calcularDeterminante(matrizPrincipal);
    float det_a = calcularDeterminante(matrizA);
    float det_b = calcularDeterminante(matrizB);
    
    /*Regla de cramer*/
    double a = exp(det_a/det_principal);
    double b = det_b/det_principal;



    printf("Parámetros de ajuste: a = %f, b = %f\n\n", a, b);

    printf("%-4s %-20s %-8s %-15s %-15s %-15s\n", "No.", "Nombre", "Atómico", "Neutrones Reales", "Neutrones Predichos", "Redondeo al más cercano", "Redondeo hacia cero");
    printf("------------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < num_elements; i++)
    {
        double predicted_neutrons = a * pow(elements[i].atomic_number, b);
        int rounded_nearest = round(predicted_neutrons);
        int rounded_down = floor(predicted_neutrons);

        printf("%-4d %-20s %-8d %-15d %-15.2f %-15d %-15d\n",
               i + 1, elements[i].name, elements[i].atomic_number, elements[i].neutrons, predicted_neutrons, rounded_nearest, rounded_down);
    }

    crear_archivo_csv("datos.csv", elements, num_elements, a, b);

    return 0;
}
