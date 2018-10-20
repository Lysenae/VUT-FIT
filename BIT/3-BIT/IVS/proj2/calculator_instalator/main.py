#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys

from PyQt5.QtWidgets import QApplication
from calcwindow import CalcWindow

if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = CalcWindow()
    window.show()
    sys.exit(app.exec_())
