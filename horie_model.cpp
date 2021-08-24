#include <Rcpp.h>
using namespace Rcpp;
using namespace std;

// [[Rcpp::export]]
int dvr_value(NumericVector Temp2,NumericVector Photo2, float g1, float g2, 
              float a1, float a2, float b1, float b2, float Th1, float Th2, 
              float Lc1, float Lc2, float DVI_star){

  double dvs;
  int days;
  int max_date;
  float value_i;
  dvs=0.0;  
  max_date=Temp2.length();
  
  // c++型でオブジェクトを取得しているならば、番号が0から始まる？
  for(int i=0; i<max_date; i++){
    
    if(dvs<=DVI_star){
      dvs+=1./(1+exp(-a1*(Temp2[i]-Th1)));
    } 
    else if(dvs>=DVI_star&&dvs<=g1){
      value_i = (1-exp(-b1*(Photo2[i]-Lc1))/(1+exp(-a1*(Temp2[i]-Th1))));
      if(value_i>0){
        dvs+=value_i;
      }else{
        dvs+=0;
      }
    }else{
      value_i = (1-exp(-b2*(Photo2[i]-Lc2))/(1+exp(-a2*(Temp2[i]-Th2))));
      if(value_i>0){
        dvs+=value_i;
      }else{
        dvs+=0;
      }
    }
    days+=1;
    if(dvs>g1+g2) break;
    printf("%f", dvs);
  }
  
  return days;
}


// [[Rcpp::export]]
int dvr_value_module(NumericVector input, NumericVector freevec, NumericVector parameter){
  IntegerVector posTemp2;
  IntegerVector posPhoto2;
  NumericVector Temp2;
  NumericVector Photo2; 
  posTemp2 = seq(freevec[0], freevec[1]-1);
  posPhoto2 = seq(freevec[2], freevec[3]-1);
  // Rf_PrintValue(posPhoto2);
  
  Temp2 = input[posTemp2];
  Photo2 = input[posPhoto2];
  
  Rf_PrintValue(input);
  Rf_PrintValue(Temp2);
  Rf_PrintValue(Photo2);

  float g1=exp(parameter(0));
  float g2=exp(parameter(1)); 
  float a1=exp(parameter(2));
  float a2=exp(parameter(3)); 
  float b1=exp(parameter(4));
  float b2=exp(parameter(5)); 
  float Th1=parameter(6); 
  float Th2=parameter(7); 
  float Lc1=parameter(8);
  float Lc2=parameter(9);
  float DVI_star=parameter(10);
  
  double dvs;
  int days;
  int max_date;
  dvs=0.0;  
  max_date=Temp2.length();
  
  // c++型でオブジェクトを取得しているならば、番号が0から始まる？
  days = dvr_value(Temp2,Photo2, g1=g1, g2=g2, 
                   a1=a1, a2=a2, b1=b1, b2=b2, Th1=Th1, Th2=Th2, 
                   Lc1=Lc1, Lc2=Lc2, DVI_star=DVI_star);
  
  return days;
}

