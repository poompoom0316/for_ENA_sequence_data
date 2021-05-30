// [[Rcpp::depends(RcppParallel)]]
#include <RcppParallel.h>
#include <Rcpp.h>
using namespace RcppParallel;
using namespace Rcpp;


// [[Rcpp::depends(RcppParallel)]]
struct parallelTOPH : public Worker {
  
  //inputmatrix
  const RMatrix<double> input_matrix;
  const RMatrix<double> parameter;
  const int ne;
  const int md;
  //outputmatrix
  RMatrix<double> res_mat;
  
  parallelTOPH(const NumericMatrix input_matrix,  
               const NumericMatrix parameter, const int ne, const int md, NumericMatrix res_mat): input_matrix(input_matrix),  
               parameter(parameter), ne(ne), md(md), res_mat(res_mat) {}
  
  void operator()(std::size_t begin, std::size_t end){
    for (std::size_t i=begin; i <end; i++){
        
        RMatrix<double>::Column input = input_matrix.column(i);
        
        double cumdvs, dvs, dvs1, dvs2;
        double g, a, b;
        double pb=0.0, pc=24.0, po, popb, pcpo, pratio, pd, gpd;
        double tb=8.0, tc=42.0, to, totb, tcto, tratio, td, ftd;
        int env, day, envmd;
        int nemd;
        int photosensitive;
        
        g = exp(parameter(0, i));
        a = exp(parameter(1, i));
        b = exp(parameter(2, i));
        po = parameter(3, i);
        to = parameter(4, i);
        popb = po - pb;
        pcpo = pc - po;
        pratio = pcpo / popb;
        totb = to - tb;
        tcto = tc - to;
        tratio = tcto / totb;
        dvs1 = 0.145*g + 0.005*g*g;
        dvs2 = 0.345*g + 0.005*g*g;
        
        nemd = ne * md;
        NumericVector output(ne);
        
        for (env = 0; env<ne; ++env)
        {
          envmd = env * md;
          
          for (day = 0, cumdvs = 0.0, photosensitive = 0; day < md; ++day)/* include the emergence day*/
          {
            if (photosensitive)
            {
              pd = input[nemd + envmd + day];
              if(pd<po) gpd = 1.0; else gpd = (pd - pb)/popb * pow((pc - pd)/pcpo, pratio);
              
              td = input[envmd + day];
              if(td<tb||td>tc) ftd = 0.0; else ftd = (td - tb)/totb * pow((tc - td)/tcto, tratio);
              dvs = pow(ftd, a)*pow(gpd, b);
            }
            else
            {
              td = input[envmd + day];
              if(td<tb||td>tc) ftd = 0.0; else ftd = (td - tb)/totb * pow((tc - td)/tcto, tratio);
              dvs = pow(ftd, a);
            }
            
            if ((cumdvs + dvs) >= dvs1 && (cumdvs + dvs) <= dvs2)
            {
              pd = input[nemd + envmd + day];
              if(pd<po) gpd = 1.0; else gpd = (pd - pb)/popb * pow((pc - pd)/pcpo, pratio);
              
              td = input[envmd + day];
              if(td<tb||td>tc) ftd = 0.0; else ftd = (td - tb)/totb * pow((tc - td)/tcto, tratio);        
              dvs = pow(ftd, a)*pow(gpd, b);
              photosensitive = 1;
            }
            else
            {
              td = input[envmd + day];
              if(td<tb||td>tc) ftd = 0.0; else ftd = (td - tb)/totb * pow((tc - td)/tcto, tratio);
              dvs = pow(ftd, a);
              photosensitive = 0;
            }
            cumdvs += dvs;
            if(cumdvs > g) break;
          }
          res_mat(env,i) = day + 1;
        }
        
      }
    }
  }; 

//Parallel for に回すための関数を作成
// [[Rcpp::export]]
NumericMatrix rcpp_ParallelTOPH(NumericMatrix input_matrix, NumericVector freevec, NumericMatrix parameter){
  //Matrix we will return
  int ne, ng, md;
  ne = (int)freevec(0);
  md = (int)freevec(1);
  ng = (int)input_matrix.ncol();
  
  NumericMatrix res_mat(ne, ng);
  
  parallelTOPH paralleltoph(input_matrix, parameter, ne, md, res_mat);
  
  parallelFor(0, ng, paralleltoph);
  
  return res_mat;
}

/*** R




*/
