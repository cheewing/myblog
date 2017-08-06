#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h> 
#include <sys/time.h>
void swap(int &x, int &y); 
int partition(int *array, int left, int right); 
void qsort(int *array, int left, int right);
void print(int *array, int left, int right);
void print(int *array, int n);
void input(int *&array, int n);

const int number = 8;

int main(int argc, char *argv[])
{
    //int array[] = {13, 19, 9, 5, 12, 8, 7, 4, 21, 2, 6, 11};
    int array[] = {2, 8, 7, 1, 3, 5, 6, 4};
    //int *array = new int[number];
    int n = number;
    //input(array, number);

    printf("befor qsort\n");
    print(array, n);
    printf("\n");

    struct timeval stv;
    gettimeofday(&stv, NULL);
    int st_usec = stv.tv_sec * 1000000 + stv.tv_usec;

    qsort(array, 0, n - 1);
    struct timeval end_tv;
    gettimeofday(&end_tv, NULL);
    int end_usec = end_tv.tv_sec * 1000000 + end_tv.tv_usec;
    int cost = end_usec - st_usec;
    printf("qsort cost time : %dus\n", cost);

    printf("after qsort\n");
    print(array, n);
    //delete []array;
    exit(0);
}

void qsort(int *array, int left, int right)
{
    if (left < right)
    {
        //print(array, 0, number - 1);
        int pos = partition(array, left, right);
        qsort(array, left, pos - 1);
        qsort(array, pos + 1, right);
    }
}

int partition(int *array, int left, int right)
{
    printf("before partition\n");
    print(array, 0, number - 1);
    int i = left - 1;
    int x = array[right];

    for (int j = left; j < right; j++)
    {
        printf("in partition : ");
        if (array[j] <= x)
        {
            ++i;
            swap(array[i], array[j]);
        }
        //print(array, 0, number - 1);
        print(array, left, right);
        //printf("in partition\n");
    }

    swap(array[i + 1], array[right]);
    printf("after partition\n");
    print(array, 0, number - 1);
    printf("\n");
    return i + 1;
}

void swap(int &x, int &y)
{
    int temp = x;
    x = y;
    y = temp;
}

void print(int *array, int left, int right)
{
    for (int i = left; i <= right; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");
}

void print(int *array, int n)
{
    print(array, 0, n - 1);
}

void input(int *&array, int n)
{
    for (int i = 0; i < n; i++)
    {
        array[i] = rand() % n;
    }
}
