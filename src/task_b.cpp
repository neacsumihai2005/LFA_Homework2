#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>

using namespace std;

void normalizare(std::ostream& os, vector<int> &v, map<int, int> &map_normalizare){
    map_normalizare.clear();

    for(int i = 0; i < v.size(); i++){
        map_normalizare[ v[i] ]; ///funny statement; alocates space
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

void citire(std::istream&fin, std::ostream& fout, int &N, vector<int> &v, int &M, vector< vector< int > > &tabel_DFA, int &sursa, int &nrF, vector<int> &stare_final, map<int, int> &map_normalizare){

    fin >> N;
    v.resize(N);
    for(int i = 0; i < N; i++){
        fin >> v[i];
    }
    normalizare(fout, v, map_normalizare);

    fin >> M;

    for(int i = 0; i < N; i++){
        vector< int >temp;
        for(int j = 0; j <= 26; j++){
            temp.push_back(-1);
        }
        tabel_DFA.push_back(temp);
    }

    for(int i = 0; i < M; i++){
        int x, y;
        char l;
        fin >> x >> y;
        x = map_normalizare[x];
        y = map_normalizare[y];

        fin >> l;
        tabel_DFA[x][ cod_litera(l) ] = y;
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
bool gasesc(int x, vector<int>v){
    for(int i = 0; i < v.size(); i++){
        if(v[i] == x){
            return 1;
        }
    }
    return 0;
}

bool equivalent(int value1, int value2, vector < vector<int> > lista, vector< vector<int> >tabel_DFA){
    for(int id_alpha = 0; id_alpha <= 26; id_alpha++){
        int reprezentant1 = tabel_DFA[value1][id_alpha];
        int reprezentant2 = tabel_DFA[value2][id_alpha];

        ///either both representatives are valid, or none are
        if(reprezentant1 == -1 && reprezentant2 == -1){
            ///valid;
        }
        else if(reprezentant1 == -1){
            return false;
        }
        else if(reprezentant2 == -1){
            return false;
        }
        else {
            int id_reprezentant1;
            int id_reprezentant2;
            ///verify if reprezentant1 si reprezentant2 are in the same vector from 'lista'
            for(int i = 0; i < lista.size(); i++){
                for(int j = 0; j < lista[i].size(); j++){
                    if(reprezentant1 == lista[i][j]){
                        id_reprezentant1 = i;
                    }
                    if(reprezentant2 == lista[i][j]){
                        id_reprezentant2 = i;
                    }
                }
            }

            if(id_reprezentant1 != id_reprezentant2){
                return false;
            }
        }
    }

    return true;

}

void show_new(std::ostream& os, int value, vector< vector<int> > new_equivalence){
    bool flag_found = 0;
    int container_show = 0;
    for(int id_container = 0; !flag_found && id_container < new_equivalence.size(); id_container++){
        for(int itr_container = 0; !flag_found && itr_container < new_equivalence[id_container].size(); itr_container++){
            if(new_equivalence[id_container][itr_container] == value){
                flag_found = 1;
                container_show = id_container;
            }
        }
    }

    os << "{";
    for(int itr_container = 0; itr_container < new_equivalence[container_show].size(); itr_container++){
        os << "q";
        os << new_equivalence[container_show][itr_container];

        if(itr_container != new_equivalence[container_show].size() - 1){
            os << ", ";
        }
    }
    os << "}";
}

int main()
{
    ifstream fin ("test.in");
    ofstream fout ("test.out");
    int N;
    vector<int> v;
    int M;
    vector< vector< int > >tabel_DFA;
    int sursa;
    int nrF;
    vector<int> stare_final;
    map<int, int> map_normalizare;
    citire(fin,
           fout,
           N,
           v,
           M,
           tabel_DFA,
           sursa,
           nrF,
           stare_final,
           map_normalizare);

    vector < vector<int> > last_equivalence;
    vector < vector<int> > new_equivalence;
    ///equivalence number k is built upon equivalence number k - 1
    ///so we use this to save memory
    vector<int> noduri_nefinale;
    for(int i = 0; i < N; i++){
        if( !gasesc(v[i], stare_final) ){
            noduri_nefinale.push_back(v[i]);
        }
    }
    new_equivalence.push_back(noduri_nefinale);
    new_equivalence.push_back(stare_final);

    while(last_equivalence != new_equivalence){

        ///compute now_equivalence based on last_equivalence
        last_equivalence = new_equivalence; ///not a shallow copy; I tested for it
        new_equivalence.clear();

        for(int nr_vec = 0; nr_vec < last_equivalence.size(); nr_vec++){
            vector<int> current_container;
            current_container = last_equivalence[nr_vec];

            int start_pos_valids = new_equivalence.size();

            vector<int>aux;
            aux.push_back(last_equivalence[nr_vec][0]);
            new_equivalence.push_back(aux);

            for(int id_elem = 1; id_elem < current_container.size(); id_elem++){
                int elem_to_add = current_container[id_elem];
                int flag_stop = 0;

                for(int nr_new_container = start_pos_valids; !flag_stop && nr_new_container < new_equivalence.size(); nr_new_container++){
                    if( equivalent(elem_to_add, new_equivalence[nr_new_container][0], last_equivalence, tabel_DFA) ){
                        new_equivalence[nr_new_container].push_back(elem_to_add);
                        flag_stop = 1;
                    }
                }

                if( ! flag_stop){ ///I need to create a new vector for this elem_to_add
                    vector<int> temp;
                    temp.push_back(elem_to_add);

                    new_equivalence.push_back(temp);
                }
            }
        }


    }

    for(int id_container = 0; id_container < new_equivalence.size(); id_container++){
        fout << "{";
        for(int itr_container = 0; itr_container < new_equivalence[id_container].size(); itr_container++){
            int crr_elem = new_equivalence[id_container][itr_container];
            fout << "q" << crr_elem;

            if(itr_container != new_equivalence[id_container].size() - 1){
                fout << ", ";
            }
        }
        fout << "}: ";

        for(int itr_container = 0; itr_container < new_equivalence[id_container].size(); itr_container++){
            for(int j = 0; j <= 26; j++){
                int crr_elem = new_equivalence[id_container][itr_container];

                if(tabel_DFA[crr_elem][j] != -1){
                    bool flag_good = 1;
                    for(int itr_duplicate = 0; itr_duplicate < itr_container; itr_duplicate++){
                        if(tabel_DFA[ new_equivalence[id_container][itr_duplicate] ][j] == tabel_DFA[crr_elem][j]){
                            flag_good = 0;
                        }
                    }

                    if(flag_good){
                        fout << "(";
                        show_new(fout, tabel_DFA[crr_elem][j], new_equivalence);
                        fout << ", ";
                        fout << ((j == 0) ? ('L') : (char)('a' + j - 1));
                        fout<< ") ";
                    }
                }
            }
        }
        fout << "\n";

    }

    return 0;
}
