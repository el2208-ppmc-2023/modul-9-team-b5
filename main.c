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
#include <stdbool.h>
#include <math.h>
#include <limits.h>

#define maxline 255
#define maxpelabuhan 20
#define radius 6371
#define TO_RAD (3.1415926536 / 180)

struct Pelabuhan
{
    char nama[50];
    double lintang;
    double bujur;
};

struct Pelabuhan pelabuhan[maxpelabuhan];
double jrk_pelabuhan[maxpelabuhan][maxpelabuhan];
int visited_harbor[maxpelabuhan];
double jarak = 0;
int first_harbor = 0;
int first_harbor_flag = 1;
int harbor_num[maxpelabuhan];
int harbor_total = -1;

//fungsi-fungsi
void tsp(int h);
double hitungJarak(struct Pelabuhan pelabuhan1, struct Pelabuhan pelabuhan2);
double convert_RadiantoDegree(double radian);
void filetoArray(struct Pelabuhan array[maxpelabuhan]);
void arraytoMatrix(struct Pelabuhan array[maxpelabuhan], double matrix[maxpelabuhan][maxpelabuhan]);
int find_next_h(int h);
int harbor_check(int h);




int main()
{
    filetoArray(pelabuhan);
    printf("Matriks jarak antar pelabuhan : \n");
    arraytoMatrix(pelabuhan, jrk_pelabuhan);

    printf("\n\nRute Pelayaran Optimal:\n");
    tsp(0);
    printf("\n\nJarak Total Rute Pelayaran: ");
    printf("%.3f km\n", jarak);

    printf("\n\nKota yang Tidak Dikunjungi:\n");
    bool not_route = false;
    int num = 1;
    for(int i = 0; i < maxpelabuhan; i++){
        for(int j = 0; j <= harbor_total; j++){
            if(i == harbor_num[j]){
                not_route = true;
            }
        }
        if (!not_route){
            printf("%d. %s\n", num, pelabuhan[i].nama);
            num++;
        }
        not_route = false;
    }
    return 0;
}

void filetoArray(struct Pelabuhan array[maxpelabuhan]){
    FILE *fp;
    char line[maxline];
    char namafile[100];
    char *token;
    int count = 0;

    printf("Masukkan nama file: ");
    scanf("%s", namafile);
    fp = fopen(namafile, "r");
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

void arraytoMatrix(struct Pelabuhan array[maxpelabuhan], double matrix[maxpelabuhan][maxpelabuhan]){
    for(int i = 0; i < maxpelabuhan; i++){
        for(int j = 0; j < maxpelabuhan; j++){
            matrix[i][j] = hitungJarak(array[i], array[j]);
            printf("%.1f ", matrix[i][j]);
        }
        printf("\n");
        visited_harbor[i]=0;
    }
}

double hitungJarak(struct Pelabuhan pelabuhan1, struct Pelabuhan pelabuhan2){
    double hasil;
    double a = pow(sin(convert_RadiantoDegree(pelabuhan1.lintang-pelabuhan2.lintang)/2),2) + cos(convert_RadiantoDegree(pelabuhan1.lintang))*cos(convert_RadiantoDegree(pelabuhan2.lintang))*pow(sin(convert_RadiantoDegree(pelabuhan1.bujur-pelabuhan2.bujur)/2),2);
    double c = 2*atan2(sqrt(a), sqrt(1-a));
    hasil = radius*c;
    return hasil;
}

double convert_RadiantoDegree(double radian){
        return radian*(3.14/180);
    }

int harbor_check(int h){
    double min = INT_MAX;
    for (int i = 0; i < maxpelabuhan; i++){
        for(int j = 0; j < maxpelabuhan; j++){
            if (jrk_pelabuhan[i][j] < min && jrk_pelabuhan[i][j]!= 0){
                min = jrk_pelabuhan[i][j];
                h = i;
            }
        }
    }
    first_harbor_flag = 0;
    first_harbor = h;
}

void tsp(int h){
    int next_h;
    if(first_harbor_flag == 1){
        h = harbor_check(h);
    }
    visited_harbor[h] = 1;
    printf("%s --> ", pelabuhan[h].nama);

    harbor_total++;
    harbor_num[harbor_total] = h;
    next_h = find_next_h(h);

    if(next_h == INT_MAX){
        next_h = first_harbor;
        jarak += jrk_pelabuhan[h][next_h];
        printf("%s", pelabuhan[next_h].nama);
        return;
    }

    tsp(next_h);
    
}

int find_next_h(int h){
    int nh;
    double min = INT_MAX, temp_jrk=0;

    for(int i=0; i< maxpelabuhan; i++){
        if(visited_harbor[i] == 0 && jrk_pelabuhan[h][i] != 0 && jrk_pelabuhan[h][i] < min){
            temp_jrk = jrk_pelabuhan[h][i];
            min = jrk_pelabuhan[h][i];
            nh = i;
        }
    }
    if (min > 2500){
        visited_harbor[nh] = 1;
        min = 0;
        nh = INT_MAX;
    }
    if(temp_jrk != INT_MAX){
        jarak += min;
    }
    return nh;
}

