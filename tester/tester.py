#!/usr/bin/python
# -*- coding: utf-8 -*-
# run with '/cygdrive/c/Python27/python.exe tester.py'
# if missing packages, install with pip.exe, unless it's PyQt4, which you must
# install by downloading one of its install binaries

import sys
import serial
import string
import numpy
from PyQt4 import QtGui, QtCore
from matplotlib.backends.backend_qt4agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure
import matplotlib.pyplot as plt

class Tester(QtGui.QWidget):
    """ Main Tester Program for our quadcopter """

    def __init__(self):
        super(Tester, self).__init__()

        start_button = QtGui.QPushButton("Start", self)
        start_button.clicked.connect(self.Start)
        start_button.move(20, 450)

        start_button = QtGui.QPushButton("Stop", self)
        start_button.clicked.connect(self.Stop)
        start_button.move(100, 450)

        self.qtb = QtGui.QTextBrowser(self)
        self.qtb.resize(250, 440)
        self.qtb.show()

        self.resize(800, 480)
        self.show()

        fig = plt.figure()

        self.started = False
        self.com_connected = False

    def Start(self):
        if not self.started:
            self.qtb.append("Starting ...")
        try:
            self.serial_port = serial.Serial(port="COM4", baudrate=115200, timeout=1)
            self.qtb.append("Connected to Serial Port")
            self.com_connected = True
            self.serial_port.flushInput()
        except Exception as e:
            print e
        if not self.started:
            self.serial_timer = QtCore.QTimer()
            self.serial_timer.timeout.connect(self.SerialRead)
            self.serial_timer.start(10)

            self.started = True

    def Stop(self):
        if self.started:
            self.qtb.append("Stopping ...")
            self.started = False
            self.serial_timer.stop()

            self.serial_port.flush()
            self.serial_port.close()
            self.com_connected = False

    def SerialRead(self):
        if self.com_connected:
            self.serial_port.write("1\n")
            line = self.serial_port.readline().rstrip('\n').rstrip('\r')
            sensor_values = string.split(line, ' ')
            if len(sensor_values) == 6:
                #self.qtb.append(line)
                pass

if __name__ == '__main__':
    app = QtGui.QApplication(sys.argv)
    t = Tester()
    sys.exit(app.exec_())
