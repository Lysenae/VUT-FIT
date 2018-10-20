# Autor: Daniel Klimaj; xklima22@stud.fit.vutbr.cz
# Popis: Trieda pre vypisaovanie chybovych a debugovacich hlasok

import sys

class LogLevel(object):
    Debug     = 0
    Important = 1

class Log(object):

    LOGLEVEL = LogLevel.Important

    @staticmethod
    def error(msg):
        '''
        Logovanie chyby na stderr.
        @param msg Sprava
        '''

        sys.stderr.write("CHYBA: {}\n".format(msg))

    @staticmethod
    def info(msg):
        '''
        Logovanie informacneho vypisu na stdout.
        @param msg Sprava
        '''

        print("INFO:  {}".format(msg))

    @staticmethod
    def debug(msg):
        '''
        Logovanie debugovacieho vypisu na stdout. Musi byt nastaveny LOGLEVEL na Debug.
        @param msg Sprava
        '''
        if Log.LOGLEVEL == LogLevel.Debug:
            print("DEBUG: {}".format(msg))

# log.py
