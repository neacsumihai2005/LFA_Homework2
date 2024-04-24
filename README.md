# TEMA2_LFA
Observation: Tests must be moved to same directory as the C++ source code.

# Subtask a)
Read a Nondeterministic finite automaton (NFA) from a file and transform it into Deterministic finite automaton (DFA) that accepts the same language.

# Subtask b)
Minimise a given DFA.

# Format input
Read from “input.txt” the following:

- on the first line, the number of states (named N)
- on the second line, the states of the automaton (integer numbers, not necessarily consecutive)
- on the third line, the number of transitions (named M)
- on the next M lines, the description of a tranasition, under the format:
	- x y l, where x is the origin (vertex, node) of the transition (edge) that leads to y, l is the letter of the transition
- on the next line, the origin state S
- on the next line, the number of final states, nrF
- on the next line, nrF integers (nrF<=N), representing the final states
