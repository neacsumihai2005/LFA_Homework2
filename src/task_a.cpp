#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>

using namespace std;

void afisare_set(std::ostream& os, set<int> const X){
    os << "{";
    for(set<int>::iterator itr = X.begin(); itr != X.end(); itr++){
        os << "q" << (*itr);
        auto aux = itr;
        aux++;
        if(aux != X.end() ){
            os << ", ";
        }
    }
    os << "}";
}

void normalizare(std::ostream& os, vector<int> &v, map<int, int> &map_normalizare){
    map_normalizare.clear();

    for(int i = 0; i < v.size(); i++){
        map_normalizare[ v[i] ]; ///funny statement; aloca spatiul
    }

    int pt = 0;
    for(map<int, int>:: iterator it = map_normalizare.begin(); it != map_normalizare.end(); it++){
        it->second = pt++;
    }

    for(int i = 0; i < v.size(); i++){
        os << v[i] << " -> q" << map_normalizare[ v[i] ] << "\n";
        v[i] = map_normalizare[ v[i] ];
    }
    os << "\n";
}
inline int cod_litera(char x){
    if(x == '0'){
        return 0;
    }
    return x - 'a' + 1;
}

void citire(std::istream&fin, std::ostream& fout, int &N, vector<int> &v, int &M, vector< vector< set<int> > > &tabel_NFA, int &sursa, int &nrF, vector<int> &stare_final, map<int, int> &map_normalizare){

    fin >> N;
    v.resize(N);
    for(int i = 0; i < N; i++){
        fin >> v[i];
    }
    normalizare(fout, v, map_normalizare);

    fin >> M;

    for(int i = 0; i < N; i++){
        vector< set<int> >temp2;
        for(int j = 0; j <= 26; j++){
            set<int> temp1;
            temp2.push_back(temp1);
        }
        tabel_NFA.push_back(temp2);
    }

    for(int i = 0; i < M; i++){
        int x, y;
        char l;
        fin >> x >> y;
        x = map_normalizare[x];
        y = map_normalizare[y];

        fin >> l;
        tabel_NFA[x][ cod_litera(l) ].insert(y);
    }

    fin >> sursa;
    sursa = map_normalizare[sursa];

    fin >> nrF;
    stare_final.resize(nrF);
    for(int i = 0; i < nrF; i++){
        fin >> stare_final[i];
        stare_final[i] = map_normalizare[ stare_final[i] ];
    }
}

void afisare_DFA(std::ostream& os, vector< set<int> >  corespondent_linie_tabel_DFA, vector< vector< set<int> > > tabel_DFA){
    for(int i = 0; i < corespondent_linie_tabel_DFA.size(); i++){
        afisare_set(os, corespondent_linie_tabel_DFA[i]);
        os << ": ";
        for(int j = 0; j <= 26; j++){
            if(!tabel_DFA[i][j].empty()){
                os << "("; afisare_set(os, tabel_DFA[i][j]); os << ", " << (char)( (j == 0) ? ('L') : ((char)('a' + j - 1))) << ") ";
            }
        }
        os << "\n";
    }
    os << "\n";

}

int main()
{
    ifstream fin ("test.in");
    ofstream fout ("test.out");
    int N;
    vector<int> v;
    int M;
    vector< vector< set<int> > >tabel_NFA; ///tabel pt NFA-ul primit
    int sursa;
    int nrF;
    vector<int> stare_final;
    map<int, int> map_normalizare;
    citire(fin,
           fout,
           N,
           v,
           M,
           tabel_NFA,
           sursa,
           nrF,
           stare_final,
           map_normalizare);


    vector< set<int> > corespondent_linie_tabel_DFA;
    set < set<int> > corespondenti_existenti;
    set<int> set_temporar_sursa;
    set_temporar_sursa.insert(sursa);
    corespondent_linie_tabel_DFA.push_back(set_temporar_sursa);
    corespondenti_existenti.insert(set_temporar_sursa);

    vector< vector< set<int> > >tabel_DFA;
    for(int i = 0; i < corespondent_linie_tabel_DFA.size(); i++){
        vector< set<int> >temp2;
        for(int j = 0; j <= 26; j++){
            set<int> temp1;
            temp2.push_back(temp1);
        }
        tabel_DFA.push_back(temp2);


        for(int j = 0; j <= 26; j++){
            for(set<int>::iterator itr = corespondent_linie_tabel_DFA[i].begin(); itr != corespondent_linie_tabel_DFA[i].end(); itr++){
                int elem = (*itr);
                tabel_DFA[i][j].insert(tabel_NFA[ elem ][j].begin(), tabel_NFA[ elem ][j].end());
            }
        }

        for(int j = 0; j <= 26; j++){
            if(tabel_DFA[i][j].empty()){
                continue;
            }

            if(corespondenti_existenti.find(tabel_DFA[i][j]) == corespondenti_existenti.end()){
                corespondent_linie_tabel_DFA.push_back(tabel_DFA[i][j]);
                corespondenti_existenti.insert(tabel_DFA[i][j]);
            }
        }
    }

    afisare_DFA(fout, corespondent_linie_tabel_DFA, tabel_DFA);


    return 0;
}
