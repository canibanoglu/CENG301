/* Author: Can Ibanoglu
 * ID: 1557164
 * Requirements:
 * In order to use the plotting functionality, you will need gnuplot and X11
 * windowing system. 
 * Works on UNIX-like systems as long as the two requirements are met.
 * Don't know about Windows, but Windows users should seriously consider an OS
 * change before worrying about plotting and the like.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


double bubbleResults[10], insertionResults[10], quicksortResults[10];
int first=0, second=0, third=0;

double * createRandomNumbers() {
    int n;
    static double numbersArray[100000];
    srand(time(NULL));
    for (int i=0; i < 100000; i++) {
        n = rand();
        numbersArray[i] = n;
    }
    first = 1;
    return numbersArray;
}

void bubblesort(double *array) {
    double own[100000];
    for (int k=1; k<=10; k++) {
        // Copy the first k * 10000 elements into our local copy
        // This is done so that all of the test algorithms can work on the 
        // same array.
        // I must say that this is a very stupid implementation, I can't believe
        // we are forced to work like this.

        int n=k*10000;
        clock_t time = clock();
        for (int m=0; m<n; m++) {
            own[m] = array[m];
        }

        // This is where sorting starts
        time = clock();
        for (int i=1; i<n;i++) {
            for (int j=0; j < n - 1; j++) {
                if (own[j] > own[j+1]) {
                    double temp=own[j];
                    own[j] = own[j+1];
                    own[j+1] = temp;
                }
            }
        }
        bubbleResults[k-1] = (clock() - time) / (double)CLOCKS_PER_SEC;
        // This is where sorting ends
    }
}

void insertionsort(double *array) {
    double own[100000];
    int j;
    
    for (int k=1; k<=10; k++) {
        // We are copying the first k * 10000 elements into our local copy
        int n = k * 10000;
        clock_t time = clock();
        for (int m=0; m<n; m++) {
            own[m] = array[m];
        }

        // This is where sorting starts
        time = clock();
        for (int i=1; i<n; i++) {
            j = i;
            while (j>0 && own[j] < own[j-1]) {
                double temp = own[j];
                own[j] = own[j-1];
                own[j-1] = temp;

                j--;
            }
        }
        insertionResults[k-1] = (clock() - time) / (double)CLOCKS_PER_SEC;
        // this is where sorting ends
    }
}

void swap(double* a, double* b) {
    double t = *a;
    *a = *b;
    *b = t;
}

void quicksort(double *array) {
    double own[100000];
    for (int k=1; k <= 10; k++) {
        // Copy the first k * 10000 elements into our local copy
        int n = k * 10000;
        clock_t time = clock();
        for (int m=0; m<n; m++) {
            own[m] = array[m];
        }

        // Sorting starts here
        time = clock();

        int l = 0, h = n-1;

        int stack[h-l+1];
        int top = -1;

        stack[++top] = l;
        stack[++top] = h;

        while (top >= 0) {
            h = stack[top--];
            l = stack[top--];

            int x = own[h];
            int i = (l-1);

            for (int j=l; j <= h-1; j++) {
                if (own[j] <= x) {
                    i++;
                    swap(&own[i], &own[j]);
                }
            }
            swap(&own[i+1], &own[h]);
            int p = (i+1);

            if (p-1>l) {
                stack[++top] = l;
                stack[++top] = p-1;
            }

            if (p+1<h) {
                stack[++top] = p + 1;
                stack[++top] = h;
            }
        }
        quicksortResults[k-1] = (clock() - time) / (double)CLOCKS_PER_SEC;
        // Sorting ends here
    }
}

double (*createMatrix(double *array))[3] {
    static double matrix[10][3];
    quicksort(array);
    insertionsort(array);
    bubblesort(array);

    // Create the matrix using the results

    for (int i=0; i<10; i++) {
        matrix[i][0] = bubbleResults[i];
        matrix[i][1] = insertionResults[i];
        matrix[i][2] = quicksortResults[i];
    }
    second = 1;
    return matrix;
}

void createFile(double (*matrix)[3]) {
    FILE *f = fopen("profile.txt", "w");
    
    fprintf(f, "\n %15s %20s %20s %20s", "N", "Bubble Sort (ms)", "Insertion Sort (ms)", "Quicksort (ms)");
    for (int i=0; i<10; i++) {
        fprintf(f, "\n %15d %20.2lf %20.2lf %20.2lf", (i+1)*10000, matrix[i][0], matrix[i][1], matrix[i][2]);
    }
    fclose(f);
    third = 1;
}

void exitProgram() {
    printf("Have a nice day/night!\n");
    exit(0);
}

void plotFigure(double (*matrix)[3]) {
    FILE *gnuquick = fopen("gnuquicksort.txt", "w");
    FILE *gnuinsert = fopen("gnuinsertion.txt", "w");
    FILE *gnububble = fopen("gnububble.txt", "w");
    for (int i=0; i<10; i++) {
        fprintf(gnububble, "%d %lf\n", (i+1)*10000, matrix[i][0]);
        fprintf(gnuinsert, "%d %lf\n", (i+1)*10000, matrix[i][1]);
        fprintf(gnuquick, "%d %lf\n", (i+1)*10000, matrix[i][2]);
    }
    char *gnu_commands[] = {"set xlabel \"Array length n\"",
                            "set ylabel \"Runtime (sec)\"",
                            "plot \"gnububble.txt\" with linespoints, \"gnuinsertion.txt\" with linespoints, \"gnuquicksort.txt\" with linespoints"};
    FILE *gnu_pipe = popen("gnuplot -persist", "w");
    for (int i=0; i<3; i++) {
        fprintf(gnu_pipe, "%s \n", gnu_commands[i]);
    }
    fclose(gnu_pipe);
    fclose(gnuquick);
    fclose(gnuinsert);
    fclose(gnububble);
    exitProgram();
}

int main() {
    double *numbersArray;
    double (*matrix)[3];
    int choice;
    system("clear");

    while (1) {
        printf("1. Create 100000 random doubles.\n");
        printf("2. Sort the numbers with 3 algorithms and save execution time in a matrix.\n");
        printf("3. Save the data in the matrix into a file.\n");
        printf("4. Plot the execution times versus n.\n");
        printf("5. Exit the program.\n");
        scanf("%d", &choice);
        if (choice==1) {
            numbersArray = createRandomNumbers();
        }
        else if (choice == 2 && first == 0) {
            printf("You can't select choice 2 or choice 3 before choice 1.\n");
        }
        else if (choice == 2 && first == 1) {
            matrix = createMatrix(numbersArray);
        }
        else if (choice == 3 && first == 0) {
            printf("You can't select choice 2 or choice 3 before choice 1.\n");
        }
        else if (choice == 3 && second == 0) {
            printf("You can't select choice 3 before choice 2.\n");
        }
        else if (choice == 3) {
            createFile(matrix);
        }
        else if (choice == 4 && first == 0) {
            printf("You can't select choice 4 before choice 2.\n");
        }
        else if (choice == 4 && second == 0) {
            printf("You can't select choice 4 before choice 2.\n");
        }
        else if (choice == 4 && third == 0) {
            printf("You can't select choice 4 before choice 2.\n");
        }
        else if (choice == 4) {
            plotFigure(matrix);
        }
        else if (choice == 5) {
            exitProgram();
        }
    }
}
