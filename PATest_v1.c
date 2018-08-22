#if 1 // __MSTC__, richardtest, C1100Z PA issue.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// test1

// add test1 function
void test1()
{
 return ;
}

// add test2 function
void test2()
{
 return ;
}

// add test3 function
void test3()
{
 return ;
}

void doPACheck(void)
{
   FILE *wifiPAFile = NULL;
   int PA0Thr, PA1Thr, PA0Val = 0, PA1Val = 0, PANone = 0;

   wifiPAFile = fopen("/app/WiFiPAThreshold", "r");
   if(wifiPAFile != NULL) {
      fscanf(wifiPAFile, "%d %d", &PA0Thr, &PA1Thr);
      fclose(wifiPAFile);
      wifiPAFile = NULL;
   } else {
      system("echo \"5 5\" > /app/WiFiPAThreshold");
      PA0Thr = PA1Thr = 5;
   }
   printf("[Debug] PA0Thr:%d, PA1Thr:%d\n", PA0Thr, PA1Thr);

   system("wl phy_txpwrindex > /tmp/wifiThrFile");
   wifiPAFile = fopen("/tmp/wifiThrFile", "r");
   if(wifiPAFile != NULL) {
      fscanf(wifiPAFile, "txpwrindex for core{0...3}: %d %d %d %d", &PA0Val, &PA1Val, &PANone, &PANone);
      fclose(wifiPAFile);

      printf("[Debug] PA0Val:%d, PA1Val:%d\n", PA0Val, PA1Val);

      system("wl txchain 0x3");
      system("wl phyreg 0x91 0x0000");
      system("wl phyreg 0x92 0x0000");

      if((PA0Val <= PA0Thr) && (PA1Val > PA1Thr)){
         printf("[Debug] Disable ch1\n");
         system("wl txchain 0x2");
         system("wl phyreg 0x91 0x1000");
      } else if((PA1Val <= PA1Thr) && (PA0Val > PA0Thr)){
         printf("[Debug] Disable ch2\n");
         system("wl txchain 0x1");
         system("wl phyreg 0x92 0x1000");
      } else if((PA0Val <= PA0Thr) && (PA1Val <= PA1Thr)){
         printf("[Debug] Disable ch1 and ch2\n");
         system("wl phyreg 0x91 0x1000");
         system("wl phyreg 0x92 0x1000");
      }
   }
}

int main(int argc, char *argv[])
{
   int wTime = 0;
   FILE *wTimeFile = NULL;

   wTimeFile = fopen("/app/WiFiPAWaitTime", "r");
   if(wTimeFile != NULL) {
      fscanf(wTimeFile, "%d", &wTime);
      fclose(wTimeFile);
      wTimeFile = NULL;
   } else {
      system("echo \"110\" > /app/WiFiPAWaitTime");
      wTime = 110;
   }

   if((wTime - 68) <= 0) {
      wTime = 17;
   } else {
      wTime -= 68;
   }

   sleep(wTime);

   doPACheck();

   // bug fix

   return 0;
}
#endif
