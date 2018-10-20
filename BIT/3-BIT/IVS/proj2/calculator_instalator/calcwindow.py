# -*- coding: utf-8 -*-

# IVS 2016 projekt 2
# Kalkulacka
# @author xklima22

from PyQt5.QtWidgets import QMainWindow, QWidget
from ui_calcwindow import Ui_CalcWindow
from mathlib import Math_Library

class Func:
    """
    Trieda poskytujuca vyctovy typ pre operacie kalkulacky.
    @class Func
    """
    NONE      = 0
    EQUAL     = 1
    PLUS      = 2
    MINUS     = 3
    MULTIPLY  = 4
    DIVIDE    = 5
    MOD       = 6
    POWA      = 7
    POW2      = 8
    SQRT      = 9
    FACTORIAL = 10

    @staticmethod
    def to_s(i):
        """
        Staticka metoda poskytujuca preklad vyctoveho typu na typ string.
        @param i Index vyctu
        @return Vracia retazcovu reprezentaciu cisla vyctu ak je index v
        povolenom rozsahu, inak nerobi nic
        """

        n = ['NONE', 'EQUAL', 'PLUS', 'MINUS', 'MULTIPLY', 'DIVIDE', 'MOD',
        'POWA', 'POW2', 'SQRT', 'FACTORIAL']
        if i >= 0 and i < len(n):
            return n[i]

