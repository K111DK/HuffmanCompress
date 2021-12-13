#include "BasicStruct.h"
#include "utils.h"
#include "Compress.h"
int main() {
char *a="a";
char *b="b";
char *c="c";
char *d="abc";
char *f= BiChConverse(d,1);
    printf("%s--hex:%x\n",f,*f);
    f= BiChConverse(f,0);
    printf("%s--hex:%x\n",f,*f);
    return 0;
}
