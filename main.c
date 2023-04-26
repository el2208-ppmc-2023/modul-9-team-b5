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

struct Pelabuhan
{
    char nama[50];
    double lintang;
    double bujur;
};

// DEFINISI VARIABEL 
struct Pelabuhan pelabuhan[maxpelabuhan]; 
//-> struct yang menyimpan data pelabuhan dan titik koordinatnya
double jrk_pelabuhan[maxpelabuhan][maxpelabuhan]; 
//-> matriks yang menyimpan data jarak setiap pelabuhan (distance matrix)
int visited_harbor[maxpelabuhan]; 
//-> variabel untuk memberikan tanda pada setiap pelabuhan, yaitu 1 menandakan sudah dikunjungi dan 0 menandakan belum dikunjungi
double jarak = 0; 
//-> variabel untuk menyimpan jarak total rute pelayaran
int first_harbor = 0; 
//-> variabel untuk menyimpan indeks pelabuhan awal
int first_harbor_flag = 1; 
//-> variabel untuk memberikan tanda pada pelabuhan awal, yaitu 1 menandakan pelabuhan awal belum ditentukan dan 0 menandakan pelabuhan awal sudah ditentukan
int harbor_num[maxpelabuhan]; 
//-> array untuk menyimpan indeks pelabuhan yang dilewati
int harbor_total = -1; 
//-> variabel yang jumlah elemen dari array harbor_num atau menunjukkan jumlah pelabuhan yang dilewati


// DEFINISI FUNGSI
void filetoArray(struct Pelabuhan array[maxpelabuhan]);
//-> Fungsi untuk menerima masukan file dan memasukkan isinya ke dalam array
void arraytoMatrix(struct Pelabuhan array[maxpelabuhan], double matrix[maxpelabuhan][maxpelabuhan]);
//-> Fungsi untuk membuat matriks jarak dari data pelabuhan 
double hitungJarak(struct Pelabuhan pelabuhan1, struct Pelabuhan pelabuhan2);
//-> Fungsi untuk menghitung jarak dari titik-titik koordinat menggunakan rumus haversine
double convert_RadiantoDegree(double radian);
//-> Fungsi untuk mengubah satuan radian ke derajat
int harbor_check(int h);
// -> Fungsi untuk menentukan pelabuhan yang akan menjadi titik awal rute
void tsp(int h);
//-> Fungsi untuk menentukan solusi rute pelayaran yang optimal sebagai solusi dari travelling salesman problem
int find_next_h(int h);
// -> Fungsi untuk menentukan pelabuhan selanjutnya dengan algoritma nearest neighboor
void skipped_harbor();
//-> Fungsi untuk mencetak pelabuhan yang tidak dilewati

// FUNGSI MAIN PROGRAM
int main()
{
    filetoArray(pelabuhan);
    printf("\n\nMatriks jarak antar pelabuhan : \n");
    arraytoMatrix(pelabuhan, jrk_pelabuhan);

    printf("\n\nRute Pelayaran Optimal:\n");
    tsp(0);
    printf("\n\nJarak Total Rute Pelayaran: ");
    printf("%.3f km\n", jarak);

    printf("\n\nKota yang Tidak Dilewati:\n");
    skipped_harbor();
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
    // Jika file tidak ada
    if (fp == NULL)
    {
        printf("\nTidak ada file.");
        exit(0);
    }
    
    // Jika file kosong
    int c = fgetc(fp);
    if (c == EOF){
        printf("\nFile kosong.");
        exit(0);
    }
    ungetc(c,fp);

    // Membaca isi file dan memasukkan isi file ke dalam array
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
            //Menentukan jarak yang paling kecil dari matriks jarak
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
    //Menentukan pelabuhan titik awal pelayaran
    if(first_harbor_flag == 1){
        h = harbor_check(h);
    }
    visited_harbor[h] = 1;

    // Mencetak nama pelabuhan yang dilewati
    printf("%s --> ", pelabuhan[h].nama);

    //Menambahkan indeks pelabuhan yang dilewati ke dalam array harbor_num
    harbor_total++;
    harbor_num[harbor_total] = h;

    //Menentukan pelabuhan selanjutnya
    next_h = find_next_h(h);

    // Bagian basis jika semua pelabuhan sudah dilewati oleh fungsi tsp
    if(next_h == INT_MAX){
        next_h = first_harbor;
        jarak += jrk_pelabuhan[h][next_h];
        printf("%s", pelabuhan[next_h].nama);
        return;
    }

    //Bagian rekursi dengan memanggil fungsi tsp dengan parameter pelabuhan selanjutnya
    tsp(next_h);
    
}

int find_next_h(int h){
    int nh;
    double min = INT_MAX, temp_jrk=0;

    for(int i=0; i< maxpelabuhan; i++){
        //Menentukan pelabuhan dan jaraknya yang memiliki jarak paling kecil dengan pelabuhan h
        if(visited_harbor[i] == 0 && jrk_pelabuhan[h][i] != 0 && jrk_pelabuhan[h][i] < min){
            temp_jrk = jrk_pelabuhan[h][i];
            min = jrk_pelabuhan[h][i];
            nh = i;
        }
    }

    // Jika jarak minimal lebih besar dari 2500, jarak minimal dibuat 0 agar tidak ditambahkan ke jarak total dan pelabuhan selanjutnya kembali ke awal
    if (min > 2500){
        min = 0;
        nh = INT_MAX;
    }

    if(temp_jrk != INT_MAX){
        //Menambahkan jarak minimal pelabuhan ke dalam total jarak rute pelayaran
        jarak += min;
    }
    return nh;
}

void skipped_harbor(){
    bool not_route = false;
    int num = 1;
    for(int i = 0; i < maxpelabuhan; i++){
        for(int j = 0; j <= harbor_total; j++){
            if(i == harbor_num[j]){
                not_route = true;
            }
        }
        //Mencetak pelabuhan yang indeksnya tidak ada di dalam array_num
        if (!not_route){
            printf("%d. %s\n", num, pelabuhan[i].nama);
            num++;
        }
        not_route = false;
    }
}