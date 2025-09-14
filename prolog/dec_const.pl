
do_eval(cputime, Type, C) :- do_eval_cputime(Type, C).
do_eval(epsilon, Type, C) :- do_eval_epsilon(Type, C).
do_eval(0,  Type, C) :- do_eval_z(Type, C).
do_eval(1,  Type, C) :- do_eval_1(Type, C).
do_eval(-1, Type, C) :- do_eval_m1(Type, C).
do_eval(e,  Type, C) :- do_eval_e(Type, C).
do_eval(pi, Type, C) :- do_eval_pi(Type, C).
do_eval(sign(X), Type, C) :-
    eval(Type, X, V),
    do_eval_z(Type, Z),
    ( compare_b(<, Type, Z, V)
    ->do_eval_1(Type, C)
    ; compare(Type, >, Z, X)
    ->do_eval_m1(Type, C)
    ; C = Z
    ).
do_eval(eval(Expr), Type, C) :- eval(Type, Expr, C).
do_eval(+(Expr), Type, C) :- eval(Type, Expr, C).
do_eval(-(Expr), Type, C) :- do_eval(0-Expr, Type, C).
do_eval(Expr, Type, C) :-
    expr_pred(Expr, Pred),
    Pred =.. [Name|Args],
    maplist(eval_1(Type), Args, EvalL, EAs),
    AC =.. [Name, Type, C|EAs],
    list_sequence(EvalL, EvalS),
    neckis,
    EvalS,
    AC.

reserve_eps(1000).

% table required, specially for epsilon, otherwise it will take 300 times
% more!!!

:- table
    do_eval_1/2,
    do_eval_m1/2,
    do_eval_e/2,
    do_eval_pi/2,
    do_eval_epsilon/2.

do_eval_1(Type, C) :-
    cd_type(Type),
    cast(Type, 1, C).

do_eval_m1(Type, C) :-
    cd_type(Type),
    cast(Type, -1, C).

do_eval_e(Type, C) :-
    cd_type(Type),
    do_eval_1(Type, F1),
    exp(Type, C, F1).

do_eval_pi(Type, C) :-
    cd_type(Type),
    do_eval(4*atan(1), Type, C).

do_eval_epsilon(Type, E) :-
    cd_type(Type),
    eval(Type, '0.1', P),
    do_eval_1(Type, O),
    once(( between(1,200,X),
           do_eval(P^X, Type, E),
           do_eval(O+E, Type, Y),
           compare_b(=, Type, Y, O)
         )).
