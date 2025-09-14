/*  CLP over binary integer decimal numbers

    Author:        Edison Mera
    E-mail:        efmera@gmail.com
    WWW:           https://github.com/edisonm/assertions
    Copyright (C): 2020, Process Design Center, Breda, The Netherlands.
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

:- module(gen_dec,
          [ gen/1
          ]).

:- use_module(library(filesex)).
:- use_module(library(lists)).
:- use_module(library(assertions)).
:- init_expansors.

:- multifile prefix_type/3.
:- multifile desc/4.

gen(Dec) :-
    absolute_file_name(plbin('.'), Dir, [file_type(directory), access(exist)]),
    gen_pl(Dec, Dir),
    gen_h(Dec, Dir).

gen_pl(Dec, Dir) :-
    atom_concat(Dec, '_auto.pl', Name),
    directory_file_path(Dir, Name, File),
    tell(File),
    dump_pl(Dec),
    told.

gen_h(Dec, Dir) :-
    format(atom(Name), "pl-~w_auto.h", [Dec]),
    directory_file_path(Dir, Name, File),
    tell(File),
    dump_h(Dec),
    told.

dump_h(Dec) :-
    ( desc(Dec, Prefix, FL, A),
      member(F, FL),
      format("GEN_~w_ALL(~w~w,~w)~n", [Dec, Prefix, A, F]),
      fail
    ; true
    ).

compats(1, T, -T) :- !.
compats(N, T, (Cs * +T)) :-
    N > 1,
    succ(N1, N),
    compats(N1, T, Cs).

compats(pl_, A, T, Cs) :- compats(A, T, Cs).
compats(pn_, A, T, Cs) :- compats(A, T, Cs).
compats(pt_, 2, T, +T * -).
compats(pi_, 2, T, -int* +T).
compats(is_, 2, T, +T* +T).

dump_pl(Dec) :-
    ( prefix_type(Dec, Pre, T),
      desc(Dec, Prefix, FL, A),
      compats(Prefix, A, T, Cs),
      findall(Func/A,
              ( member(F, FL),
                atomic_list_concat([Pre, '_', F], Func)
              ), PIL),
      forall(member(PI, PIL), portray_clause((:- export(PI)))),
      portray_clause((:- pred PIL :: Cs + native(prefix(Prefix)))),
      fail
    ; true
    ).
