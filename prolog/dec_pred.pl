expr_pred(atan(A, B), atan2(A, B)).
% expr_pred(copysign(A, B), copysign(A, B)).
expr_pred(A/B, div(A,B)).
expr_pred(A**B, pow(A,B)).
% expr_pred(float(A), float(A)).
% expr_pred(float_fractional_part(A), float_fractional_part(A)).
% expr_pred(float_integer_part(A), float_integer_part(A)).
expr_pred(A^B, pow(A, B)).
% expr_pred(inf, inf).
expr_pred(A-B, sub(A, B)).
% expr_pred(nan, nan).
% expr_pred(nexttoward(A, B), nexttoward(A, B)).
expr_pred(A+B, add(A, B)).
% expr_pred(random_float, random_float).
expr_pred(A*B, mul(A, B)).
expr_pred(Pred, Pred) :-
    member(Desc, [pl_, pn_]),
    desc(Desc, FL, A1),
    member(F, FL),
    succ(A, A1),
    functor(Pred, F, A),
    \+ expr_pred(Pred, _),
    neck.
