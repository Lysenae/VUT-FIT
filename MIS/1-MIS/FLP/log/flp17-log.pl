% Projekt: FLP #2 - Turingov Stroj
% Autor:   Daniel Klimaj (xklima22@stud.fit.vutbr.cz)

:- dynamic rule/4.

% Vstupny bod programu
main :- parse_stdin(Config), !, tm_perform(Config), halt.

% Spracovanie nacitanych udajov - zostavenie pasky a pravidiel TS.
% Config - vystpuny parameter pre konfiguraciu TS
parse_stdin(Config) :-
  prompt(_, ''),
  read_lines(Lines),
  get_config(Lines, Config),
  create_rules(Lines).

% Ziska pociatocnu konfiguraciu TS.
% In     - zoznam
% Config - vystupny parameter pre konfiguraciu
get_config(In, Config) :-
  last(In, C),
  Config = ['S'|C]. % Nastavi pociatocny stav

% Zostavi pravidla.
create_rules(In) :-
  last(In, Last),
  delete(In, Last, In2), % Odstran pasku
  create_rules2(In2).

% Zostavi pravidla zo stvorprvkoveho zoznamu.
create_rules2([]).
create_rules2([H|T]) :-
  nth0(0, H, SttP), % StatePresent  - sucasny stav
  nth0(2, H, SymP), % SymbolPresent - sucasny synbol
  nth0(4, H, SttN), % StateNew      - novy stav
  nth0(6, H, SymN), % SymbolNew     - novy symbol, resp. posun
  assert(rule(SttP, SymP, SttN, SymN)),
  create_rules2(T).

% Vypise aktualnu konfiguraciu.
% Config - konfiguracia TS
print_config(Config) :-
  atomic_list_concat(Config, '', S),
  atom_string(S, R),
  writeln(R).

% Vypise zoznam konfiguracii, kazdu na novy riadok.
% Configs - zoznam konfiguracii
print_configs([]).
print_configs([H|T]) :- print_config(H), print_configs(T).

% Najde aktualnu poziciu hlavy TS.
% Config - konfiguracia TS
% Pos    - vystupny parameter pre poziciu hlavy na paske
pos(Config, Pos) :-
  length(Config, Len),
  L is Len - 1,
  pos2(Config, L, P),
  Pos is P.

% Vyhlada velke pismeno, ktore znaci poziciu hlavy a vrati jeho poziciu v retazci.
% Config - konfiguracia TS
% CurPos - aktualne kontrolovana pozicia
% OutPos - vystupny parameter pre poziciu hlavy
pos2(Config, CurPos, OutPos) :-
  nth0(CurPos, Config, N),
  ( (char_type(N, upper), OutPos is CurPos);
    ( NewPos is CurPos - 1,
      pos2(Config, NewPos, OutPos)
    )
  ).

% Ziska aktualny stav TS.
% Config - konfiguracia TS
% State  - vystupny parameter pre stav
state(Config, State) :-
  pos(Config, Pos),
  nth0(Pos, Config, State).

% Ziska retazec pred a za hlavou TS.
% Config - prvy parameter, konfiguracia TS
% L      - vystupny parameter pre retazec za (nalavo) hlavou
% R      - vystupny parameter pre retazec pred (napravo) hlavou
split_config([],[],[]).
split_config([H|T],L,R) :- char_type(H, upper),R = T, L = [].
split_config([H|T],L,R) :-
  \+ char_type(H, upper),
  split_config(T,X,R),
  append([H],X,L).

% Ziska symbol pod hlavou TS.
% Config - konfiguracia TS
% Sym    - vystupny parameter pre symbol pod hlavou TS
symbol(Config, Sym) :-
  split_config(Config, _, R),
  first(R, Sym).

% Ziska vlastnosti konfiguracie - stav, retazec pred a za hlavou, poziciu.
% Config - konfiguracia TS
% State  - vystupny parameter pre stav TS
% Left   - vystupny parameter pre retazec za (nalavo) hlavou
% Right  - vystupny parameter pre retazec pred (napravo) hlavou
% Symbol - vystupny parameter pre symbol pod hlavou TS
config(Config, State, Left, Right, Symbol) :-
  state(Config, State),
  split_config(Config, Left, Right),
  symbol(Config, Symbol).

% Zisti, ci je zoznam prazdny.
% L - zoznam
is_empty(L) :-
  length(L, Len),
  Len == 0.

% Odstrani N-ty prvok zoznamu.
% InList  - vstupny zoznam
% N       - pozicia odstranovaneho prvku
% OutList - vystupny parameter pre vystupny zoznam
delete_nth(InList, N, OutList) :- delete_nth2(InList, N, 0, [], OutList).

% Vid. delete_nth.
% CN      - aktualny prvok
% TmpList - docasny zoznam
delete_nth2([HI|TI], N, CN, TmpList, OutList) :-
  ( N == CN,
    append(TmpList, TI, OutList)
  );
  ( N \== CN,
    CNx is CN + 1,
    append(TmpList, [HI], TL),
    delete_nth2(TI, N, CNx, TL, OutList)
  ).

% Odstrani posledny prvok zoznamu.
% InList  - vstupny zoznam
% OutList - vystupny zoznam
delete_last([], []).
delete_last(InList, OutList) :-
  length(InList, Len),
  L is Len-1,
  delete_nth(InList, L, OutList).

