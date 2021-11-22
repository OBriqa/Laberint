#ifndef _PARET_HPP
#define _PARET_HPP

#include <string>

class paret {

    private:
    
        int _p;

    public:

        enum {NORD, EST, SUD, OEST, NO_DIR};

        paret(int p = NO_DIR) throw() : _p(p) {}

        paret(const std::string& s) throw() {
            _p = NO_DIR;
            if (s == "nord" || s == "NORD" || s == "N") _p = NORD;
            if (s == "est"  || s == "EST"  || s == "E") _p = EST;
            if (s == "sud"  || s == "SUD"  || s == "S") _p = SUD;
            if (s == "oest" || s == "OEST" || s == "O") _p = OEST;
        }

        paret& operator++() throw() {
            _p = (_p == NO_DIR) ? NO_DIR : _p + 1;
            return *this;
        }

        operator int() const throw() {
            return _p;
        }
  
};
#endif
