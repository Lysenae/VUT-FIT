# IVS 2016 projekt 2
# Matematicka kniznica
# @author xmalik14

"""
Documentation for this module.
@file mathlib.py
@description Trieda, ktora poskytuje matematicke operacie
@author xmalik14
"""

class Math_Library (object):
    """
    Matematicka kniznica - obsahuje elementarne operacie sucet, rozdiel, nasobenie, delenie,
    umocnovanie, modulo atd.
    @class Math_Library
    """
    def __init__(self):
        """
        Konstruktor.
        """
        self.lookup = {}
    
    def add(self, a, b):
        """
        Sucet dvoch cisel.
        @param a Scitanec 1
        @param b Scitanec 2
        @return Vracia sucet cisel.
        """
        return a + b

    def sub(self, a, b):
        """
        Rozdiel dvoch cisel.
        @param a Mensenec
        @param b Mensitel
        @return Vracia rozdiel cisel.
        """
        return a - b

    def mul(self, a, b):
        """
        Sucin dvoch cisel.
        @param a Cinitel 1
        @param b Cinitel 2
        @return Vracia sucin cisel.
        """
        return a * b

    def div(self, a, b):
        """
        Podiel dvoch cisel.
        @param a Delenec
        @param b Delitel
        @return Vracia podiel cisel.
        """
        if b == 0:
            return None
        else:
            return a / b

    # redefinovane - len pre prirodzene exponenty (b)
    def pow(self, a, b):
        """
        Umocnenie dvoch cisel.
        @param a Zaklad
        @param b Exponent
        @return Vracia b-tu mocninu cisla a.
        """
        if b < 0:
            return None
        else:
            return a ** int(b)
    
    def pow2(self, a):
        """
        Umocnenie na druhu.
        @param a Zaklad
        @return Vracia druhu mocninu cisla a.
        """
        return a ** 2
    
    def pow3(self, a):
        """
        Umocnenie na druhu.
        @param a Zaklad
        @return Vracia tretiu mocninu cisla a.
        """
        return a ** 3
    
    def sqrt(self, a):
        """
        Druha odmocnina cisla.
        @param a Cislo na odmocnenie
        @return Vracia druhu odmocninu cisla a.
        """
        if a < 0:
            return None
        return a ** 0.5

    def mod(self, a, b):
        """
        Celociselny zvysok po deleni cisel.
        @param a Delenec
        @param b Delitel
        @return Vracia celociselny zvysok po deleni cisel - modulo.
        """
        if b == 0:
            return None
        else:
            return a % b

    def neg(self, a):
        """
        Zmeni znamienko cisla.
        @param a Cislo
        @return Vracia cislo s opacnym znamienkom.
        """
        return -a

    def fact(self, a):
        """
        Faktorial cisla - iteracna verzia.
        @param a Cislo
        @return Vracia faktorial cisla.
        """
        f = 1;
        for i in range(1,a+1):
            f *= i;
        return f;

    def fact2(self, a):
        """
        Faktorial cisla - rekurzivna verzia.
        @param a Cislo
        @see https://en.wikipedia.org/wiki/Memoization
        @return Vracia faktorial cisla.
        """
        if (a <= 1):
            return 1

        if(a < len(self.lookup)+1):
            return self.lookup[a]

        self.lookup[a] = a * self.fact2(a - 1)
        return self.lookup[a]

