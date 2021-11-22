#ifndef _LABERINT_HPP
#define _LABERINT_HPP

#include <iostream>
#include <utility>
#include <esin/error>
#include <esin/util>
#include "cambra.hpp"

using util::nat;

// El primer natural de la posicio indica la fila i el segon indica la columna.
typedef std::pair<nat,nat> posicio;

/* Un objecte d'aquesta classe representa un laberint ortogonal amb un cert
nombre de files i de columnes. A cada posició de la "graella" hi ha una cambra
que podem identificar per les coordenades de la seva posició, és a dir,
l'índex de la seva fila i de la seva columna. Les cambres exteriors no poden
tenir portes envers l'exterior. A més, si una cambra C té una porta en una
certa direcció (p.e. NORD), la cambra adjacent C' en aquesta direcció
necessàriament tindrà una porta en la direcció contrària (SUD en l'exemple).*/
class laberint {
    
    private:

        #include "laberint.rep"

    public:

        // Constructora d'un laberint buit sense excavar (sense cap porta oberta)
        // de la mida indicada. Totes les cambres del laberint no tenen cap porta.
        // Es produeix un error si el número de files o de columnes del laberint és 0.
        explicit laberint(nat num_fil=5, nat num_col=5) throw(error);

        // Constructora d'un laberint a partir d'un istream (canal d'entrada) is.
        // El format de l'istream seguirà l'exposat a l'apartat 2.3. Es presuposa que
        // el laberint és correcte.
        explicit laberint(std::istream & is) throw(error);

        // Constructora per còpia, assignació i destructora.
        laberint(const laberint & l) throw(error);
        laberint & operator=(const laberint & l) throw(error);
        ~laberint() throw();

        // Retornen el número de files i columnes que té el laberint, respectivament.
        nat num_files() const throw();
        nat num_columnes() const throw();

        // Retorna la cambra situada a la posició especificada per pos.
        // Aquest mètode permet utilitzar el laberint d'aquesta manera:
        //   laberint l(3,4);
        //   posicio pos(2,2);
        //   cambra c = l(pos);
        // Es produeix un error si la posició donada no existeix al laberint.
        cambra operator()(const posicio & pos) const throw(error);

        // Obre la porta en la paret donada de la cambra que està especificada per pos.
        // També obre la porta corresponent en la cambra adjacent. Es produeix un error
        // si la posició no existeix o no es pot obrir una porta en la direcció
        // indicada perquè dóna a l'exterior.
        void obre_porta(paret p, const posicio & pos) throw(error);

        // Tanca la porta en la paret donada de la cambra que està especificada per pos.
        // També tanca la porta corresponent en la cambra adjacent. Es produeix un error
        // si la posició no existeix.
        void tanca_porta(paret p, const posicio & pos) throw(error);

        // Escriu el laberint a l'ostream (canal de sortida) os. El format per escriure
        // el laberint seguirà l'exposat a l'apartat 2.3.
        void print(std::ostream & os) const throw();

        // Gestió d'errors.
        static const int FilsColsIncorrecte = 30;
        static const int PosicioInexistent  = 31;
        static const int PortaExterior      = 32;

};

#endif
