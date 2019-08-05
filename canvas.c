#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "bmp.h"
#include "canvas.h"
#include <math.h>

#define NUMBER_OF_SPHERES 5
#define NUMBER_OF_SOLS 3
static sphere_t *sphere[NUMBER_OF_SPHERES];
static point_t *sol[NUMBER_OF_SOLS];
static unsigned char sphereCount = 0;
static unsigned char solCount = 0;

void putpixel(int x, int y, unsigned char red, unsigned char green, unsigned char blue);

void AddSphere(float x, float y, float z, float r, unsigned char red, unsigned char green, unsigned char blue)
{
  static char i = 0;

  if(i<NUMBER_OF_SPHERES)
  {
    sphere[i] = (sphere_t*) malloc(sizeof(sphere_t));
    sphere[i]->center.x = x;
    sphere[i]->center.y = y;
    sphere[i]->center.z = z;
    sphere[i]->r  = r;
    sphere[i]->color.red = red;
    sphere[i]->color.green = green;
    sphere[i]->color.blue = blue;
    sphereCount = ++i;
  }
  else
  {
    printf("Too many spheres!\n");
  }
 }

 void AddSol(float x, float y, float z)
 {
  static char j = 0;
  if(j<NUMBER_OF_SOLS)
  {
    sol[j] = (point_t*)malloc(sizeof(point_t));
    sol[j]->x = x;
    sol[j]->y = y;
    sol[j]->z = z;
    solCount = ++j;
  }
  else
  {
    printf("Too many sols\n");
  }
 }

int DrawSphere(void)
{
  int i, j, k, m;
  line_t line;
  line.p1.x = 1.732f; //CANVAS_WIDTH/2 Point of view
  line.p1.y = 1.299f; //CANVAS_HEIGHT/2 Point of view
  line.p1.z = 0.0f;
  line.p2.z = 1.0f;
  vector_t ray;
  vector_t spherenormal;
  point_t intersection; //pov sphere point of intersection
  float light[2] = {0.0, 0.0};
  float specularLight = 0.0;
  float diffuseLight = 0.0;
  float light2[2] = {0.0, 0.0};
  for(i=0; i<PIXEL_HEIGHT; i++)
  {
    for(j=0; j<PIXEL_WIDTH; j++)
    {
      line.p2.x = ((float)j+0.5)*CANVAS_WIDTH/PIXEL_WIDTH;
      line.p2.y =  ((float)i+0.5)*CANVAS_HEIGHT/PIXEL_HEIGHT;
    
      for(k=0; k < sphereCount; k++)
      { 
        if(LineIntersectsSphere(&line, sphere[k], &intersection))
        {
         vector_t sphereUnitNormal;
         vector_t vtmp;
         vector_t reflection;
         spherenormal = pointstovector(&sphere[k]->center, &intersection);
         
          for(m = 0; m < solCount; m++)
          {
            ray = pointstovector(&intersection, sol[m]);
            //diffuse reflection calculation
            light[m] = cosalpha(ray, spherenormal);
            if(light[m] <= 0)
            {
              light[m] = 0;
              light2[m] = 0;
            } 
          
            if(light[m] > 0)
            {
            //  specular reflection calculation
              sphereUnitNormal = createunitvector(&spherenormal);
              reflection = vectorminusvector(scalartimesvector(dotproduct(scalartimesvector(2.0, ray), sphereUnitNormal),sphereUnitNormal), ray);
            
              light2[m] = cosalpha( pointstovector(&intersection, &line.p2) , reflection);
            }
            if(light2[m] <= 0)
            {
              light2[m] = 0;
            }
          
            light2[m] = pow(light2[m], 40);
          }
          
          for(diffuseLight=0, specularLight=0, m=0; m<solCount; m++)
          {
            diffuseLight = diffuseLight+ light[m];
            specularLight = specularLight+ light2[m];
          }
          if(diffuseLight > 1.0)
          {
            diffuseLight = 1.0;
          }
          if(specularLight > 1.0)
          {
            specularLight = 1.0;
          }
          float dred = sphere[k]->color.red*diffuseLight; 
          float dgreen = sphere[k]->color.green*diffuseLight;
          float dblue = sphere[k]->color.blue*diffuseLight;
          float red = dred + (255-dred)*specularLight;
          float green = dgreen + (255-dgreen)*specularLight;
          float blue = dblue + (255-dblue)*specularLight;
          red =  red>255 ? 255:red;
          green = green>255 ? 255:green;
          blue = blue>255 ? 255:blue;
          //display
        
         // putpixel(image, j, PIXEL_HEIGHT-i-1, sphere[k]->color.red*light, sphere[k]->color.green*light, sphere[k]->color.blue*light);
        // putpixel(image, j, PIXEL_HEIGHT-i-1,255.0*light2, 255.0*light2, 255.0*light2);
           putpixel(j, PIXEL_HEIGHT-i-1, red, green, blue);
           break;    
        }
        else
        {
          putpixel(j, PIXEL_HEIGHT-i-1, 0xFF, 0xFF, 0xFF);
        }
      }
    }
  }
}

void putpixel(int x, int y, unsigned char red, unsigned char green,unsigned char blue)
{
  PixelData_t* pixeldata = GetPixelData();
  pixeldata->data[y][x][0] = blue;
  pixeldata->data[y][x][1] = green;
  pixeldata->data[y][x][2] = red;
}
