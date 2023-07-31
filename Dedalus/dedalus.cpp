#include "dedalus.hpp"

using util::pair;
using util::Random;

// Cost = constant = O(1)
// PRE: La cambra amb posició 'pos' pertany al laberint
// POST: Retorna una cambra aleatoria adjacent a la cambra amb posició 'pos' i la porta que les separa.
pair<paret,posicio> adjacent(const posicio & pos, nat fil, nat col, Random & R){

    pair<paret,posicio> par;

    bool trobat = false;
    while(not trobat){

        paret p(R(4));

        if(p == paret::NORD and pos.first > 0){
            trobat = true;
            par.first = p; 
            par.second.first  = pos.first - 1;
            par.second.second = pos.second;
        }
        
        else if(p == paret::EST and pos.second < (col - 1)){
            trobat = true;
            par.first = p; 
            par.second.first  = pos.first;
            par.second.second = pos.second + 1;
        }

        else if(p == paret::SUD and pos.first <  (fil - 1)){
            trobat = true; 
            par.first = p;
            par.second.first  = pos.first + 1;
            par.second.second = pos.second; 
        }
        
        else if(p == paret::OEST and pos.second > 0){ 
            trobat = true; 
            par.first = p;
            par.second.first  = pos.first;
            par.second.second = pos.second - 1;
        }

    }   
    
    return par;
}

// Cost = n (construit?) + n (crear partició) + n*log(n)*extra_c = O(n*log(n)*extra_c)
// n = nombre de cambres que té el laberint M
// log(n) és el cost de cercar si les cambres són al mateix grup, i en el seu cas, unir-les
// extra_c = nombre de vegades que s'obté una cambra 'c' repetida on no es pugui obir cap porta.
// Depèn de la classe Random que genera nombres aleatoris
void dedalus::construir(laberint & M) throw(error){

    Random R1(777), R2(142), R3(1723);

    // Comprovem que el laberint no estigui construit
    bool construit = false;
    nat i = 0, j = 0, files = M.num_files(), columnes = M.num_columnes();
    
    while(i < files and not construit){
        while(j < columnes and not construit){
            construit = (   M(posicio(i+1,j+1)).porta_oberta(paret::NORD) or
                            M(posicio(i+1,j+1)).porta_oberta(paret::EST)  or
                            M(posicio(i+1,j+1)).porta_oberta(paret::SUD)  or
                            M(posicio(i+1,j+1)).porta_oberta(paret::OEST) );
            j++;
        }
        i++;j=0;
    }

    if(not construit){

        particio<int> P(M.num_files()*M.num_columnes());
        for(nat i = 0; i < P.num_maxim(); i++) P.afegir(i);

        nat files = M.num_files(), columnes = M.num_columnes();

        // Algorisme de Kruskal.
        // -> Seleccionem aleatoriament una cambra. Obtenim una cambra aleatoria que sigui adjacent.
        // -> Si la porta entre aquestes dues cambres està tancada (grups diferents), l'obrim i unim 
        //    els dos grups al que pertanyen les cambres.
        // -> Repetim aquest procès fins que nomès hi hagi un únic grup.
        
        // A la partició representem cada cambra amb un natural que indica la seva posició. (fila*columnes + columna)

        while(P.size() > 1){

            nat f = R1(files);
            nat c = R2(columnes);

            pair<paret,posicio> p = adjacent(posicio(f,c),files,columnes,R3);

            nat x = f*columnes + c;                            // Posició dins de la partició de la cambra C
            nat y = p.second.first*columnes + p.second.second; // Posició dins de la partició de la cambra adjacent a C

            if(not P.mateix_grup(x,y)){
                P.unir(x,y);
                M.obre_porta(p.first,posicio(f+1,c+1));
            }

        }

    }
    else throw error(EstaExcavat);

}