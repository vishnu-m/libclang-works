
#include <stdio.h>

// Declaration of union is same as structures
union test {
    int x, y;
};

int po;
const va=1;
struct yobabe {
  int y;
};

typedef union test Test;

enum State {Working = 1, Failed = 0};

int main(int argc)
{
    // A union variable t
    union test t;

    t.x = 2; // t.y also gets value 2
    printf("After making x = 2:\n x = %d, y = %d\n\n",
           t.x, t.y);

    t.y = 10; // t.x is also updated to 10
    printf("After making y = 10:\n x = %d, y = %d\n\n",
           t.x, t.y);
    return 0;
}
