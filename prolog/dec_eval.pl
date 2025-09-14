/*  CLP over binary integer decimal numbers

    Author:        Edison Mera
    E-mail:        efmera@gmail.com
    WWW:           https://github.com/edisonm/assertions
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

:- module(dec_eval, [int/3]).

:- use_module(library(lists)).
:- use_module(library(libdec)).
:- use_module(library(compare_eq)).
:- use_module(library(neck)).
:- use_module(library(list_sequence)).
:- use_module(library(dec_desc)).
:- init_expansors.

cd_type(dec64).
cd_type(dec128).

cd_prefix(Type, Type, []) :-
    cd_type(Type),
    neck.

int(dec64,  A, B) :- dec64_round( A, B).
int(dec128, A, B) :- dec128_round(A, B).

op_pred(=,  equal).
op_pred(=<, lessequal).
op_pred(>=, greaterequal).
op_pred(<,  less).
op_pred(>,  greater).
% op_pred(\=, not_equal).

is_dec64(X) :- dec64_t(X).

is_dec128(X) :- dec64_t(X).
is_dec128(X) :- dec128_t(X).

desc(Desc, FL, A) :- dec_desc(Desc, FL, A).

expr_pred(ceiling(A), ceil(A)).
expr_pred(integer(A), round(A)).

:- include(library(dec_pred)).
:- include(library(eval)).
:- include(library(dec_const)).
