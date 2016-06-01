//
//  main.cpp
//  factorization machine on wordnet18
//
//  Created by Vanellope on 5/20/16.
//  Copyright © 2016 Vanellope. All rights reserved.
//
//(o,r) vs. s
#define MAX 2000
#define FACTMAX 500000
#define NMAX 8200000
#define num_e 40943
#define num_r 18
#define num_t 141442
#define num_valid 5000
#define K 30//<35
#define thres 0.001// schosatic
//#define val 0.0001//negative facts
#define eta 10
#define GEN 50000
#define gama 0.01//learning rate

#include <iostream>
#include <math.h>
using namespace std;

int n=num_r*num_e+num_e;
double v[NMAX];
double w[NMAX][K];
//double v1[NMAX];
//double w1[NMAX][K];
double X[FACTMAX][eta+1];
double X1[num_valid+1];
int M[FACTMAX][3];
int M1[num_valid+1][3];
//int Neg[FACTMAX][eta+1];
int flag[FACTMAX]={0};
//long x[FACTMAX][eta+1],y[FACTMAX];
//long line[FACTMAX];

FILE *fp=fopen("/WN18/m3train.dat","r");
FILE *tp=fopen("/WN18/m3test.dat","r");

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


double dot(long x, long y){
    double sum=0;
    for (int j=0;j<K;j++){
        sum+=2*w[x][j]*w[y][j];
        sum+=w[x][j]*w[x][j];
        sum+=w[y][j]*w[y][j];
    }
    return sum;
}

double evaluate(){
    double eva=0;
    double evaluate=0;
    for (int i=0;i<num_valid;i++){
        // double value[10]={0};
        int m=0;
        int x=(M1[i][1]-1)*num_e+M1[i][2]-1;
        int y=M1[i][0]+num_e*num_r-1;
        X1[i]=v[x]+v[y]+dot(x, y);
   //     cout<<"we are evaluating!\n";
   //     cout<<X1[i]<<'\n';
        for (int j=0;j<num_e;j++){
            if(j==M1[i][2]) continue;
            int x1=(M1[i][1]-1)*num_e+j-1;
            eva=v[x1]+v[y]+dot(x1,y);
            if (eva>X1[i]){
          //      cout<<j<<' '<<eva<<'\n';
                m++;
            }
            if (m==1) break;
        }
        if (m<1) {cout<<i<<'\n';evaluate++;}
      //  int t;
      //  cin>>t;
      //  if(t==0) return 0;
    }
    //cout<<"aaa"<<evaluate<<'\n';
    evaluate=evaluate/(double)num_valid;
    return evaluate;
}

void training(){
    for(long g=1;g<=GEN;g++){
        double loss=0;
        for(long i=0;i<num_t;i++){
            if(randdouble()>thres) continue;
            long Neg[eta+1]={0};
            long x[eta+1];
            double X[eta+1];
            x[0]=(M[i][1]-1)*num_e+M[i][2]-1;

            long y=M[i][0]+num_e*num_r-1;
            X[0]=v[x[0]]+v[y]+dot(x[0],y);
            double temp[eta+1];
            if(X[0]<1) {loss+=(1-X[0])*(1-X[0]);temp[0]=(1-X[0])*(2);}
            else {temp[0]=0;}
            v[x[0]]+=gama*temp[0];

            for(int j=1;j<=eta;j++){
                double r1=randdouble();
               // cout<<r1*num_e<<'\n';
                Neg[j]=(long)(r1*num_e);
                if(Neg[j]==0) Neg[j]=num_e;
                x[j]=(M[i][1]-1)*num_e+Neg[j]-1;
                X[j]=v[x[j]]+v[y]+dot(x[j],y);
                if(X[j]>-1) {loss+=(1+X[j])*(1+X[j])/eta;temp[j]=2*(1+X[j])/eta;}
                else {temp[j]=0;}
                v[x[j]]-=gama*temp[j];
      //          for(int k=0;k<K;k++)
      //              w[x[j]][k]-=gama*temp[j]*2*(w[x[j]][k]+w[y][k]);
            }
            v[y]+=gama*temp[0];
            for(int j=1;j<=eta;j++)
                v[y]-=gama*temp[j];
            for(int k=0;k<K;k++){
                double w1[eta+1],w2=w[y][k];
                for(int j=0;j<=eta;j++) w1[j]=w[x[j]][k];
                w[x[0]][k]+=gama*temp[0]*2*(w1[0]+w2);
                for(int j=1;j<=eta;j++)
                    w[x[j]][k]-=gama*temp[j]*2*(w1[j]+w2);
                w[y][k]+=gama*temp[0]*2*(w1[0]+w2);
                for(int j=1;j<=eta;j++)
                    w[y][k]-=gama*temp[j]*2*(w1[j]+w2);
                
            }
            
        }

            cout<<g<<' '<<loss<<'\n';
        if(g%10000==0) cout<<evaluate()<<'\n';
        //if(loss<1) return;
    }
}

int main(){
    srand(99);
    initial();
    input();
    training();
   // evaluate();
}