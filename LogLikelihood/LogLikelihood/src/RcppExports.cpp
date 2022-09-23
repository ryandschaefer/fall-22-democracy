// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

#ifdef RCPP_USE_GLOBAL_ROSTREAM
Rcpp::Rostream<true>&  Rcpp::Rcout = Rcpp::Rcpp_cout_get();
Rcpp::Rostream<false>& Rcpp::Rcerr = Rcpp::Rcpp_cerr_get();
#endif

// log_likelihood
DataFrame log_likelihood(DataFrame t, String group, String word, String n);
RcppExport SEXP _LogLikelihood_log_likelihood(SEXP tSEXP, SEXP groupSEXP, SEXP wordSEXP, SEXP nSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< DataFrame >::type t(tSEXP);
    Rcpp::traits::input_parameter< String >::type group(groupSEXP);
    Rcpp::traits::input_parameter< String >::type word(wordSEXP);
    Rcpp::traits::input_parameter< String >::type n(nSEXP);
    rcpp_result_gen = Rcpp::wrap(log_likelihood(t, group, word, n));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_LogLikelihood_log_likelihood", (DL_FUNC) &_LogLikelihood_log_likelihood, 4},
    {NULL, NULL, 0}
};

RcppExport void R_init_LogLikelihood(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
