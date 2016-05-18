//
//  main.cpp
//  preprocess on KB
//
//  Created by Vanellope on 5/11/16.
//  Copyright © 2016 Vanellope. All rights reserved.
//
#define MAX 2000
#define FACTMAX 500000
#define NMAX 32000
#define K 10
#define val 0.0001//negative facts
#define eta 100
#define GEN 1000

#include <iostream>
#include <math.h>
#include <map>
using namespace std;

map <string,int> relation;
map <string,int> entity;
int num_e=0;//记录entity个数
int num_r=0;//relation number
int num_t=0;//triple number
int neg[500]={0};
int num_neg=1;
int flag[FACTMAX];

int M[FACTMAX][3];
int M1[FACTMAX][3];
int M2[FACTMAX][3];



FILE* tp=fopen("FB15k/m3train.dat","w");
FILE* tp2=fopen("FB15k/m3test.dat","w");
FILE* tp3=fopen("FB15k/m3valid.dat","w");

double randdouble(){
    double r=rand()/(double)RAND_MAX;
    return r;
}

int getline(FILE* fp, char a[]){
    int i=0;
    while(1){
        fscanf(fp,"%c",&a[i]);
        if (a[i]==EOF) return -1;
        if (a[i]=='\n') {
            a[i]='\0';
            return 1;
        }
        i++;
    }
    return 0;
}


void input(){
    char a[MAX];
    FILE* fp=fopen("FB15k/freebase_mtr100_mte100-train.txt","r");
    //int j=1;//记录行号
    while (getline(fp,a)!=-1){
        char subject[MAX], object[MAX],relat[MAX];
        int i=0;
        while (a[i]!='\t'){
            subject[i]=a[i];
            i++;
        }
        subject[i]='\0';
        int i1=i+1;
        while (a[i1]!='\t')
            i1++;
        i1++;
        int j=0;
        while (a[i1]!='\0'){
            object[j]=a[i1];
            i1++;
            j++;
        }
        object[j]='\0';
        //cout<<subject<<'\n'<<object<<'\n';
        int subjectnum,objectnum;
        if (entity.find(subject)!=entity.end()){
            subjectnum=entity[subject];
        }
        else{
            num_e++;
            subjectnum=entity[subject]=num_e;
        }
        if (entity.find(object)!=entity.end()){
            objectnum=entity[object];
        }
        else{
            num_e++;
            objectnum=entity[object]=num_e;
        }
        i++;
        while(a[i]!='\t'){
            i++;
            int r=0;
            while(a[i]!='\t'&&a[i]!='\\'){
                relat[r]=a[i];
                r++;
                i++;
            }
            relat[r]='\0';
            int relationnum;
            if (relation.find(relat)!=relation.end()){
                relationnum=relation[relat];
            }
            else{
                num_r++;
                relationnum=relation[relat]=num_r;
            }
            num_t++;
            M[num_t][0]=subjectnum;
            M[num_t][1]=relationnum;
            M[num_t][2]=objectnum;
        }
    }
    for (int i=1;i<=num_t;i++){
        fprintf(tp,"%d %d %d \n",M[i][0],M[i][1],M[i][2]);
    }
    cout<<num_e<<' '<<num_r<<' '<<num_t<<' ';
    return ;
}

int n_e=0;
int n_r=0;
int n_t=0;

void input2(){
    char a[MAX];
    FILE* fp2=fopen("FB15k/freebase_mtr100_mte100-test.txt","r");
    //int j=1;//记录行号
    while (getline(fp2,a)!=-1){
        char subject[MAX], object[MAX],relat[MAX];
        int i=0;
        while (a[i]!='\t'){
            subject[i]=a[i];
            i++;
        }
        subject[i]='\0';
        int i1=i+1;
        while (a[i1]!='\t')
            i1++;
        i1++;
        int j=0;
        while (a[i1]!='\0'){
            object[j]=a[i1];
            i1++;
            j++;
        }
        object[j]='\0';
        //cout<<subject<<'\n'<<object<<'\n';
        int subjectnum,objectnum;
        if (entity.find(subject)!=entity.end()){
            subjectnum=entity[subject];
        }
        else{
            n_e++;
            subjectnum=entity[subject]=n_e;
        }
        if (entity.find(object)!=entity.end()){
            objectnum=entity[object];
        }
        else{
            n_e++;
            objectnum=entity[object]=n_e;
        }
        i++;
        while(a[i]!='\t'){
            i++;
            int r=0;
            while(a[i]!='\t'&&a[i]!='\\'){
                relat[r]=a[i];
                r++;
                i++;
            }
            relat[r]='\0';
            int relationnum;
            if (relation.find(relat)!=relation.end()){
                relationnum=relation[relat];
            }
            else{
                n_r++;
                relationnum=relation[relat]=n_r;
            }
            n_t++;
            M1[n_t][0]=subjectnum;
            M1[n_t][1]=relationnum;
            M1[n_t][2]=objectnum;
        }
    }
    for (int i=1;i<=n_t;i++){
        fprintf(tp2,"%d %d %d \n",M1[i][0],M1[i][1],M1[i][2]);
    }
    cout<<n_e<<' '<<n_r<<' '<<n_t<<' ';
    return ;
}

int nu_e=0;
int nu_r=0;
int nu_t=0;

void input3(){
    char a[MAX];
    FILE* fp3=fopen("FB15k/freebase_mtr100_mte100-valid.txt","r");
    //int j=1;//记录行号
    while (getline(fp3,a)!=-1){
        char subject[MAX], object[MAX],relat[MAX];
        int i=0;
        while (a[i]!='\t'){
            subject[i]=a[i];
            i++;
        }
        subject[i]='\0';
        int i1=i+1;
        while (a[i1]!='\t')
            i1++;
        i1++;
        int j=0;
        while (a[i1]!='\0'){
            object[j]=a[i1];
            i1++;
            j++;
        }
        object[j]='\0';
        //cout<<subject<<'\n'<<object<<'\n';
        int subjectnum,objectnum;
        if (entity.find(subject)!=entity.end()){
            subjectnum=entity[subject];
        }
        else{
            nu_e++;
            subjectnum=entity[subject]=nu_e;
        }
        if (entity.find(object)!=entity.end()){
            objectnum=entity[object];
        }
        else{
            nu_e++;
            objectnum=entity[object]=nu_e;
        }
        i++;
        while(a[i]!='\t'){
            i++;
            int r=0;
            while(a[i]!='\t'&&a[i]!='\\'){
                relat[r]=a[i];
                r++;
                i++;
            }
            relat[r]='\0';
            int relationnum;
            if (relation.find(relat)!=relation.end()){
                relationnum=relation[relat];
            }
            else{
                nu_r++;
                relationnum=relation[relat]=nu_r;
            }
            n_t++;
            M2[nu_t][0]=subjectnum;
            M2[nu_t][1]=relationnum;
            M2[nu_t][2]=objectnum;
        }
    }
    for (int i=1;i<=n_t;i++){
        fprintf(tp2,"%d %d %d \n",M2[i][0],M2[i][1],M2[i][2]);
    }
    cout<<nu_e<<' '<<nu_r<<' '<<nu_t<<' ';
    return ;
}

int main(){
    input();
    input2();
    input3();
}