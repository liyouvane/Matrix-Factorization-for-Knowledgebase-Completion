//
//  main.cpp
//  3 FM on KB
//
//  Created by Vanellope on 5/15/16.
//  Copyright © 2016 Vanellope. All rights reserved.
//


#define MAX 2000
#define FACTMAX 500000
#define NMAX 32000
#define num_e 14951
#define num_r 1345
#define num_t 483142
#define num_valid 50000
#define K 5//<50
#define thres 0.001// schosatic
//#define val 0.0001//negative facts
#define eta 3
#define GEN 1000
#define gama 0.0001//learning rate

#include <iostream>
#include <math.h>
using namespace std;

int n=2*num_e+num_r;
double v[NMAX];
double w[NMAX][K];
double X[FACTMAX][eta+1];
double X1[FACTMAX];
int M[FACTMAX][3];
int M1[FACTMAX][3];
int Neg[FACTMAX][eta];
int flag[FACTMAX];

FILE *fp=fopen("/FB15k/m3train.dat","r");
FILE *tp=fopen("/FB15k/m3valid.dat","r");

double randdouble(){
    double r=rand()/(double)RAND_MAX;
    return r;
}

void initial(){
    for (int i=0;i<n;i++){
        v[i]=0;
        for (int j=0;j<K;j++){
            w[i][j]=randdouble()*2/sqrt(K)-1/sqrt(K);
        }
    }
    return;
}

void input(){
    for (int i=0;i<num_t;i++){
        for (int j=0;j<3;j++){
            fscanf(fp,"%d", &M[i][j]);
        }
    }
    for (int i=0;i<num_valid;i++){
        for (int j=0;j<3;j++){
            fscanf(tp,"%d", &M1[i][j]);
        }
    }
    return ;
}


double dot(int x, int y, int z){
    double sum=0;
    for (int j=0;j<K;j++){
        sum+=w[x][j]*w[y][j];
        sum+=w[y][j]*w[z][j];
        sum+=w[z][j]*w[x][j];
        sum+=w[x][j]*w[x][j];
        sum+=w[y][j]*w[y][j];
        sum+=w[z][j]*w[z][j];
    }
    return sum;
}

double evaluate(){
    double eva=0;
    double evaluate=0;
    for (int i=0;i<num_valid;i++){
       // double value[10]={0};
        int m=0;
        int x=M1[i][0]-1;
        int y=M1[i][1]+num_e-1;
        int z=M1[i][2]+num_e+num_r-1;
        X1[i]=v[x]+v[y]+v[z]+dot(x, y, z);
        //cout<<X1[i]<<'\n';
        for (int j=0;j<num_e;j++){
            int z1=j+num_e+num_r;
            eva=v[x]+v[y]+v[z1]+dot(x,y,z1);
            if (eva>X1[i]){
                //cout<<j<<' '<<eva<<'\n';
                m++;
            }
            if (m==10) break;
        }
        if (m<9) evaluate++;
    }
    //cout<<"aaa"<<evaluate<<'\n';
    evaluate=evaluate/(double)num_valid;
    return evaluate;
}

void score(){
    for (int g=1;g<=GEN;g++){
        double loss=0;
        for (int i=0;i<num_t;i++){
            for (int j=0;j<eta;j++){
                double r1=randdouble();
                Neg[i][j]=(int)num_e*r1;
                if (Neg[i][j]==0) Neg[i][j]=num_e;
            }
            int x=M[i][0]-1;
            int y=M[i][1]+num_e-1;
            int z=M[i][2]+num_e+num_r-1;
            X[i][0]=v[x]+v[y]+v[z]+dot(x,y,z);
            //cout<<X[i][0]<<' ';
            for (int j=0;j<eta;j++){
                int z1=Neg[i][j]+num_e+num_r-1;
                X[i][j+1]=v[x]+v[y]+v[z1]+dot(x,y,z1);
              //  cout<<X[i][j+1]<<' ';
            }
           // cout<<'\n';
        }
        for (int i=1;i<num_t;i++){
            loss-=log(1+exp(X[i][0]));
            for (int j=0;j<eta;j++){
                loss+=1/(double)eta*log(1+exp(X[i][j+1]));
            }
        }
        cout<<g<<' '<<loss<<'\n';
        
        for (int i=0;i<n;i++){
            double r2=randdouble();
            if (r2<thres){
                for (int f=0;f<num_t;f++){
                    int x=M[f][0]-1;
                    int y=M[f][1]+num_e-1;
                    int z=M[f][2]+num_e+num_r-1;
                    if ((x==i)||(y==i)||(z==i)){
                        v[i]+=gama*exp(X[f][0])/(1+exp(X[f][0]));
                        //cout<<i<<' '<<v[i]<<'\n';
                    }
                    for (int j=0;j<eta;j++){
                        int z1=Neg[f][j]+num_e+num_r-1;
                        if ((x==i)||(y==i)||(z1==i)){
                            v[i]-=gama*exp(X[f][j+1])/(eta*(1+exp(X[f][j+1])));
                        }
                    }
                }
            }
        }
        
        for (int i=0;i<n;i++){
            for (int k=0;k<K;k++){
                double r3=randdouble();
                if (r3<thres){
                    for (int f=0;f<num_t;f++){
                        int x=M[f][0]-1;
                        int y=M[f][1]+num_e-1;
                        int z=M[f][2]+num_e+num_r-1;
                        if ((x==i)||(y==i)||(z==i)){
                            w[i][k]-=w[i][k]*exp(X[f][0])/(1+exp(X[f][0]));
                            for (int s=0;s<n;s++){
                                if((x==s)||(y==s)||(z==s)){
                                    w[i][k]+=w[s][k]*exp(X[f][0])/(1+exp(X[f][0]));
                                }
                            }
                        }
                        for (int j=0;j<eta;j++){
                            int z1=Neg[f][j]+num_e+num_r-1;
                            if ((x==i)||(y==i)||(z1==i)){
                                w[i][k]+=w[i][k]*exp(X[f][j+1])/(eta*(1+exp(X[f][j+1])));
                                for(int s=0;s<n;s++){
                                    if ((x==s)||(y==s)||(z1==s))
                                        w[i][k]-=w[s][k]*exp(X[f][j+1])/(eta*(1+exp(X[f][j+1])));
                                }
                            }
                        }
                    }
                }
            }
        }
        //double e=evaluate();
        if (g%200==0) {
            double e=evaluate();
            cout<<e<<'\n';
        }
    }
    return ;
}

int main(){
    initial();
    input();
    score();
    //evaluate();
}