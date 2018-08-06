#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[] )
{
  int a = 1, b = 1, c = 0, d=0;
  FILE *fPtr;
  char c1[20], c2[20], c3[20]; 
     
  fPtr = fopen("test.txt", "r");
  if (!fPtr) {
    printf("fail\n");
    exit(1);
  }
#if 0
  // txpwrindex for core{0...3}: 52 47 0 0
  //fscanf(fPtr, "txpwrindex for core{0...3}: %d %d 0 0", &a, &b);
  fscanf(fPtr, "%d,%d", &a, &b);
  // 10,10
#endif

  fread(c1, sizeof(c1),1,fPtr);
  fclose(fPtr);
  //sscanf(c1, "%d,%d", &a, &b);
  //printf("a: %d, b %d\n", a, b);

  sscanf(c1, "%dd", &a);
  printf("a: %d\n", a);

  printf("%d, argv[0]:%s\n", argc, argv[0]);
	return 0;
}