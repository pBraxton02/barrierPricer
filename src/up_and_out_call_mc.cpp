#include <Rcpp.h>
using namespace Rcpp;

// Up-and-out call: payoff = max(S_T - K, 0) if max_t S_t < B, else 0
// Underlying: GBM under Q
// Discrete monitoring with nSteps (approx of continuous barrier)
//
// [[Rcpp::export]]
double up_and_out_call_mc(double S0,
                          double K,
                          double B,
                          double sigma,
                          double r,
                          double T,
                          int nSteps = 252,
                          int nPaths = 100000,
                          int seed = 123) {

  if (S0 <= 0 || K <= 0 || B <= 0 || sigma < 0 || T <= 0) stop("Invalid parameters.");
  if (nSteps <= 0 || nPaths <= 0) stop("nSteps and nPaths must be positive.");
  if (B <= S0) {
    // bariera jest już “dotknięta” na starcie (w praktyce opcja wybita)
    return 0.0;
  }

  RNGScope scope;          // ensures R RNG state is managed correctly
  Function set_seed("set.seed");
  set_seed(seed);

  const double dt = T / static_cast<double>(nSteps);
  const double drift = (r - 0.5 * sigma * sigma) * dt;
  const double vol_dt = sigma * std::sqrt(dt);

  double sum_payoff = 0.0;

  for (int i = 0; i < nPaths; ++i) {
    double S = S0;
    bool knocked_out = false;

    for (int j = 0; j < nSteps; ++j) {
      // Z ~ N(0,1) from R
      const double Z = R::rnorm(0.0, 1.0);
      S = S * std::exp(drift + vol_dt * Z);

      if (S >= B) {
        knocked_out = true;
        break;
      }
    }

    if (!knocked_out) {
      const double payoff = (S > K) ? (S - K) : 0.0;
      sum_payoff += payoff;
    }
  }

  const double disc = std::exp(-r * T);
  return disc * (sum_payoff / static_cast<double>(nPaths));
}
