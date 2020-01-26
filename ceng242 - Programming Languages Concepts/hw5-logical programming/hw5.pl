:- module(hw5,[configuration/3]).
:- [ hardware ] .


configuration([],[],[]):- !.
configuration(X,Y,Puts) :- helper(X,Y,Puts,[]).

helper([],_,[],_) :- !.
helper([Head|Tail],Cons,[X|Y],Puts) :- sections(Secs) ,
                                       letsee(Head, Secs, Puts, Cons, B) ,
                                       X =put(Head,B), 
                                       helper(Tail, Cons, Y, [X|Puts]).

out(Head,[outer_edge(Head)|_]):-!.
out(Head,[_|T]):-out(Head,T). 


letsee(Head,[A|T],Puts,Cons,B) :- ayni(A,Puts), !, letsee(Head,T,Puts,Cons,B).
letsee(Head,[A|T],Puts,Cons,B) :- out(Head, Cons), constraint(A), !, letsee(Head,T,Puts,Cons,B).
letsee(Head,[A|T],Puts,Cons,B) :- isclose(Head, Puts, Cons, A), !, letsee(Head,T,Puts,Cons,B).
letsee(_,[A|_],_,_,A).
letsee(Head,[_|T],Puts,Cons,A) :- letsee(Head,T,Puts,Cons,A).

ayni(A,[put(_,A)|_]):-!.
ayni(A,[_|T]) :- ayni(A,T).

constraint(B) :- findall(X,(adjacent(B,X)),L1), 
                 findall(Y,(adjacent(Y,B)),L2), 
                 append(L1,L2,L), 
                 length(L,Ln),
                 Ln==4.

isclose(Head,Puts,[close_to(X,Head)|_],A) :- init(Puts,X,A).
isclose(Head,Puts,[close_to(Head,X)|_],A) :- init(Puts,X,A).
isclose(Head,Puts,[_|Cons],A) :- isclose(Head,Puts,Cons,A).

init([put(X,Yer)|_],X,A) :- findall(S1,adjacent(Yer,S1),L1),
                            findall(S2,adjacent(S2,Yer),L2),
                            append(L1,L2,L),
                            not(member(A,L)).
init([_|Puts],X,A) :- init(Puts,X,A).