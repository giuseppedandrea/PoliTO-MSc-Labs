#define N 10

extern unsigned int variance(unsigned int* V, unsigned int n);

int main(void) {
    unsigned int V[N] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    volatile unsigned int r;
    
    r = variance(V, N);
    
    while(1);
}
