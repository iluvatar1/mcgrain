// Suma de Fuerzas sobre un grano, con sólo fuerzas normales
#include <iostream>
#include <cmath>
#include <vector>
#include"Random.h"
using namespace std;

const int Z=3;
const double Alphaxx=10.0;
const double Alphayy=10.0;
const double Alphaxy=0.0;
const double Twidth=0.1;

const double g60=M_PI/3;
const double g240=4*M_PI/3;
const double TwoPI=2*M_PI;

class Grano{
private:
  double Theta[Z],CosTheta[Z],SinTheta[Z];
  double * Fn,* Fnnew;
public:
  Grano(void);
  ~Grano(void);
  void SwitchForces(void);
  void SetInitialForces(double F0,Crandom & ran2);
  void GenerateAngles(Crandom & ran2);
  void GenerateNewForces(double a,Crandom & ran2);
  void MonteCarloStep(double a,double alphaxx,double alphayy,double alphaxy,Crandom & ran2);
  double GetSigmaxx(void);
  double GetSigmayy(void);
  double GetSigmaxy(void);
  double GetSigmaTrace(void){return GetSigmaxx()+GetSigmayy();};
  double GetSigmaDeterminant(void){return GetSigmaxx()*GetSigmayy()-pow(GetSigmaxy(),2);};
  void ShowAngles(void);
  void ShowFn(void);
  double GetSumFx(void);
  double GetSumFy(void);
};
Grano::Grano(void){
  Fn=new double[Z];  Fnnew=new double[Z];
}
Grano::~Grano(void){
  delete [] Fn;  delete [] Fnnew;
}
void Grano::SwitchForces(void){
  double * aux;
  aux=Fn; Fn=Fnnew; Fnnew=aux;
}
void Grano::GenerateAngles(Crandom & ran2){
  int i; double Shift;
  Theta[0]=0;
  for(i=1;i<Z;i++)
    Theta[i]=Theta[i-1]+g60+ran2.r()*(g240-Theta[i-1]);
  Shift=TwoPI*ran2.r();
  for(i=0;i<Z;i++){
    Theta[i]+=Shift; if(Theta[i]>TwoPI) Theta[i]-=TwoPI;
  }
  for(i=0;i<Z;i++){
    CosTheta[i]=cos(Theta[i]);  SinTheta[i]=sin(Theta[i]);
  }
}
void Grano::SetInitialForces(double F0,Crandom & ran2){
  double Ax,Ay,det;  int origin,i,is,Zm1,Zm2; //is=ishifted
  //Escoger al azar por donde comenzar
  origin=(int) Z*ran2.r(); Zm1=(origin-1+Z)%Z; Zm2=(origin-2+Z)%Z;
  //Asignar el valor de entrada como el valor inicial esde las primeras Z-2 fuerzas
  for(Ax=0,Ay=0,i=0;i<Z-2;i++){
    is=(origin+i)%Z;
    Fn[is]=F0;
    Ax+=Fn[is]*CosTheta[is];  Ay+=Fn[is]*SinTheta[is];
  }
  //Las dos fuerzas restantes se calculan con las restricciones fuertes:  
  det=CosTheta[Zm2]*SinTheta[Zm1]-CosTheta[Zm1]*SinTheta[Zm2];
  if(det!=0){
    Fn[Zm2]=(Ay*cos(Theta[Zm1])-Ax*sin(Theta[Zm1]))/det;
    Fn[Zm1]=(Ax*sin(Theta[Zm2])-Ay*cos(Theta[Zm2]))/det;
  }
  else{
    Fn[Zm2]=F0;
    Fn[Zm1]=F0;
  }
}
void Grano::GenerateNewForces(double a,Crandom & ran2){
  double Ax,Ay,det;  int origin,i,is,Zm1,Zm2; //is=ishifted
  //Escoger al azar podr donde comenzar
  origin=(int) Z*ran2.r(); Zm1=(origin-1+Z)%Z; Zm2=(origin-2+Z)%Z;
  //Escoger al azar nuevos valores de las primeras Z-2 fuerzas (cumpliendo desigualdades)
  for(Ax=0,Ay=0,i=0;i<Z-2;i++){
    is=(origin+i)%Z;
    Fnnew[is]=Fn[is]+a*(2*ran2.r()-1); if(Fnnew[is]<0) Fnnew[is]=Fn[is];
    Ax+=Fnnew[is]*CosTheta[is];  Ay+=Fnnew[is]*SinTheta[is];
  }
  //Las dos fuerzas restantes se calculan con las restricciones fuertes:  
  det=CosTheta[Zm2]*SinTheta[Zm1]-CosTheta[Zm1]*SinTheta[Zm2];
  if(det!=0){
    Fnnew[Zm2]=(Ay*cos(Theta[Zm1])-Ax*sin(Theta[Zm1]))/det;
    Fnnew[Zm1]=(Ax*sin(Theta[Zm2])-Ay*cos(Theta[Zm2]))/det;
  }
  else{
    Fnnew[Zm2]=Fn[Zm2]+a*(2*ran2.r()-1); if(Fnnew[Zm2]<0) Fnnew[Zm2]=Fn[Zm2];
    Fnnew[Zm1]=Fn[Zm1]+a*(2*ran2.r()-1); if(Fnnew[Zm1]<0) Fnnew[Zm1]=Fn[Zm1];
  }
}
void Grano::MonteCarloStep(double a,double alphaxx,double alphayy,double alphaxy,Crandom & ran2){
  double sigmaxxnew,sigmaxx,dsigmaxx,sigmayynew,sigmayy,dsigmayy,sigmaxynew,sigmaxy,dsigmaxy;
  double AcceptRate;
  //Choose New Forces
  GenerateNewForces(a,ran2);
  //Compute the Accepting Rate
  sigmaxx=GetSigmaxx();  sigmayy=GetSigmayy();  sigmaxy=GetSigmaxy();
  SwitchForces();
  sigmaxxnew=GetSigmaxx();  sigmayynew=GetSigmayy();  sigmaxynew=GetSigmaxy();
  SwitchForces();
  dsigmaxx=sigmaxxnew-sigmaxx;  dsigmayy=sigmayynew-sigmayy;  dsigmaxy=sigmaxynew-sigmaxy;
  AcceptRate=exp(-alphaxx*dsigmaxx-alphayy*dsigmayy-alphaxy*dsigmaxy);
  //Perform a Metropolis acceptance procedure
  if(AcceptRate>1.0)
    SwitchForces();
  else if(ran2.r()<AcceptRate)
    SwitchForces();
}
double Grano::GetSigmaxx(void){
  int i; double suma;
  for(suma=0,i=0;i<Z;i++)
    suma+=Fn[i]*CosTheta[i]*CosTheta[i];
  return suma;
}
double Grano::GetSigmayy(void){
  int i; double suma;
  for(suma=0,i=0;i<Z;i++)
    suma+=Fn[i]*SinTheta[i]*SinTheta[i];
  return suma;
}
double Grano::GetSigmaxy(void){
  int i; double suma;
  for(suma=0,i=0;i<Z;i++)
    suma+=Fn[i]*CosTheta[i]*SinTheta[i];
  return suma;
}
void Grano::ShowAngles(void){
  for(int i=0;i<Z;i++)
    cout<<Theta[i]*180/M_PI<<" ";
  cout<<endl;
}
void Grano::ShowFn(void){
  for(int i=0;i<Z;i++)
    cout<<Fn[i]<<" ";
  cout<<endl;
}
double Grano::GetSumFx(void){
  int i; double suma;
  for(suma=0,i=0;i<Z;i++)
    suma+=Fn[i]*CosTheta[i];
  return suma;
}
double Grano::GetSumFy(void){
  int i; double suma;
  for(suma=0,i=0;i<Z;i++)
    suma+=Fn[i]*SinTheta[i];
  return suma;
}

const int NForceRuns=10000;

int main(){
  Grano Pepa;
  Crandom ran2(0);
  int tF;

  Pepa.GenerateAngles(ran2);
  Pepa.SetInitialForces(1.0,ran2);
  for(tF=0;tF<NForceRuns;tF++){
    cout<<tF<<" "<<Pepa.GetSigmaxx()<<" "<<Pepa.GetSigmayy()<<" "<<Pepa.GetSigmaxy()<<endl;
    //    cout<<tF<<" "<<Pepa.GetSigmaTrace()<<" "<<Pepa.GetSigmaDeterminant()<<endl;
    Pepa.MonteCarloStep(Twidth,Alphaxx,Alphayy,Alphaxy,ran2);
  }
  
  return 0;
}
