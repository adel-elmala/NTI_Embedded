
#include "Calculator.h"
#if 1
// right implementation
int (*Arr_ptr[4])(int, int) = {add, sub, mul, divide};
void main()
{
  int op, a, b;
  printf("Please Enter operation:\n");
  printf("1-Add\n2-Sub\n3-Mul\n4-Divide\n");
  scanf("%d", &op);
  printf("Please enter 2 numbers\n");
  scanf("%d %d", &a, &b);
  if (op >= 1 && op <= 4)
    printf("res = %d", Arr_ptr[op - 1](a, b));
}
#endif

#if 0
// faulty Implementation
int (Arr_ptr[4])={add,sub,mul ,divide};// error 
void main()
{
    int op, a, b;
    printf("Please Enter operation:\n");
    printf("1-Add\n2-Sub\n3-Mul\n4-Divide\n");
    scanf("%d",&op);
    printf("Please enter 2 numbers\n");
    scanf("%d %d",&a,&b);
    if(op >= 1 && op <= 4)
      printf("res = %d",Arr_ptr[op](a,b));
}
#endif // 1
