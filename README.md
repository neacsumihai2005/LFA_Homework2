# TEMA2_LFA
Observation: Tests must be moved to same directory as the C++ source code.

# Subtask a)
Read a Nondeterministic finite automaton (NFA) from a file and transform it into Deterministic finite automaton (DFA) that accepts the same language.

# Subtask b)
Minimise a given DFA.

# Format input
Programul citeste din fisierul “input.txt” urmoatoarele informatii:

- pe prima linie, numarul de stari ale automatului (numit N)
- pe a doua linie, starile automatului (numere intregi, nu neaparat consecutive)
- pe linia a treia, numarul de tranzitii (sa il notam M)
 -pe urmatoarele M linii, descrierea unei tranzitii, sub formatul:
	- x y l, unde x este starea (nodul) din care pleaca tranzitia (muchia, arcul) y este starea in care ajunge tranzitia, l este litera tranzitiei
- pe urmatoarea linie este scris un numar natural S, care inseamna starea initiala
- pe urmatoarea linie gasim nrF, care inseamna numarul de stari finale
- pe urmatoarea linie gasim nrF numere intregi (nrF<=N), reprezentand starile finale
