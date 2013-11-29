#include <stdio.h>

void b(int s[]);
void f(int *x);

int main(void) {
    int a[4] = {-5, 3, 4, 2};
    int i;

    b(a);

    for (i=0; i<4; i++) 
        printf("%3d", a[i]);
    printf("\n");

    for (i=0; i<4; i++)
        a[i] += i;

    for (i=0; i<4; ++i)
        f(&a[i]);

    for (i=0; i<4; i++)
        printf("%3d", a[i]);
    return 0;
}

void b(int s[]) {
    int t;
    for (t=0; t<4; t++) {
        if(s[t] % 2 == 0)
            f(&s[t]);
        else
            s[t] += t;
    }
}

void f(int *x) {
    int i = 3;
    *x += i;
}
