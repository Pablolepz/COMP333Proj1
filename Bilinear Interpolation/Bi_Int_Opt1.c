#include <stdint.h>
#include <stdio.h>
#include "omp.h"

typedef struct {
    uint32_t *pixels;
    unsigned int w;
    unsigned int h;
} image_t;

#define getByte(value, n) (value >> (n*8) & 0xFF)

uint32_t getpixel(image_t *image, unsigned int x, unsigned int y)
{
    return image->pixels[(y*image->w)+x];
}

float lerp(float s, float e, float t)
{
  return s+(e-s)*t;
}

float blerp(float c00, float c101, float c01, float c11, float tx, float ty)
{
    return lerp(lerp(c00, c10, tx), lerp(c01, c11, tx), ty);
}

void putpixel(image_t *image, unsigned int x, unsigned int y, uint32_t color)
{
    image->pixels[(y*image->w) + x] = color;
}

void scale(image_t *src, image_t *dst, float scalex, float scaley)
{
    int newWidth = (int)src->w*scalex;
    int newHeight= (int)src->h*scaley;
    int x, y;

    //traverse x direction
    #pragma omp for shared(x,y)
    for(x= 0, y=0; y < newHeight; x++)
    {
        //traverse y direction
        if(x > newWidth)
        {
            x = 0; y++;
        }
        float gx = x / (float)(newWidth) * (src->w-1);
        float gy = y / (float)(newHeight) * (src->h-1);
        int gxi = (int)gx;
        int gyi = (int)gy;
        uint32_t result=0;
        uint32_t c00 = getpixel(src, gxi, gyi);
        uint32_t c10 = getpixel(src, gxi+1, gyi);
        uint32_t c01 = getpixel(src, gxi, gyi+1);
        uint32_t c11 = getpixel(src, gxi+1, gyi+1);
        uint8_t i;

        //transformation on pixel, no parallelization possible (?)
        for(i = 0; i < 3; i++)
        {
            //((uint8_t*)&result)[i] = blerp( ((uint8_t*)&c00)[i], ((uint8_t*)&c10)[i], ((uint8_t*)&c01)[i], ((uint8_t*)&c11)[i], gxi - gx, gyi - gy); // this is shady
            result |= (uint8_t)blerp(getByte(c00, i), getByte(c10, i), getByte(c01, i), getByte(c11, i), gx - gxi, gy -gyi) << (8*i);
        }
        putpixel(dst,x, y, result);
    }
}

image_t jpgToImage_t(FILE *ptr)
{
  //this needs to be made to be able to transpose code
  if
  return
}

int main ()
{
  image_t img = NULL;
  image_t img2 = NULL;
  if (FILE *pntr = fopen("pic_1.jpg", "rb"))
  {
    img = jpgToImage_t(pntr);
  }
  if (FILE *pntr2 = fopen("pic_out.jpg", "wb"))
  {
    img2 = jpgToImage_t(pntr2);
  }

//==============================



  #pragma omp parallel
  {
    #pragma omp single
    scale(pntr, pntr2, 1.6f, 1.6f);
    fclose(pntr);
    fclose(pntr2);
  }
  return 1;
}
