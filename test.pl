piece(m1). piece(m2). piece(m3). piece(m4). piece(m5). piece(m6). piece(m7).
piece(m8). piece(m9).
piece(s1). piece(s2). piece(s3). piece(s4). piece(s5). piece(s6). piece(s7).
piece(s8). piece(s9). 
piece(p1). piece(p2). piece(p3). piece(p4). piece(p5). piece(p6). piece(p7). 
piece(p8). piece(p9). 
piece(z1). piece(z2). piece(z3). piece(z4). piece(z5). piece(z6). piece(z7). 

in_sequence(m1,m2). in_sequence(m2,m3). in_sequence(m3,m4). in_sequence(m4,m5). 
in_sequence(m5,m6). in_sequence(m6,m7). in_sequence(m7,m8). in_sequence(m8,m9).
in_sequence(s1,s2). in_sequence(s2,s3). in_sequence(s3,s4). in_sequence(s4,s5).
in_sequence(s5,s6). in_sequence(s6,s7). in_sequence(s7,s8). in_sequence(s8,s9).
in_sequence(p1,p2). in_sequence(p2,p3). in_sequence(p3,p4). in_sequence(p4,p5).
in_sequence(p5,p6). in_sequence(p6,p7). in_sequence(p7,p8). in_sequence(p8,p9).

sequence([H1,H2]):- in_sequence(H1,H2).
sequence([H1,H2|T1]):- in_sequence(H1,H2), sequence([H2|T1]).

series(P):-
  maplist(=(P1),P),
  piece(P1).

occurrences([], 0, _). 
occurrences([Match|T1], O, Match):- occurrences(T1, O0, Match), O is O0 + 1.
occurrences([H1|T1], O, Match):- occurrences(T1, O, Match), H1 \= Match.

max_occurrences(L,N,Match):-
  occurrences(L,O,Match),
  O =< N.

chi(C):- length(C,3), sequence(C).
pon(P):- length(P,3), series(P).
pair(P):- length(P,2), series(P).

set(S):- pon(S).
set(S):- chi(S).
set(S):- pair(S).
set(P,S):- pon(P), S = pon(P).
set(P,S):- chi(P), S = chi(P).
set(P,S):- pair(P), S = pair(P).


find_set([P1|T],[P1,P2,P3],R):-
  set([P1,P2,P3]),
  selectchk(P2,T,R0),
  selectchk(P3,R0,R).

find_set([P1|T],[P1,P1],R):-
  length(T,L),
  (L-1) mod 3 =:= 0,
  selectchk(P1,T,R),
  set([P1,P1]).

sets([],[],_).
sets(H,[S0|S1],S):-
  S0 = [P1|_],
  selectchk(P1,H,R0),
  find_set([P1|R0],S0,R),
  flatten([S0|S],L),
  maplist(max_occurrences(L,4),S0),
  sets(R,S1,[S0|S]).

valid_hand(Hand,Sets):-
  length(Hand,14),
  length(Sets,5),
  sets(Hand,Sets,[]).