% Vid. delete_last.
delete_first([], []).
delete_first(InList, OutList) :- delete_nth(InList, 0, OutList).

% Ziska prvy prvok zoznamu alebo vrati medzeru (blank symbol).
first([], ' ').
first([H|_], C) :- C = H.

% Vypocet TS.
% Config - vstupna konfiguracia
tm_perform(Config) :-
  config(Config, State, _, _, Symbol), !,
  ( tm_step(State, Symbol, Config, []);
    false                                  % Abnormalne zastavenie
  ).

% Krok vypoctu TS.
% State      - aktualny stav TS
% Symbol     - symbol pod hlavou TS
% InConfig   - vstupna konfiguracia TS
% OutConfigs - zoznam vystupnych konfiguracii
tm_step('F', _, InConfig, OutConfigs) :-
  print_configs(OutConfigs),
  print_config(InConfig),
  !.

tm_step(State, Symbol, InConfig, OutConfigs) :-
  append(OutConfigs, [InConfig], OCs),
  rule(State, Symbol, NewState, Action),
  ( % Ak sa dookola opakuje ta ista akcia, skus nahodne vybrat inu
    ( State == NewState,
      (Symbol == Action; Action == 'R'; Action == 'L'),
      tm_choose_rule(State, Symbol, NS, NA),
      tm_action(NA, NS, InConfig, OC),
      config(OC, NStt, _, _, NSym),
      tm_step(NStt, NSym, OC, OCs)
    );
    (
      tm_action(Action, NewState, InConfig, OC),
      config(OC, NStt, _, _, NSym),
      tm_step(NStt, NSym, OC, OCs)
    )
  ).

% Vyberie pravidlo lisiace sa od predosleho pouziteho, ak existuje take pravidlo.
% State     - aktualy stav TS
% Symbol    - symbol pod hlavou TS
% NewState  - vystupny parameter pre novy symbol
% NewSymbol - vystupny parameter pre symbol alebo posun
tm_choose_rule(State, Symbol, NewState, NewSymbol) :-
  findall([NS, A], rule(State, Symbol, NS, A), Rules),
  length(Rules, Len),
  (
    ( Len > 1,
      tm_choose_rule2(Rules, State, Symbol, NewState, NewSymbol)
    );
    ( Len == 1,
      nth0(0, Rules, Rule),
      nth0(0, Rule, NewState),
      nth0(1, Rule, NewSymbol)
    );
    ( % Ziadne odpovedajuce pravidlo nebolo najdene
      false
    )
  ).

tm_choose_rule2([], _, _, _, _) :- false.
tm_choose_rule2([H|T], State, Symbol, NewState, NewSymbol) :-
  nth0(0, H, Stt),
  nth0(1, H, Sym),
  (
    ( Stt \== State,
      Sym \== Symbol,
      NewState = Stt,
      NewSymbol = Sym
    );
    tm_choose_rule2(T, State, Symbol, NewState, NewSymbol)
  ).

% Posun hlavy TS alebo zapis znaku na poziciu hlavy.
% Sym       - operacia (L,R) alebo symbol
% State     - novy stav
% Inconfig  - vstupna konfiguracia
% OutConfig - vystupna konfiguracia
tm_action('L', State, InConfig, OutConfig) :- % Posun hlavy dolava
  split_config(InConfig, L, R),
  \+ is_empty(L),                             % Chyba pri zapise za lavy okraj
  last(L, LLast),
  delete_last(L, LNew),
  append(LNew, [State], Tmp1),                % Skladanie novej konfiguracie
  append(Tmp1, [LLast], Tmp2),
  append(Tmp2, R, OutConfig).

tm_action('R', State, InConfig, OutConfig) :- % Posun hlavy doprava
  split_config(InConfig, L, R),
  first(R, RFirst),                           % Bud ziska znak alebo blank
  delete_first(R, RNew),
  append(L, [RFirst], Tmp1),                  % Skladanie novej konfiguracie
  append(Tmp1, [State], Tmp2),
  append(Tmp2, RNew, OutConfig).

tm_action(Sym, State, InConfig, OutConfig) :- % Zapis symbolu
  char_type(Sym, lower),                      % Kontrola, ci je to symbol
  split_config(InConfig, L, R),
  delete_first(R, RNew),
  append(L, [State], Tmp1),                   % Skladanie novej konfiguracie
  append(Tmp1, [Sym], Tmp2),
  append(Tmp2, RNew, OutConfig).

% ##############################################################################
% Tato cast je prebrana z input2.pl
% ##############################################################################

%Reads line from stdin, terminates on LF or EOF.
read_line(L, C) :-
  get_char(C),
  (isEOFEOL(C), L = [], !;
    read_line(LL, _),% atom_codes(C,[Cd]),
    [C|LL] = L).

%Tests if character is EOF or LF.
isEOFEOL(C) :-
  C == end_of_file;
  (char_code(C, Code), Code==10).

read_lines(Ls) :-
  read_line(L, C),
  ( C == end_of_file, Ls = [] ;
    read_lines(LLs), Ls = [L|LLs]
  ).

% ##############################################################################
