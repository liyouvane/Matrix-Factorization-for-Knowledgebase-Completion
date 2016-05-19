#include <iostream>
#include <fstream>
#include <cstdlib>
#include <math.h>
#define MAX_FACT 490000
#define MAX_ENTITY 15000
#define MAX_RELATION 1400 
#define F 483142
#define E 14951
#define R 1345

#define RE 20  //RE=10Rr,Rr<15 
#define Rr 2
#define Ita 5
#define SMP 0.001 
using namespace std;
int train[MAX_FACT][3];
int valid[MAX_FACT][3];
int test[MAX_FACT][3];

double a[MAX_ENTITY][Rr+1][RE+1];
double b[RE+1][MAX_RELATION][RE+1];
double c[RE+1][Rr+1][MAX_ENTITY];

double a1[MAX_ENTITY][Rr+1][RE+1];
double b1[RE+1][MAX_RELATION][RE+1];
double c1[RE+1][Rr+1][MAX_ENTITY];

FILE *tp=fopen("m3.dat","w");
void input()
{
    FILE *fp1=fopen("m3train.dat","r");
    FILE *fp2=fopen("m3valid.dat","r");
    FILE *fp3=fopen("m3test.dat","r");
    for(long i=1;i<=F;i++)
      fscanf(fp1,"%d%d%d",&train[i][0],&train[i][1],&train[i][2]);
    for(int i=1;i<=59071;i++)
      fscanf(fp3,"%d%d%d",&test[i][0],&test[i][1],&test[i][2]);
    for(int i=1;i<=50000;i++)
      fscanf(fp2,"%d%d%d",&valid[i][0],&valid[i][1],&valid[i][2]);
    return;
    }

void initialize(){
    double ini=pow(1.0/RE/RE/Rr,1.0/3);
    for(long s=1;s<=E;s++)
      for(long r=1;r<=Rr;r++)
        for(long o=1;o<=RE;o++)
          a[s][r][o]=(ini*rand())/RAND_MAX-ini;
    for(long s=1;s<=RE;s++)
      for(long r=1;r<=R;r++)
        for(long o=1;o<=RE;o++)
          b[s][r][o]=(ini*rand())/RAND_MAX-ini;
    for(long s=1;s<=RE;s++)
      for(long r=1;r<=Rr;r++)
        for(long o=1;o<=E;o++)
          c[s][r][o]=(ini*rand())/RAND_MAX-ini; 

    return;
    }
    
void initialize1(){
    for(long s=1;s<=E;s++)
      for(long r=1;r<=Rr;r++)
        for(long o=1;o<=RE;o++)
          a1[s][r][o]=0;
    for(long s=1;s<=RE;s++)
      for(long r=1;r<=R;r++)
        for(long o=1;o<=RE;o++)
          b1[s][r][o]=0;
    for(long s=1;s<=RE;s++)
      for(long r=1;r<=Rr;r++)
        for(long o=1;o<=E;o++)
          c1[s][r][o]=0;
    return;
    }

double tensor(long s,long r,long o){
    double x=0;
    for(long r1=1;r1<=Rr;r1++)
        for(long o1=1;o1<=RE;o1++)
            for(long s1=1;s1<=RE;s1++)
            {
                x+=a[s][r1][o1]*b[s1][r][o1]*c[s1][r1][o];
                }
    return x;
    }
    
double norm(){
    double x=0;
    for(long s=1;s<=E;s++)
      for(long r=1;r<=Rr;r++)
        for(long o=1;o<=RE;o++)
          x+=a1[s][r][o]*a1[s][r][o];
    for(long s=1;s<=RE;s++)
      for(long r=1;r<=R;r++)
        for(long o=1;o<=RE;o++)
          x+=b1[s][r][o]*b1[s][r][o];
    for(long s=1;s<=RE;s++)
      for(long r=1;r<=Rr;r++)
        for(long o=1;o<=E;o++)
          x+=c1[s][r][o]*c1[s][r][o];
    return x;
    }

