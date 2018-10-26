#include <stdio.h>
#include <float.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "../Sort/sorts.h"
#include "../Utils/Point.h"
 

int compareX(Point *a, Point *b)
{
    return (a->x - b->x);
}

int compareY(Point *a, Point *b)
{
    return (a->y - b->y);
}
 

float dist(Point *p1, Point *p2)
{
    return sqrt( (p1->x - p2->x)*(p1->x - p2->x) +
                 (p1->y - p2->y)*(p1->y - p2->y)
               );
}
 

float bruteForce(Point *P[], int bgn, int end, char* out)
{
    float min = FLT_MAX;
    for (int i = bgn; i < end; ++i){
        for (int j = i+1; j < end; ++j){
            if (dist(P[i], P[j]) < min){
                min = dist(P[i], P[j]);
                sprintf(out, "%s %s", P[i]->id, P[j]->id);
            }
        }
    }
    return min;
}
 

float min(float x, float y)
{
    return (x < y)? x : y;
}
 
 

float stripClosest(Point *strip[], int bgn, int end, float d, char* out)
{
    float min = d;
 
    mergeSort(strip, bgn, end, compareY); // merge ou heap
 
    for (int i = bgn; i < end; ++i){
        for (int j = i+1; j < end && (strip[j]->y - strip[i]->y) < min; ++j){
            if (dist(strip[i],strip[j]) < min){
                min = dist(strip[i], strip[j]);
                sprintf(out, "%s %s", strip[i]->id, strip[j]->id);
            }
        }
    }
 
    return min;
}
 

float closestUtil(Point *P[], int bgn, int end, char* out)
{

    if (end-bgn <= 3){
        return bruteForce(P, bgn, end, out);
    }

    int mid = (end-bgn)/2 + bgn;
    Point midPoint = *P[mid];

    char *out1, *out2;
    out1 = (char*) calloc(25, sizeof(char));
    out2 = (char*) calloc(25, sizeof(char));
 
    float dl = closestUtil(P, bgn, mid, out1);
    float dr = closestUtil(P, mid+1, end, out2);
 
    float d;
    if(dl<dr){
        d = dl;
        strcpy(out, out1);
    }else{
        d = dr;
        strcpy(out, out2);
    }
    free(out1);
    free(out2);

    


    Point *strip[end-bgn];
    int j = 0;
    for (int i = bgn; i <= end; i++)
        if (abs(P[i]->x - midPoint.x) < d){
            strip[j] = P[i], j++;
        }

    double res;
    out1 = (char*) calloc(155, sizeof(char));
    res = stripClosest(strip, 0, j, d, out1);
    if(d <= res){
        return d;
    }else{
        strcpy(out, out1);
        free(out1);
        return res;
    }

}
 

float closest(Point *P[], int n, char* out)
{
    mergeSort(P, 0, n, compareX); // merge ou heap
 
    return closestUtil(P, 0, n, out);
}