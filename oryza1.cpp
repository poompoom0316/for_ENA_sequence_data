#include <Rcpp.h>
#include <cmath>
using namespace Rcpp;

// [[Rcpp::export]]
float huh(float Td, float Tbase, float Topt, float Thigh){
  float huh;
  
  if((Td >= Tbase)&&(Td <= Thigh)){
    huh = (Topt - (Td - Topt) * (Topt - Tbase) / (Thigh - Topt)) / 24;
  }
  else if((Tbase < Td) && (Td <= Topt)){
    huh = (Td - Tbase) / 24;
  }
  else{
    huh = 0.0;
  }
  return huh;
}

float hu_func(float Tmax, float Tmin, float Tbase, float Topt, float Thigh){
  float hu_v;
  float td;
  
  hu_v = 0.;
  
  for(int i=0; i<24; i++){
    td =  (Tmin + Tmax) / 2 + (Tmax - Tmin) * cos(0.2618 * (i - 14)) / 2;
    hu_v += huh(td, Tbase=Tbase, Topt=Topt, Thigh=Thigh);
  }

  return hu_v;
}


float dvr_func(float DAS, float DVS, float DVRJ, float DVRI, float DVRP, float DVRR, float HU, 
               float DAYL, float MOPP, float PPSE, float TS, float SHCKD, float DOYTR, float DOYS, float TSTR){
  float ISA;
  float DVR;
  float DL;
  float PPFAC;
   
  ISA = DOYTR - DOYS;
  
  if((DVS >= 0) && (DVS < 0.4)){
    DVR = DVRJ * HU;
  }
  else if ((DVS >= 0.4) && (DVS < 0.65)){
    DL = DAYL + 0.9;
    PPFAC = std::min(1, 1 - (DL - MOPP) * PPSE);
    PPFAC = min(1., std::max(0., PPFAC))
    DVR = DVRI * HU * PPFAC
  } 
  
  else if((DVS >= 0.65) & (DVS < 1.0)){
    DVR = DVRP * HU;
  }
  else if(DVS > 1.){
    DVR = DVRR * HU;
  }
    
  if(DAS == ISA){
    TSTR = TS;
    TSHCKD = SHCKD * TSTR;
  }

  if((DAS > ISA) && (TS < (TSTR + TSHCKD))){
    DVR = 0
  }
    
  
  return DVR, HU
}
