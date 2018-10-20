#!/usr/bin/env python3
"""
Documentation for this module.
@file Test.py
@description Trieda, ktora testuje jednotlive funkcie implementovanej matematickej kniznice
@author Milan Tomcik
"""

import mathlib
import unittest


class Test_Math_Library (unittest.TestCase):
    """
    Trieda v ktorej su definovane jednotlive metody, ktore budu otestovane
    @class Test_Math_Library
    """
    def setUp(self):
        """
        
        """
        self.math = mathlib.Math_Library()

    def test_add(self):
        """
        Test scitania.
        """
        self.assertEqual(self.math.add(5,7), 12)
        self.assertEqual(self.math.add(5.2, 5.3), 10.5)

    def test_sub(self):
        """
        Test odcitania.
        """
        self.assertEqual(self.math.sub(5,5), 0)

    def test_mul(self):
        """
        Test nasobenia.
        """
        self.assertEqual(self.math.mul(5,-5), -25)
        self.assertEqual(self.math.mul(0.5, -0.5), -0.25)

    def test_div(self):
        """
        Test delenia.
        """
        self.assertEqual(self.math.div(10,-2), -5)
        self.assertEqual(self.math.div(10, 0), None)
        self.assertEqual(self.math.div(0.25,0.25), 1)

    def test_pow(self):
        """
        Test umocnenia.
        """
        self.assertEqual(self.math.pow(5,2), 25)
        self.assertEqual(self.math.pow(5,-2), None)
        self.assertEqual(self.math.pow(0.5,2), 0.25)
        
    def test_pow2(self):
        """
        Test umocnenia na druhu.
        """
        self.assertEqual(self.math.pow2(-2), 4)
        self.assertEqual(self.math.pow2(12), 144)
        
    def test_pow3(self):
        """
        Test umocnenia na tretiu.
        """
        self.assertEqual(self.math.pow3(-2), -8)
        
    def test_sqrt(self):
        """
        Test odmocnenia.
        """
        self.assertEqual(self.math.sqrt(-2), None)
        self.assertEqual(self.math.sqrt(25), 5)

    def test_factorial(self):
        """
        Test faktorialu - iteracna verzia.
        """
        self.assertEqual(self.math.fact(7), 5040)
        self.assertEqual(self.math.fact(0), 1)

    def test_factorial2(self):
        """
        Test faktorialu - rekurzivna verzia.
        """
        self.assertEqual(self.math.fact2(1), 1)

    def test_mod(self):
        """
        Test operacie modulo.
        """
        self.assertEqual(self.math.mod(3,2), 1)



suite = unittest.TestLoader().loadTestsFromTestCase(Test_Math_Library)
unittest.TextTestRunner(verbosity=2).run(suite)

