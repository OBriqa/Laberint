#include "teseus.hpp"

using util::list;

using namespace std;

// Cost = f*c = O(f*c)
// f = nombre de files, c = nombre de columnes

// PRE: Existeix un camí entre la posició 'inici' i la posició 'final' del laberint M
// POST: S'ha emmagatzemat a la llista 'L' el camí entre la posició 'inici' i la posició 'final' del laberint M
void cami(const laberint & M, const posicio & inici, const posicio & final, int* pes, std::list<posicio> & L){

    L.push_back(final);
    posicio act = final;
    nat fil = M.num_files(), col = M.num_columnes();
    while(act != inici){
            
        nat f = act.first - 1, c = act.second - 1;
        
        if(f > 0 and ((pes[f*col + c] - 1) == pes[(f-1)*col + c]) and M(act).porta_oberta(paret::NORD)){
            act.first--;
            L.push_front(act);
        }
        else if(c < col and ((pes[f*col + c] - 1) == pes[f*col + (c+1)]) and M(act).porta_oberta(paret::EST)){
            act.second++;
            L.push_front(act);
        }
        else if(f < fil and ((pes[f*col + c] - 1) == pes[(f+1)*col + c]) and M(act).porta_oberta(paret::SUD)){    
            act.first++;
            L.push_front(act);
        }
        else if(c > 0 and ((pes[f*col + c] - 1) == pes[f*col + (c-1)]) and M(act).porta_oberta(paret::OEST)){
            act.second--;
            L.push_front(act);
        }

    }

}

// Cost = f*c (cas mig) = O(f*c)
// PRE: Tant la posició 'act' com la posició 'fi' pertanyen al laberint M
// POST: A la matriu pes s'ha indicat tots els possibles camins desde la posició 'act' fins a la posició 'fi'
bool speedy(const laberint & M, const posicio & act, const posicio & fi, int* pes){

    list<posicio> L;
    bool trobat = false;

    nat columnes = M.num_columnes();

    L.push_back(act);

    while(not L.empty() and not trobat){

        posicio act = L.front();

        nat f = act.first - 1;
        nat c = act.second - 1;

        if(act == fi){

            if(M(act).porta_oberta(paret::NORD) and pes[(f-1)*columnes + c] != -1)
                pes[f*columnes + c] = pes[(f-1)*columnes + c] + 1;

            else if (M(act).porta_oberta(paret::EST)  and pes[f*columnes + (c+1)] != -1)
                pes[f*columnes + c] = pes[f*columnes + (c+1)] + 1;

            else if (M(act).porta_oberta(paret::SUD)  and pes[(f+1)*columnes + c] != -1)
                pes[f*columnes + c] = pes[(f+1)*columnes + c] + 1;

            else if (M(act).porta_oberta(paret::OEST) and pes[f*columnes + (c-1)] != -1 )
                pes[f*columnes + c] = pes[f*columnes + (c-1)] + 1;

            trobat = true;
        }
        else{

            if(M(act).porta_oberta(paret::NORD) and pes[(f-1)*columnes + c] == -1 and not trobat){
                pes[(f-1)*columnes + c] = pes[f*columnes + c] + 1;
                L.push_back(posicio(act.first-1,act.second));
            }

            if(M(act).porta_oberta(paret::EST)  and pes[f*columnes + (c+1)] == -1 and not trobat){
                pes[f*columnes + (c+1)] = pes[f*columnes + c] + 1;
                L.push_back(posicio(act.first,act.second+1));
            }
            
            if(M(act).porta_oberta(paret::SUD)  and pes[(f+1)*columnes + c] == -1 and not trobat){
                pes[(f+1)*columnes + c] = pes[f*columnes + c] + 1;
                L.push_back(posicio(act.first+1,act.second));
            }   

            if(M(act).porta_oberta(paret::OEST) and pes[f*columnes + (c-1)] == -1 and not trobat){
                pes[f*columnes + (c-1)] = pes[f*columnes + c] + 1;
                L.push_back(posicio(act.first,act.second-1));
            }

            if(not L.empty()) L.pop_front();

        }

    }

    return trobat;

}


// Cost = f*c = O(f*c), f = nombre de files, c = nombre de columnes
void teseus::buscar(const laberint & M, const posicio & inici, const posicio & final, std::list<posicio> & L) throw(error){

    if( (inici.first  > 0  and inici.first   <= M.num_files())        and
        (inici.second > 0  and inici.second  <= M.num_columnes())     and
        (final.first  > 0  and final.first   <= M.num_files())        and
        (final.second > 0  and final.second  <= M.num_columnes())
    ){

        nat mida = (M.num_files()+1)*(M.num_columnes());
        int* pes = new int[mida];
        for(nat i = 0; i < mida; i++) pes[i] = -1;

        pes[(inici.first-1)*M.num_columnes() + (inici.second-1)] = 1;

        bool te_solucio = speedy(M,inici,final,pes);
        
        if(te_solucio){
            cami(M,inici,final,pes,L);
            delete[] pes;
        }
        else{
            delete[] pes;
            throw error(SenseSolucio);
        }
    }
    else throw error(IniciFinalNoValid);

}

// L'algorisme emprat per la resolució del laberint és l'algorisme de Dijkstra.
// Creem una matriu d'enters que representa el laberint. A l'inici, totes les posicions tenen valor -1
// Marquem amb un 0 la posició de la matriu d'on començarà el camí que hem de buscar.
// -> Per a cada posició on es pugui passar se li assigna el valor de la posició origen més 1.
//    Sempre s'assigna un valor +1, ja que fer un pas en qualsevol lloc té cost 1.
// -> Aquesta operació es repeteix per a totes les direccions on es pugui passar i es torna a realitzar el procés per
//    totes les cambres on hem passat.
// -> Per assegurar-nos que se segueix l'ordre correcte, implementem una cua amb la classe list.
//      -> Es marquen totes les possibles sortides, i s'introdueixen al final de la llista.
//      -> A continuació s'elimina de la llista el primer element i s'agafa el que ara és el primer.
// -> Així agafarem sempre l'element inicial, i farem un pas per a tots els possibles camins.
// Acabem quan s'arriba a la cambra final o quan s'hagi acabat de recórrer tot el laberint.