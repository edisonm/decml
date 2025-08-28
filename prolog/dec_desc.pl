/*  CLP over binary integer decimal numbers

    Author:        Edison Mera
    E-mail:        efmera@gmail.com
    WWW:           https://github.com/edisonm/decml
    Copyright (C): 2025, Process Design Center, Breda, The Netherlands.
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

    1. Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in
       the documentation and/or other materials provided with the
       distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
    FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
    COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
    LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
    ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.
*/

:- module(dec_desc, [dec_desc/3]).

dec_desc(pl_, [ acos,
                % acosh,
                asin,
                % asinh,
                atan,
                % atanh,
                cbrt,  cos,    cosh,
                % erf, erfc,
                exp10m1,
                exp10,
                % exp2,
                exp,
                expm1,
                % lgamma,
                log10,
                % log1p, log2,
                log,
                floor,
                ceil,
                round,
                sin,    sinh,  sqrt,
                % tan,
                tanh
                % tgamma
              ], 2).
dec_desc(pl_, [ add,
                % atan2,
                div,
                % fdim, hypot,
                mul,
                pow,
                % quantize,
                sub,
                max,
                min,
                mod
              ], 3).
dec_desc(pt_, [atom, string], 2).
% dec_desc(pi_, [rnint,  xrnint, xrninta, int,  xint,
%                floor,  xfloor, rninta,  ceil, xceil], 2).
dec_desc(is_, [ equal,
                less,
                greater,
                lessequal,
                greaterequal
              ], 2).
