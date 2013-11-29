#include <stdio.h>

void func1(int *p, int r);
void func2(int t);

int m=5, n=10;
int main(void) {
    int i=2, j=3;
    m++;
    func1(&i, j);

    printf("%3d%3d%3d%3d\n", i,j,m,n);

    n--;
    func1(&j, i);
    return(0);
}

void func1(int *p, int r) {
    int i = 1, n;
    i *= 2;
    n += i;
    m++;
    *p = r + 3;
    func2(r);

    printf("%3d%3d%3d%3d\n", *p, i,m,n);
}

void func2(int t) {
    int i, m;
    m = t+ 2;
    t++;
    i = t;
    n--;

    printf("%3d%3d%3d%3d\n", i,t,m,n);
}
