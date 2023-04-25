/*EL2208 Praktikum Pemecahan Masalah dengan C 2022/2023
 *Modul : 9 - Tugas Besar
 *Kelompok : B5
 *Hari dan Tanggal : 16 April 2023
 *Asisten (NIM) : Emmanuella Rumanti (13220031)
 *Nama File : main.c
 *Deskripsi : Traveling salesman problem
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#define maxline 255
#define maxpelabuhan 20
#define radius 6371

struct Pelabuhan
{
    char nama[50];
    double lintang;
    double bujur;
};

struct Pelabuhan pelabuhan[maxpelabuhan];
int jrk_pelabuhan[maxpelabuhan][maxpelabuhan];
int visited_harbor[maxpelabuhan];
int jarak = 0;

//fungsi-fungsi
void tsp(int h);
int hitungJarak(struct Pelabuhan pelabuhan1, struct Pelabuhan pelabuhan2);
double convert_RadiantoDegree(double radian);
void filetoArray(struct Pelabuhan array[maxpelabuhan]);
void arraytoMatrix(struct Pelabuhan array[maxpelabuhan], int matrix[maxpelabuhan][maxpelabuhan]);
int find_next_h(int h);





int main()
{
    filetoArray(pelabuhan);
    printf("Matriks jarak antar pelabuhan : \n");
    arraytoMatrix(pelabuhan, jrk_pelabuhan);

    printf("\n\nRute Pelayaran Optimal:\n");
    tsp(0);
    printf("\n\nJarak Total Rute Pelayaran: ");
    printf("%d km\n", jarak);

    return 0;
}

void filetoArray(struct Pelabuhan array[maxpelabuhan]){
    FILE *fp;
    char line[maxline];
    char *token;
    int count = 0;

    fp = fopen("pelabuhan.csv", "r");
    if (fp == NULL)
    {
        printf("Tidak ada file.");
        exit(0);
    }

    fgets(line, maxline,fp);
    while (fgets(line, maxline, fp))
    {
        token = strtok(line, ",");
        strcpy(array[count].nama, token);
        token = strtok(NULL, ",");
        array[count].lintang = atof(token);
        token = strtok(NULL, ",");
        array[count].bujur = atof(token);
        count++;
    }

    fclose(fp);


}

void arraytoMatrix(struct Pelabuhan array[maxpelabuhan], int matrix[maxpelabuhan][maxpelabuhan]){
    for(int i = 0; i < maxpelabuhan; i++){
        for(int j = 0; j < maxpelabuhan; j++){
            matrix[i][j] = hitungJarak(array[i], array[j]);
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int hitungJarak(struct Pelabuhan pelabuhan1, struct Pelabuhan pelabuhan2){
    int hasil;
    double a = pow(sin(convert_RadiantoDegree(pelabuhan1.lintang-pelabuhan2.lintang)/2),2) + cos(convert_RadiantoDegree(pelabuhan1.lintang))*cos(convert_RadiantoDegree(pelabuhan2.lintang))*pow(sin(convert_RadiantoDegree(pelabuhan1.bujur-pelabuhan2.bujur)/2),2);
    double c = 2*atan2(sqrt(a), sqrt(1-a));
    hasil = radius*c;
    return hasil;
}

double convert_RadiantoDegree(double radian){
        return radian*(3.14/180);
    }

void tsp(int h){
    int next_h;
    visited_harbor[h] = 1;
    printf("%s ->", pelabuhan[h].nama);

    next_h = find_next_h(h);

    if(next_h == INT_MAX){
        int v = 0;
        jarak += jrk_pelabuhan[next_h][v];
        printf("%s", pelabuhan[v].nama);
        return;
    }

    tsp(next_h);
    
}

int find_next_h(int h){
    int nh, min = INT_MAX, min_index, temp_jrk=0;

    for(int i=0; i< maxpelabuhan; i++){
        if(visited_harbor[i] == 0 && jrk_pelabuhan[h][i] != 0 && jrk_pelabuhan[h][i] < min){
            temp_jrk = jrk_pelabuhan[h][i];
            min = jrk_pelabuhan[h][i];
            min_index = i;
        }
    }
    if (min == INT_MAX){
        return min;
    }
    else{
        nh = min_index;
        jarak += temp_jrk;
        return nh;
    }
}