void renew(double y){
    for(long s=1;s<=E;s++)
      for(long r=1;r<=Rr;r++)
        for(long o=1;o<=RE;o++)
          a[s][r][o]+=a1[s][r][o]*y;
    for(long s=1;s<=RE;s++)
      for(long r=1;r<=R;r++)
        for(long o=1;o<=RE;o++)
          b[s][r][o]+=b1[s][r][o]*y;
    for(long s=1;s<=RE;s++)
      for(long r=1;r<=Rr;r++)
        for(long o=1;o<=E;o++)
          c[s][r][o]+=c1[s][r][o]*y;    
    
    return ;
    }

void evaluate(){
    double value[MAX_ENTITY],temp;
    long eva=0;
    for(long f=1;f<=50000;f++){
        long s=valid[f][0],r=valid[f][1],o=valid[f][2],num=0;
        temp=tensor(s,r,o);
        for(long o1=1;o1<=E;o1++){
          if(o1!=o&&tensor(s,r,o1)>temp) num++;
          if(num>=10) break; 
          }
        if(num<10) eva++;
        cout<<f<<' '<<eva<<'\n';
        }
    // fprintf(tp,"%f\n", (double)eva/50000);  
    cout<<"Ans= "<<(double)eva/50000<<'\n';
    system("pause");
    }

void training()
{
 //   double pre=-1*F*SMP;
 //   cout<<pre<<'\n';
    long times=0;
    while(1)
    {
        times++;
        double loss=0;
        initialize1();
        for(long f=1;f<=F*SMP;f++)
        {
            long temp=rand()/(double)RAND_MAX*F+1;
            if(temp<1||temp>F) continue;
            long s=train[temp][0], r=train[temp][1], o=train[temp][2];
            double x=tensor(s,r,o);
            if(x>1) continue;
            loss+=(x-1)*(x-1);
            double y=-2*(x-1);
            for(long r1=1;r1<=Rr;r1++)
              for(long o1=1;o1<=RE;o1++)
                for(long s1=1;s1<=RE;s1++)
                {
                    a1[s][r1][o1]+=y*b[s1][r][o1]*c[s1][r1][o];
                    b1[s1][r][o1]+=y*a[s][r1][o1]*c[s1][r1][o];
                    c1[s1][r1][o]+=y*b[s1][r][o1]*a[s][r1][o1];
                    }          
//            cout<<temp<<'\n';
            }
        for(long f=1;f<=F*SMP*Ita;f++)
        {
            long temp=rand()/(double)RAND_MAX*F+1;
            if(temp<1||temp>F) continue;
            long s=train[temp][0], r=train[temp][1], o=rand()/(double)RAND_MAX*E+1;
            if(o<1||o>E) continue;
            double x=tensor(s,r,o);
            double y=-2*(x+1)/Ita;
            if(x<-1) continue;
            loss+=(x+1)*(x+1)/Ita;
            for(long r1=1;r1<=Rr;r1++)
              for(long o1=1;o1<=RE;o1++)
                for(long s1=1;s1<=RE;s1++)
                {
                    a1[s][r1][o1]+=y*b[s1][r][o1]*c[s1][r1][o];
                    b1[s1][r][o1]+=y*a[s][r1][o1]*c[s1][r1][o];
                    c1[s1][r1][o]+=y*b[s1][r][o1]*a[s][r1][o1];
                    }
            }
        double x=norm(),y=0.01;
     //   if(loss>700) y=0.05;
        cout<<times<<' '<<loss<<' '<<x<<' '<<'\n';
        renew(y);
        if(times%10000==0){
            evaluate();
            }
        }
    return;
    }


int main()
{
    srand((unsigned)(time(NULL)));
    input();
    initialize();
    training();
    system("pause");
    return 0;
    }
