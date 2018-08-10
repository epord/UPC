#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

int main(){
  bmpInfoHeader *h;
  h = (bmpInfoHeader*) malloc(sizeof (bmpInfoHeader));
  char* img = LoadBMP("alpes_BN.bmp", h);

  DisplayInfo("alpes_BN.bmp", h);

  SaveBMP("alpes2.bmp", h, img);
}
