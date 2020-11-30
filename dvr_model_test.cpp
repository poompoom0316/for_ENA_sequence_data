#include <Rcpp.h>
#include <math.h>
using namespace Rcpp;

// This is a simple example of exporting a C++ function to R. You can
// source this function into an R session using the Rcpp::sourceCpp 
// function (or via the Source button on the editor toolbar). Learn
// more about Rcpp at:
//
//   http://www.rcpp.org/
//   http://adv-r.had.co.nz/Rcpp.html
//   http://gallery.rcpp.org/
//

// [[Rcpp::export]]
NumericVector timesTwo(NumericVector x) {
  return x * 2;
}

// [[Rcpp::export]]
int dvr_value(NumericVector Temp2,NumericVector Photo2, float g,float a,float b, int emergence){
  float DVS1;
  float DVS2;
  
  DVS1 = 0.145*g+0.005*g*g;
  DVS2 = 0.345*g+0.005*g*g;
  
  double dvs;
  int days;
  int max_date;
  dvs=0.0;  
  max_date=Temp2.length();
  
  // c++型でオブジェクトを取得しているならば、番号が0から始まる？
  for(int i=emergence-1; i<max_date; i++){

    if(dvs>=DVS1&&dvs<=DVS2){
      dvs+=pow(Temp2[i], a)*pow(Photo2[i], b);   
      days += 1;
    }else{
      dvs+=pow(Temp2[i], a);
      days += 1;
    }
    if(dvs>g) break;
    printf("%f", dvs);
  }
  
  return days;
}


// [[Rcpp::export]]
int dvr_value_module(NumericVector input, NumericVector freevec, NumericVector parameters){
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
  
  float g;
  float a;
  float b;
  int emergence;
  g = parameters[0];
  a = parameters[1];
  b = parameters[2];
  emergence = input[0];
  
  float DVS1;
  float DVS2;
  
  DVS1 = 0.145*g+0.005*g*g;
  DVS2 = 0.345*g+0.005*g*g;
  
  double dvs;
  int days;
  int max_date;
  dvs=0.0;  
  days = emergence;
  max_date=Temp2.length();
  
  // c++型でオブジェクトを取得しているならば、番号が0から始まる？
  for(int i=emergence; i<max_date; i++){
    
    if(dvs>=DVS1&&dvs<=DVS2){
      dvs+=pow(Temp2[i], a)*pow(Photo2[i], b);   
      days += 1;
    }else{
      dvs+=pow(Temp2[i], a);
      days += 1;
    }
    if(dvs>g) break;
    //printf("%f\n", dvs);
    //printf("%d\n", i);
  }
  
  return days;
}

// You can include R code blocks in C++ files processed with sourceCpp
// (useful for testing and development). The R code will be automatically 
// run after the compilation.
//

/*** R
## It seems to work
tp_pre=as.matrix(read.csv("Temp_mod.csv"))
photo_pre=as.matrix(read.csv("Photo_mod.csv"))
Emergence <- as.matrix(read.table("Data/BIL.emergencedate.txt"))

temp1=tp_pre[1:168,2]
photo1=photo_pre[1:168,2]
emg1=as.numeric(Emergence[1,1])

a=5.
b=5.
g=60.5

dvr_value(Temp2=temp1,Photo2=photo1, g=g,a=a,b=b, emergence=emg1)

input=c(emg1, temp1, photo1)
parameters=c(g, a, b)
freevec=c(1, 1+length(temp1), 1+length(temp1), 1+length(photo1)+length(temp1))

dvr_value_module(input=input, parameters=parameters, freevec=freevec)
*/
