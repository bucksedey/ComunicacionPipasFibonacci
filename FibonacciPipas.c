/*
Instrucciones:
crear un programa que analice la suma de 0 a N de una serie de fibonacci en el proceso hijo 
del proceso que pide las variables. Se creará un tercer proceso que multiplique la suma y un
cuarto que multiplique el resultado del 3 por 3 y escriba en un archivo el resultado del mismo
*/
#include <unistd.h> // Constantes simbólicas
#include <sys/types.h> // Tipos de datos del sistema primitivos
#include <errno.h> // Errores
#include <stdio.h> // Input / output
#include <sys/wait.h> // Espera por la terminación del proceso
#include <stdlib.h> // Utilerías en general
#include <stdbool.h>

bool isPrimo(int num);
int fibonacci(int n);
int sumFibonacci(int n);

int main()
{
int limPrim = 0, limFib = 0, sumNumPrim = 0, sumFib = 0, pipa1[2], pipa2[2], pipa3[2];
pid_t pidPadre = -1, pid1 = -1, pid2 = -1, pid3 = -1;

pipe(pipa1);
pipe(pipa2);
pipe(pipa3);

pidPadre = getpid();
pid1 = fork();
pid2 = fork();

// Mata al hijo del hijo 1 (nieto) porque no lo usaremos
if (getppid() != pidPadre && getpid() != pidPadre)
    return -1;
else if (pidPadre == getpid()) // Proceso 1
{
    // Obtener suma de primos
    printf("\n- Proceso 1-\n");
    printf("Ingrese el limite de los numeros primos: ");
    scanf("%d", &limPrim);
    for (int i = 1; i <= limPrim; i++)
        sumNumPrim += isPrimo(i) ? i : 0;
    printf("La suma de primos es: %d\n", sumNumPrim);

    //Cerramos lectura
    close(pipa1[0]);
    // Enviamos al proceso 2
    write(pipa1[1], &sumNumPrim, sizeof(int));
    printf("Enviado al proceso 2\n");

    // Obtener suma de fibonacci
    printf("\nIngrese el limite de la serie de fibonacci: ");
    scanf("%d", &limFib);
    sumFib = sumFibonacci(limFib);
    printf("La suma de fibonacci es: %d\n", sumFib);

    //Cerramos lectura
    close(pipa3[0]);
    // Enviamos al proceso 3
    write(pipa3[1], &sumFib, sizeof(int));
    printf("Enviado al proceso 3\n\n");
}
else // Hijos
{
    // Proceso 2
    if (pid1 == 0)
    {
        int valorRecibidoP1 = 0;
        read(pipa1[0], &valorRecibidoP1, sizeof(int));
        printf("\n- Proceso 2 -\n");
        printf("La suma de primos recibida es: %d\n", valorRecibidoP1);
        int resultadoFib = valorRecibidoP1 * valorRecibidoP1;
        printf("El resultado final es: %d\n", resultadoFib);

        //Cerramos escritura
        close(pipa2[1]);
        // Enviamos al proceso 3
        write(pipa2[0], &resultadoFib, sizeof(int));
        printf("Enviado al proceso 3\n\n");
    }
    else // Proceso 3
    {
        int valorRecibidoP3 = 0;
read(pipa3[0], &valorRecibidoP3, sizeof(int));
printf("\n- Proceso 3 -\n");
printf("La suma de fibonacci recibida es: %d\n", valorRecibidoP3);
int resultadoSuma = valorRecibidoP3 + fibonacci(10);
printf("El resultado final es: %d\n", resultadoSuma);
}
}

return 0;
}

// Función que devuelve true si el número es primo
bool isPrimo(int num)
{
if (num < 2)
return false;
for (int i = 2; i <= num / 2; i++)
{
if (num % i == 0)
return false;
}
return true;
}

// Función que devuelve el n-ésimo término de la serie de Fibonacci
int fibonacci(int n)
{
if (n <= 1)
return n;
return fibonacci(n - 1) + fibonacci(n - 2);
}

// Función que devuelve la suma de los n primeros términos de la serie de Fibonacci
int sumFibonacci(int n)
{
int sum = 0;
for (int i = 0; i <= n; i++)
sum += fibonacci(i);
return sum;
}
