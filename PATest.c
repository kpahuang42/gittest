#if 1 // __MSTC__, richardtest, C1100Z PA issue.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void doWlTestMode(void)
{
   system("wl pkteng_stop tx");
   system("wl down");
   system("wl country ALL");
   system("wl mimo_txbw 2");
   system("wl chanspec -c 11 -b 2 -w 20 -s 0");
   system("wl nrate -m 0 -s 1");
   system("wl txchain 3");
   system("wl rxchain 3");
   system("wl up");
   system("wl bi 65535");
   system("wl join WLAN_MCS0H2");
   system("wl ssid \"\"");
   system("wl pkteng_stop tx");
   system("wl txpwr1 -o -q 60");
   system("wl pkteng_stop tx");
   system("wl phy_forcecal 1");
   system("wl pkteng_start 12:34:56:78:9A:BC tx 30 1000 0");
   sleep(1);
}

void doPACheck(void)
{
   FILE *wifiPAFile = NULL;
   unsigned char chkMode = 0;
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

      if((PA0Val <= PA0Thr) && (PA1Val > PA1Thr)){
         chkMode = 1;
      } else if((PA1Val <= PA1Thr) && (PA0Val > PA0Thr)){
         chkMode = 2;
      } else if((PA0Val <= PA0Thr) && (PA1Val <= PA1Thr)){
         chkMode = 3;
      }

      // restart WiFi
      system("wl pkteng_stop tx");
      system("wl mimo_txbw -1");
      system("wl txpwr1 -1");
      system("nvram commit");
      sleep(15);

      if(chkMode == 1){
         printf("[Debug] Disable ch1\n");
         system("wl txchain 0x2");
         system("wl phyreg 0x91 0x1000");
      } else if(chkMode == 2){
         printf("[Debug] Disable ch2\n");
         system("wl txchain 0x1");
         system("wl phyreg 0x92 0x1000");
      } else if(chkMode == 3){
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

   doWlTestMode();
   doPACheck();

   return 0;
}
#endif