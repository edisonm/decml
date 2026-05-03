# decml

`decml` provides fixed-precision decimal floating-point domains for `clpcd` and
a low-level Prolog/C interface for IEEE 754 decimal BID-style values.

The pack currently exposes two CLP domains:

- `library(cddd)`: decimal64, mapped internally to `dec64`.
- `library(cdqd)`: decimal128, mapped internally to `dec128`.

This is not an arbitrary-precision library like MPFR. The intent is decimal64
and decimal128 arithmetic with Prolog CLP integration.

## Layout

- `prolog/cddd.pl` and `prolog/cdqd.pl` register the CLP domains and reexport `library(clpcd)`.
- `prolog/cddec.pl` maps CLP domain operations to decimal evaluation.
- `prolog/dec_desc.pl` is the source of truth for generated Prolog foreign predicates.
- `prolog/libdec.pl` builds/loads the foreign interface and includes generated `dec_auto.pl`.
- `include/` and `src/` implement the C decimal64/decimal128 arithmetic and functions.
- `tests/*.plt` run the CLP test suite through `clpcd_common`; `tests/*.c` exercise the C API.

Generated files are written under `bin/` inside this pack or under the workspace
`target/`; do not edit generated `*_auto.*`, `*.o`, `*.a`, or foreign wrapper
artifacts by hand.

## Build And Test

From the `plsteroids` workspace root:

```bash
make build
make decml/tests/cddd.plt
make decml/tests/cdqd.plt
```

From this directory, the C decimal library and C tests can be run with:

```bash
make all
make tests
```

## Low-Level Prolog API

Load the low-level predicates with:

```prolog
?- use_module(library(libdec)).
```

The generated predicates are prefixed by the format, for example `dec64_sqrt/2`
and `dec128_sqrt/2`. The conversion predicates are:

- `dec64/2`, `dec128/2`: cast a Prolog term into the native decimal term.
- `dec64_atom/2`, `dec128_atom/2`: convert a decimal term to/from an atom.
- `dec64_string/2`, `dec128_string/2`: convert a decimal term to/from a string.

Example:

```prolog
?- use_module(library(libdec)).
?- dec64('1.25', X), dec64_sqrt(Y, X), dec64_string(Y, S).
S = "1.118033988749895".
```

## Current Function Coverage

The same names are generated for `dec64_` and `dec128_` unless noted.

Unary functions:

- `abs`, `acos`, `asin`, `atan`, `cbrt`, `cos`, `cosh`, `exp10m1`, `exp10`, `exp`, `expm1`, `lgamma`, `log10`, `log`, `floor`, `ceil`, `round`, `sin`, `sinh`, `sqrt`, `tanh`

Binary functions:

- `add`, `atan2`, `div`, `mul`, `pow`, `sub`, `max`, `min`, `mod`

Comparisons:

- `equal`, `less`, `greater`, `lessequal`, `greaterequal`

Expression evaluation also maps common Prolog expressions such as `A+B`, `A-B`,
`A*B`, `A/B`, `A**B`, `A^B`, `atan(A,B)`, `ceiling(A)`, `integer(A)`, `e`, `pi`,
`epsilon`, and `sign(X)`.

## Current Limitations

- Correct rounding is not yet documented as a guarantee. Some C tests accept small tolerances, and `impl_dec_arithmetic.h` still has a TODO around multiplication overflow/rounding.
- Rounding modes, IEEE exception/status flags, and ternary inexact return values are not exposed. MPFR makes these semantics explicit for almost every operation.
- Some functions are visible as commented-out candidates in `dec_desc.pl` or `dec_pred.pl`, but are not exported: `acosh`, `asinh`, `atanh`, `erf`, `erfc`, `exp2`, `log1p`, `log2`, `tan`, `tgamma`, `hypot`, `fdim`, `quantize`, `copysign`, `nexttoward`.
- `inf` and `nan` exist in the C representation and string conversions, but there are no public Prolog classification predicates such as `dec64_nan/1`, `dec64_inf/1`, or `dec64_finite/1`.

## Recommended Function Roadmap

For MPFR-like breadth, prioritize functions that improve numerical robustness and
CLP usability before adding niche special functions.

1. Decimal semantics and observability:

- Public classification predicates: `nan`, `inf`, `finite`, `zero`, `integer`, `signbit`, `unordered`.
- Explicit rounding mode support and status flags for inexact, underflow, overflow, divide-by-zero, invalid/NaN.
- Next-representable and spacing operations: `nexttoward` or `nextafter`, `ulp`, `quantum`, `samequantum`, `quantize`, exponent/scale extraction.

2. Robust elementary functions:

- `log1p`, `log2`, `exp2`, `expm1`-style variants are important near zero and for avoiding cancellation. `expm1` exists; `log1p`, `log2`, and `exp2` are the obvious complements.
- `hypot` should be added before more exotic functions because stable norm computation is widely useful.
- `fma` is valuable for polynomial evaluation and can improve accuracy of later transcendental implementations.

3. Missing trigonometric and hyperbolic family members:

- `tan`, `acosh`, `asinh`, `atanh`.
- Optional reciprocal functions after the basics are stable: `sec`, `csc`, `cot`, `sech`, `csch`, `coth`.

4. Remainder and integer-related operations:

- Distinguish `mod`, `fmod`, IEEE `remainder`, and `remquo` semantics.
- Add truncation variants (`trunc`, round-to-nearest-even, round-toward-zero, round-up/down) rather than relying only on `floor`, `ceil`, and `round`.

5. Special functions:

- Complete the gamma family: `tgamma`/`gamma`, improve and expand `lgamma`, then consider `digamma`.
- Add `erf` and `erfc` for probability/statistics use.
- Later candidates from MPFR coverage: `beta`, incomplete gamma, `zeta`, Bessel functions (`j0`, `j1`, `jn`, `y0`, `y1`, `yn`), `eint`, `li2`.

6. Constants:

- Current expression evaluation covers `e`, `pi`, and `epsilon`.
- Useful additions aligned with MPFR-style APIs are `log2`, Euler's constant, and Catalan's constant.

The highest-impact order is: classification and flags, `log1p`/`log2`/`exp2`,
`hypot`, `fma`, missing inverse hyperbolic functions, then broader special
functions.
