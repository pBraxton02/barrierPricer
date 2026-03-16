# barrierPricer

`barrierPricer` is a lightweight R package for pricing an **up-and-out European call option** with a **Monte Carlo simulation** engine implemented in **C++ via Rcpp**. The repository currently exposes a single user-facing R function, `price_up_and_out_call()`, which calls the underlying `up_and_out_call_mc()` routine compiled from `src/up_and_out_call_mc.cpp`.

## Features

- Prices an **up-and-out call** under a **geometric Brownian motion** assumption.
- Uses **risk-neutral Monte Carlo simulation** with **discrete barrier monitoring**.
- Implements the pricing engine in **C++** for speed using **Rcpp**.
- Provides a simple R wrapper for use in scripts and analysis workflows.

## Model assumptions

The pricing engine in this repository assumes:

- The underlying asset follows **GBM under the risk-neutral measure**.
- The barrier is monitored at `nSteps` discrete time steps, so it is an approximation to continuously monitored barrier behavior.
- If the simulated path touches or exceeds the barrier `B`, the option is knocked out and the payoff becomes zero.
- If `B <= S0`, the option is treated as already knocked out at inception and the function returns `0`.

## Installation

This repository looks like an R package skeleton with `DESCRIPTION`, `NAMESPACE`, `R/`, and `src/` directories already present, but the package metadata in `DESCRIPTION` is still placeholder text and should be updated before publishing or wider reuse. 

You can install locally with:

```r
# from the project root
install.packages("Rcpp")

# build/install as a local package
# in R:
devtools::install_local(".")
```

Or source it during development.

## Usage

```r
library(barrierPricer)

price_up_and_out_call(
  S0 = 100,
  K = 95,
  B = 130,
  sigma = 0.20,
  r = 0.05,
  T = 1,
  nSteps = 252,
  nPaths = 100000,
  seed = 123
)
```

## Function reference

### `price_up_and_out_call()`

Prices an up-and-out European call option using Monte Carlo simulation.

#### Arguments

- `S0`: initial asset price
- `K`: strike price
- `B`: up-and-out barrier level
- `sigma`: volatility
- `r`: continuously compounded risk-free rate
- `T`: time to maturity in years
- `nSteps`: number of monitoring steps in each simulated path
- `nPaths`: number of Monte Carlo paths
- `seed`: random seed for reproducibility 

#### Returns

A single numeric value: the discounted Monte Carlo estimate of the option price.

## Project structure

```text
barrierPricer/
├── DESCRIPTION
├── NAMESPACE
├── R/
│   ├── RcppExports.R
│   └── pricing.R
└── src/
    ├── RcppExports.cpp
    └── up_and_out_call_mc.cpp
```

This structure is reflected directly in the repository tree. 

## Notes

A few things in the repo still look unfinished:

- `DESCRIPTION` still contains placeholder title, author, description, and license fields.
- There is no meaningful repository description on GitHub yet.
- The package currently appears focused on one barrier option pricer rather than a broader pricing library. 

## Roadmap ideas

Potential next improvements:

- Add support for **down-and-out**, **up-and-in**, and **down-and-in** barrier options.
- Add **put** payoffs.
- Add variance reduction techniques such as antithetic variates or control variates.
- Add unit tests and benchmarks.
- Replace placeholder package metadata and add examples/vignettes.

## License

No concrete license is currently specified in `DESCRIPTION`; the file still contains a placeholder. You should choose and declare a license before distributing the package more broadly.
