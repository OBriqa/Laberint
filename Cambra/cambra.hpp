#ifndef _CAMBRA_HPP
#define _CAMBRA_HPP

#include <esin/error>
#include <esin/util>
#include "paret.hpp"

class cambra {
public:

    private:
    
        #include "cambra.rep"

    public:

        // Construeix una cambra amb les portes als llocs indicats.
        explicit cambra(bool n=false, bool s=false, bool e=false, bool o=false) throw(error);

        // Constructora per còpia, assignació i destructora.
        cambra(const cambra & c) throw(error);
        cambra & operator=(const cambra & c) throw(error);
        ~cambra() throw();

        // Retorna si l'habitació té una porta oberta a la paret indicada.
        bool porta_oberta(paret p) const throw();

        // Obre una nova porta a la paret indicada. Si la porta ja està oberta no
        // fa res. Es produeix un error si la paret és NO_DIR.
        void obre_porta(paret p) throw(error);

        // Tanca la porta a la paret indicada. Si la porta ja estava tancada no
        // fa res. Es produeix un error si la paret és NO_DIR.
        void tanca_porta(paret p) throw(error);

        // Igualtat i desigualtat entre cambres. Dues cambres es consideren iguals
        // si tenen les mateixes portes obertes a les mateixes parets.
        bool operator==(const cambra & c) const throw();
        bool operator!=(const cambra & c) const throw();

        // Operador "menor que" entre cambres. Una cambra és més petita que una
        // altra si té més portes tancades que l'altra. Tenint en compte que una
        // porta tancada és més petita que una porta oberta, en cas que tinguin el
        // mateix número de portes tancades, per decidir qui és més petit es miraran
        // les portes en aquest ordre NORD, EST, SUD i OEST. Per exemple:
        //   cambra c1, c2(true,true), c3(true, false, true);
        //   cout << (c1 < c2); // escriu 'true'
        //   cout << (c2 < c3); // escriu 'true'
        bool operator<(const cambra & c) const throw();

        // Gestió d'errors.
        static const int  ParetInexistent = 20;

};

#endif
