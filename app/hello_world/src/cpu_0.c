#include "system.h"
#include <stdio.h>

int fac5, fac10, fac15;

long factorial(int n)
{
  int c;
  long result = 1;
 
  for (c = 1; c <= n; c++)
    result = result * c;
 
  return result;
}

int main()
{ 
  printf("Hello from Nios II!\n");
  
  fac5 = factorial(5);
  fac10 = factorial(10);
  fac15 = factorial(15);
  
  printf("factorial(5) = %x (hexadecimal)\n",fac5);
  printf("factorial(10) = %x (hexadecimal)\n",fac10);
  printf("factorial(15) = %x (hexadecimal)\n",fac15);

  while (1);

  return 0;
}
