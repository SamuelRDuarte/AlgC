#include <stdio.h>

int main(void){
  int count = 0;
  int potencias[10];
  potencias[0]=0;
  potencias[1]=0;

  for (int i = 2; i < 10; i++) {
    potencias[i]=i*i*i;
    count+=3;
  }

  for (int i = 1; i <10; i++) {
    for (int j = 0; j < 10; j++) {
      for(int k = 0;k<10;k++){
        int num = i*100+j*10+k;
        count+=2;
        int sum = potencias[i]+potencias[j]+potencias[k];
        if(sum == num)  printf("%d\n",num);
      }
    }
  }
  printf("Number of multiplications: %d\n", count);
  return 0;
}
