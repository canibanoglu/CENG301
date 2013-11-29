#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

typedef struct{
    char id[8];
    char fname[30];
    char lname[30];
    double midterm;
    double final;
    double hw1;
    double hw2;
    double hw3;
}Student;

Student dummy, *array = NULL;
int arrayCapacity = 0, itemCount = 0;
double midtermWeight, finalWeight, hwWeight;

int main(int argc, char **argv) {
    void readInfo();
    void courseGrade();
    double studentScore(double, double, double, double, double);
    double avgScore();
    double stdDev(double, int);
    readInfo();
    if (argc != 4) {
        printf("You have to supply three arguments to this program.");
    }
    else {
        midtermWeight = atof(argv[1]) / 100;
        finalWeight = atof(argv[2]) / 100;
        hwWeight = atof(argv[3]) / 100;
        if ((midtermWeight + finalWeight + hwWeight) != 1) {
            printf("The given percentages do not add up to 1!\nGoodbye!");
            exit(-1);
        }
    }
    printf("Midterm: %lf, Final: %lf, HWs: %lf", midtermWeight, finalWeight, hwWeight);
    printf("\nNumber of students: %d", itemCount);

    double test;
    dummy = array[0];
    test = studentScore(dummy.midterm, dummy.final, dummy.hw1, dummy.hw2, dummy.hw3);
    printf("\n%lf", test);
    printf("\n%lf", avgScore());
    test = avgScore();
    printf("\nStdDev: %lf", stdDev(test, itemCount));
    courseGrade();
    free(array);
}

void readInfo() {
    FILE *file = fopen("scores.txt", "r");
    while(fscanf(file, "%s %s %s %lf %lf %lf %lf %lf", dummy.id, dummy.fname, dummy.lname, &dummy.midterm,  &dummy.final, &dummy.hw1, &dummy.hw2, &dummy.hw3) == 8) {
        if (itemCount >= arrayCapacity) {
            const int MIN_CAPACITY = 10;
            const double GROWTH_RATE = 1.5;
            int newCapacity = arrayCapacity < MIN_CAPACITY ? MIN_CAPACITY : (int) (arrayCapacity * GROWTH_RATE);
            Student* tmp = realloc(array, newCapacity * sizeof(Student));

            if (tmp == NULL) {
                printf("Couldn't allocate memory to the students array. How many students do you have?\n");
            }
            else {
                array = tmp;
                arrayCapacity = newCapacity;
            }
        }
        if (itemCount >= arrayCapacity) {
                printf("Can't increase the size of array and can't add a new item.\n");
                break;
        }
        else {
            array[itemCount++] = dummy;
        }
    }
}

double studentScore(double midterm, double final, double hw1, double hw2, double hw3) {
    return (midterm * midtermWeight) + (final * finalWeight) + ((hw1 + hw2 + hw3) * hwWeight / 3);
}

double avgScore() {
    double total = 0;
    for (int j = 0; j < itemCount; j++) {
        dummy = array[j];
        total += studentScore(dummy.midterm, dummy.final, dummy.hw1, dummy.hw2, dummy.hw3);
    }
    return total / itemCount;
}

double stdDev(double avg, int numOfStudents) {
    double total = 0, term;
    for (int j = 0; j < numOfStudents; j++) {
        dummy = array[j];
        term = studentScore(dummy.midterm, dummy.final, dummy.hw1, dummy.hw2, dummy.hw3) - avg;
        total += term * term;
    }
    return (sqrt(total/numOfStudents));
}

void courseGrade() {
    int aboveSixty = 0;
    double averageScore, standardDeviation, stdScore; 
    char letter[3]; //3 because we need the null character at the end! Without this I get Abort trap: 6 on OS X 10.9 (buffer overflow)
    FILE *fout = fopen("1551764_allScores.txt", "w");
    for (int j = 0; j < itemCount; j++) {
        dummy = array[j];
        stdScore = studentScore(dummy.midterm, dummy.final, dummy.hw1, dummy.hw2, dummy.hw3);
        if ( 90 <= stdScore)  {strcpy(letter, "AA");}
        else if (85 <= stdScore)  {strcpy(letter, "BA");}
        else if (80 <= stdScore)  {strcpy(letter, "BB");}
        else if (75 <= stdScore)  {strcpy(letter, "CB");}
        else if (70 <= stdScore)  {strcpy(letter, "CC");}
        else if (65 <= stdScore)  {strcpy(letter, "DC");}
        else if (60 <= stdScore)  {strcpy(letter, "DD");}
        else if (50 <= stdScore)  {strcpy(letter, "FD");}
        else  {strcpy(letter, "FF");}
        if (stdScore > 60) aboveSixty++;
        
        fprintf(fout, "%s %s %s %3.1lf %s\n", dummy.id, dummy.fname, dummy.lname, stdScore, letter);
    }
    averageScore = avgScore();
    standardDeviation = stdDev(averageScore, itemCount);
    fprintf(fout, "Average: %4.2lf\nStandard Deviation: %4.2lf\n(>60): %d", averageScore, standardDeviation, aboveSixty);
}
