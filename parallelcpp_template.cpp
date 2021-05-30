// [[Rcpp::depends(RcppParallel)]]
#include <RcppParallel.h>
#include <Rcpp.h>
using namespace RcppParallel;
using namespace Rcpp;


// [[Rcpp::depends(RcppParallel)]]
struct parallelTemplate : public Worker {
  
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
        

  }; 

//Parallel for に回すための関数を作成
// [[Rcpp::export]]
NumericMatrix rcpp_parallelTemplate(NumericMatrix input_matrix, NumericVector freevec, NumericMatrix parameter){
  //Matrix we will return
  int ne, ng, md;
  ne = (int)freevec(0);
  md = (int)freevec(1);
  ng = (int)input_matrix.ncol();
  
  NumericMatrix res_mat(ne, ng);
  
  parallelTemplate paralleltemplate(input_matrix, parameter, ne, md, res_mat);
  
  parallelFor(0, ng, paralleltoph);
  
  return res_mat;
}

/*** R




*/