class CalcWindow(QMainWindow):
    """
    Trieda poskytujuca prepojenie medzi GUI kalkulacky a Math_Library. Vyzaduje
    existujuci subor ui_calcwindow.py.
    @class CalcWindow
    """

    def __init__(self):
        """
        Konstrukor.
        """

        super(CalcWindow, self).__init__()
        self.ui = Ui_CalcWindow()
        self.ui.setupUi(self)
        self.entry = self.ui.line_edit
        self.math = Math_Library()
        self.connect_signals()
        self.reset()
        Func.to_s(1)

    def connect_signals(self):
        """
        Prepojenie signalov stlacenia tlacitok so slotom btn_clicked.
        """

        attrs = dir(self.ui)
        for attr in attrs:
            if 'btn_' in attr:
                a = getattr(self.ui, attr)
                a.clicked.connect(self.btn_clicked)

    def reset(self):
        """
        Nastavenie kalkulacky do defaultneho stavu, vyresetovanie errorov,
        priznakov, atd.
        """

        self.result    = 0
        self.entry1    = None
        self.entry2    = None
        self.dot_used  = False
        self.has_bin   = False
        self.e2_input  = False
        self.error     = False
        self.has_rslt  = False
        self.def_bin   = False
        self.func      = Func.NONE
        self.bin_op    = Func.NONE
        self.entry.setText(str(self.result))

    def btn_clicked(self):
        """
        Spracovanie signalu stlacenia tlacitka. Rozlisuje typ tlacitok - 
        funkcia, cislo, operator a vola prislune operacie.
        """

        try:
            sender = self.sender()
            if not self.error:
                if self.is_num(sender.objectName()):
                    self.handle_num(sender)
                elif self.is_dot(sender.objectName()):
                    self.handle_dot(sender)
                elif self.is_neg(sender.objectName()):
                    self.handle_neg()
                elif self.is_function(sender.objectName()):
                    self.handle_function(sender)
        except:
            self.error = True
            self.set_entry('ERROR')

        if sender.text() == 'C':
            self.reset()
        elif sender.text() == 'CE' and not self.error:
            self.entry.setText('0')
            self.dot_used = False



    def set_entry(self, entry, delete_rzeroes = False):
        """
        Nastavenie hodnoty na "displeji".
        @param entry Text, ktory sa ma vypisat
        @param delete_rzeroes Priznak urcujuci, ci sa maju odstranit nuly vpravo.
        """

        if not isinstance(entry, str):
            entry = str(entry)

        entry = entry.lstrip('0')
        if not entry:
            entry = '0'
        elif entry[0] == '.':
            entry = '0' + entry

        if delete_rzeroes and '.' in entry:
            entry = entry.rstrip('0').rstrip('.')

        self.entry.setText(entry)
        self.entry.setCursorPosition(0)

    def is_num(self, obj_name):
        """
        Test ci je tlacitko cislene.
        @param obj_name ID objektu
        @return Vracia True ak je tlacitko ciselne.
        """

        if 'num' in obj_name:
            return True
        return False

    def is_dot(self, obj_name):
        """
        Test ci je tlacitko bodka.
        @param obj_name ID objektu
        @return Vracia True ak je tlacitko bodka.
        """

        if 'dot' in obj_name:
            return True
        return False

    def is_neg(self, obj_name):
        """
        Test ci je tlacitko negacia.
        @param obj_name ID objektu
        @return Vracia True ak je tlacitko negacia.
        """

        if 'neg' in obj_name:
            return True
        return False

    def is_function(self, obj_name):
        """
        Test ci je tlacitko funkcia.
        @param obj_name ID objektu
        @return Vracia True ak je tlacitko funkcia.
        """

        n = ['sqrt', 'powa', 'pow2', 'mod', 'factorial', 'equal',
            'plus', 'minus', 'multiply', 'divide']
        for i in n:
            if i in obj_name:
                return True
        return False

    def handle_num(self, sender):
        """
        Spracovanie stlacenie tlacitka s cislom.
        @param sender Objekt, ktory signal vyvolal
        """

        self.def_bin = False
        if self.has_bin and not self.e2_input:
            self.set_entry('')
            self.e2_input = True
        elif self.has_rslt:
            self.set_entry('')
            self.has_rslt = False
        self.set_entry(self.entry.text() + sender.text())

    def handle_dot(self, sender):
        """
        Spracovanie stlacenie tlacitka desatinna ciarka.
        @param sender Objekt, ktory signal vyvolal
        """

        self.def_bin = False
        if not self.dot_used:
            self.set_entry(self.entry.text() + sender.text())
            self.dot_used = True

    def handle_function(self, sender):
        """
        Spracovanie stlacenie tlacitka s funkciou.
        @param sender Objekt, ktory signal vyvolal
        """

        self.dot_used = False
        if self.has_bin and self.e2_input:
            self.bin_op = self.func
            self.entry2 = self.entry.text()
            self.set_entry(self.calculate_binary(), True)
            if self.error:
                return
        self.func = self.recognize_function(sender.objectName())
        self.calculate()

    def handle_neg(self):
        """
        Spracovanie stlacenie tlacitka negacia.
        """

        if not self.def_bin:
            if self.entry.text()[0] == '-':
                self.set_entry(self.entry.text()[1:])
            else:
                self.set_entry('-' + self.entry.text())

    def recognize_function(self, obj_name):
        """
        Rozpoznanie funkcie.
        @param obj_name Id objektu, ktry vyvolal funkciu.
        @return Vracia ID funkcie.
        """

        if 'equal' in obj_name:
            return Func.EQUAL
        elif 'plus' in obj_name:
            return Func.PLUS
        elif 'minus' in obj_name:
            return Func.MINUS
        elif 'multiply' in obj_name:
            return Func.MULTIPLY
        elif 'divide' in obj_name:
            return Func.DIVIDE
        elif 'mod' in obj_name:
            return Func.MOD
        elif 'powa' in obj_name:
            return Func.POWA
        elif 'pow2' in obj_name:
            return Func.POW2
        elif 'sqrt' in obj_name:
            return Func.SQRT
        elif 'factorial' in obj_name:
            return Func.FACTORIAL
        else:
            return Func.NONE

    def calculate(self):
        """
        Vypocet pomocou urcitejfunkcie. Rozlisuje unarne a binarne funkcie a
        v zavislosti od toho vola prislusne metody.
        """

        if self.is_binary():
            if self.has_bin and self.e2_input:
                self.entry1 = self.calculate_binary()
                if self.entry1 == None:
                    self.set_entry('ERROR')
                    return
            if not self.has_bin:
                self.entry1 = self.entry.text()
            self.has_bin = True
            self.def_bin = True
            self.set_entry(self.binary_operator())
        elif self.is_unary():
            if not self.has_bin or (self.has_bin and self.e2_input):
                self.set_entry(self.calculate_unary(), True)

    def is_unary(self):
        """
        Test, ci je volana funkcia unarna.
        @return Vracia True ak je funkcia unarna.
        """

        if self.func in [Func.POW2, Func.SQRT, Func.FACTORIAL,
        Func.EQUAL]:
            return True
        return False

    def is_binary(self):
        """
        Test, ci je volana funkcia binarna.
        @return Vracia True ak je funkcia binarna.
        """

        if self.func in [Func.POWA, Func.PLUS, Func.MINUS, Func.MULTIPLY,
        Func.DIVIDE, Func.MOD]:
            return True
        return False

    def calculate_unary(self):
        """
        Vypocet unarnej funkcie.
        @return Vracia Float alebo None v pripade chyby.
        """

        val           = float(self.entry.text())
        self.has_rslt = True
        rslt          = None
        print("{}: a = {}".format(Func.to_s(self.func), val))

        if self.func == Func.POW2:
            rslt = self.math.pow2(val)
        elif self.func == Func.SQRT:
            rslt = self.math.sqrt(val)
        elif self.func == Func.FACTORIAL:
            if int(val) > 1000:
                rslt = None
            else:
                rslt = self.math.fact(int(val))
        elif self.func == Func.EQUAL:
            rslt = val

        if self.result == None:
            self.error = True
            rslt       = 'ERROR'
        else:
            rslt       = round(rslt, 5)

        print('Result: {}'.format(rslt))

        return rslt

    def binary_operator(self):
        """
        Prevod operatoru binarnej funkcie na typ string.
        @return Vracia operator prislusnej binarnej operacie.
        """
        if self.func == Func.POWA:
            return '^'
        elif self.func == Func.PLUS:
            return '+'
        elif self.func == Func.MINUS:
            return '-'
        elif self.func == Func.MOD:
            return '%'
        elif self.func == Func.MULTIPLY:
            return '*'
        elif self.func == Func.DIVIDE:
            return '/'

    def calculate_binary(self):
        """
        Vypocet binarnej funkcie.
        @return Vracia Float alebo None v pripade chyby.
        """
        rslt          = None
        a             = float(self.entry1)
        b             = float(self.entry2)
        self.entry1   = None
        self.entry2   = None
        self.e2_input = False
        self.has_bin  = False

        print("{}: a = {}, b = {}".format(Func.to_s(self.bin_op), a, b))

        if self.bin_op == Func.POWA:
            rslt = self.math.pow(a, b)
        elif self.bin_op == Func.PLUS:
            rslt = self.math.add(a, b)
        elif self.bin_op == Func.MINUS:
            rslt = self.math.sub(a, b)
        elif self.bin_op == Func.MULTIPLY:
            rslt = self.math.mul(a, b)
        elif self.bin_op == Func.DIVIDE:
            rslt = self.math.div(a, b)
        elif self.bin_op == Func.MOD:
            rslt = self.math.mod(int(a), int(b))

        print('Result: {}'.format(rslt))
        if rslt == None:
            self.error = True
            rslt       = 'ERROR'
        else:
            rslt       = round(rslt, 5)

        return rslt
