
#ifndef _TESEUS_HPP
#define _TESEUS_HPP

#include <list>

#include <esin/error>
#include <esin/util>
#include "laberint.hpp"

namespace teseus {

  // Genera una llista de posicions que conté el camí mínim. El primer element
  // d'aquesta llista serà la posició inici, i l'última la posició final.
  // Una posició respecte a la seva anterior o posterior ha de ser consecutiva.
  // Dues posicions són consecutives si i només si la diferència de les primeres
  // components de les posicions és en valor absolut 1 i les segones components
  // són iguals, o si la diferència de les segones components és en valor
  // absolut 1 i les primeres components són iguals. Es produeix un error si no
  // hi ha cap camí que vagi des de la cambra inicial del laberint a la final,
  // o si la cambra inicial o final no són vàlides.
  void buscar(const laberint & M, 
              const posicio & inici, const posicio & final, 
              std::list<posicio> & L) throw(error);

  // Gestió d'errors.
  static const int SenseSolucio      = 60;
  static const int IniciFinalNoValid = 61;
};

#endif
