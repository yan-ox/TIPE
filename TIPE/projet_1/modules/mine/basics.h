// basics.h 

#ifndef _BASIC_H
#define _BASIC_H

char* add_string(char* a, char* b); //done
float dist(int x1, int y1, int x2, int y2); //done
void coor(int i, int width, int point[2]);  //done
int ind(int x, int y, int width);   //done
int minf(float* arr, int n);    //done
int maxf(float* arr, int n);    //done
int rgb_to_hex(int* RGB);   //In StackOverflow we trust
void hex_to_rgb(int* RGB, int hex); //In StackOverflow we trust
#endif