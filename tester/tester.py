#!/usr/bin/python
# -*- coding: utf-8 -*-
# run with '/cygdrive/c/Python27/python.exe tester.py'
# if missing packages, install with pip.exe, unless it's PyQt4, which you must
# install by downloading one of its install binaries

# cygwin install - Python/python-setuptools
# install pyqt4 through cygwin installer
# install other missing packages through pip

import sys
import serial
import string
import numpy
import math
from PyQt4 import QtGui, QtCore
from matplotlib.backends.backend_qt4agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure
import matplotlib.pyplot as plt

g_sensor_plot_points = 500

class SensorPlot(FigureCanvas):
    """ Plot class that handles sensor readings from the quadcopter and display
        them """

    def __init__(self):
        self.figure = plt.figure()
        super(SensorPlot, self).__init__(self.figure)

        common_x_data = numpy.linspace(0, 1, g_sensor_plot_points)

        self.a_x_data = numpy.empty(g_sensor_plot_points)
        self.a_x_data.fill(0)
        self.a_y_data = numpy.empty(g_sensor_plot_points)
        self.a_y_data.fill(0)
        self.a_z_data = numpy.empty(g_sensor_plot_points)
        self.a_z_data.fill(0)

        self.g_x_data = numpy.empty(g_sensor_plot_points)
        self.g_x_data.fill(0)
        self.g_y_data = numpy.empty(g_sensor_plot_points)
        self.g_y_data.fill(0)
        self.g_z_data = numpy.empty(g_sensor_plot_points)
        self.g_z_data.fill(0)

        self.pitch_data = numpy.empty(g_sensor_plot_points)
        self.pitch_data.fill(0)
        self.roll_data = numpy.empty(g_sensor_plot_points)
        self.roll_data.fill(0)

        plt.subplot(4, 1, 1)
        self.ax_plot, = plt.plot(common_x_data, self.a_x_data, 'r')
        self.ay_plot, = plt.plot(common_x_data, self.a_y_data, 'g')
        self.az_plot, = plt.plot(common_x_data, self.a_z_data, 'b')
        plt.legend(['x', 'y', 'z'])
        plt.title("accelerometer readings")
        plt.ylabel("(g)")
        plt.ylim([-4, 4])

        plt.subplot(4, 1, 2)
        self.gx_plot, = plt.plot(common_x_data, self.g_x_data, 'r')
        self.gy_plot, = plt.plot(common_x_data, self.g_y_data, 'g')
        self.gz_plot, = plt.plot(common_x_data, self.g_z_data, 'b')
        plt.title("gyroscope readings")
        plt.ylabel('(degrees/s)')
        plt.ylim([-500, 500])

        plt.subplot(4, 1, 3)
        self.pitch_plot, = plt.plot(common_x_data, self.pitch_data)
        plt.title("Pitch")
        plt.ylim([-math.pi, math.pi])

        plt.subplot(4, 1, 4)
        self.roll_plot, = plt.plot(common_x_data, self.roll_data)
        plt.title("Roll")
        plt.ylim([-math.pi, math.pi])

    def InsertAccelReadings(self, x, y, z, g_x, g_y, g_z, pitch, roll):

        # update accelerometer data
        self.a_x_data = numpy.roll(self.a_x_data, -1)
        self.a_x_data[g_sensor_plot_points - 1] = x
        self.ax_plot.set_ydata(self.a_x_data)

        self.a_y_data = numpy.roll(self.a_y_data, -1)
        self.a_y_data[g_sensor_plot_points - 1] = y
        self.ay_plot.set_ydata(self.a_y_data)

        self.a_z_data = numpy.roll(self.a_z_data, -1)
        self.a_z_data[g_sensor_plot_points - 1] = z
        self.az_plot.set_ydata(self.a_z_data)

        # update gyro data
        self.g_x_data = numpy.roll(self.g_x_data, -1)
        self.g_x_data[g_sensor_plot_points - 1] = g_x
        self.gx_plot.set_ydata(self.g_x_data)

        self.g_y_data = numpy.roll(self.g_y_data, -1)
        self.g_y_data[g_sensor_plot_points - 1] = g_y
        self.gy_plot.set_ydata(self.g_y_data)

        self.g_z_data = numpy.roll(self.g_z_data, -1)
        self.g_z_data[g_sensor_plot_points - 1] = g_z
        self.gz_plot.set_ydata(self.g_z_data)

        # pitch data
        self.pitch_data = numpy.roll(self.pitch_data, -1)
        self.pitch_data[g_sensor_plot_points - 1] = pitch
        self.pitch_plot.set_ydata(self.pitch_data)

        # roll data
        self.roll_data = numpy.roll(self.roll_data, -1)
        self.roll_data[g_sensor_plot_points - 1] = roll
        self.roll_plot.set_ydata(self.roll_data)

        self.figure.canvas.draw()


class Tester(QtGui.QWidget):
    """ Main Tester Program for our quadcopter """

    def __init__(self):
        super(Tester, self).__init__()

        self.start_button = QtGui.QPushButton("Start")
        self.start_button.clicked.connect(self.StartTest)

        self.stop_button = QtGui.QPushButton("Stop")
        self.stop_button.clicked.connect(self.StopTest)

        self.debug_console = QtGui.QTextBrowser()

        self.throttle_slider = QtGui.QSlider(QtCore.Qt.Vertical)
        self.throttle_slider.valueChanged[int].connect(self.ThrottleChanged)
        self.throttle_slider.setTickInterval(10)
        self.throttle_slider.setTickPosition(QtGui.QSlider.TicksRight)

        self.sensor_reading_plot = SensorPlot()

        self.main_control_layout = QtGui.QVBoxLayout()
        self.top_level_layout = QtGui.QHBoxLayout()
        self.control_btn_layout = QtGui.QHBoxLayout()

        self.control_btn_layout.addWidget(self.start_button)
        self.control_btn_layout.addWidget(self.stop_button)

        self.main_control_widget = QtGui.QWidget()
        self.main_control_layout.addWidget(self.debug_console)
        self.main_control_layout.addLayout(self.control_btn_layout)
        self.main_control_widget.setLayout(self.main_control_layout)
        self.main_control_widget.setFixedWidth(360)

        self.top_level_layout.addWidget(self.main_control_widget)
        self.top_level_layout.addWidget(self.sensor_reading_plot)
        self.top_level_layout.addWidget(self.throttle_slider)

        self.setLayout(self.top_level_layout)

        self.started = False
        self.com_connected = False
        self.throttle_value = 0

        self.resize(1440, 960)
        self.show()

    def StartTest(self):

        if not self.started:
            self.debug_console.append("Starting ...")
        try:
            self.serial_port = serial.Serial(port="COM4", baudrate=115200, timeout=1)
            self.debug_console.append("Connected to Serial Port")
            self.com_connected = True
            self.serial_port.flushInput()
        except Exception as e:
            print e
        if not self.started:
            self.serial_timer = QtCore.QTimer()
            self.serial_timer.timeout.connect(self.SerialRead)
            self.serial_timer.start(10)

            self.started = True

        self.serial_port.write("0\r")

    def StopTest(self):

        if self.started:
            self.debug_console.append("Stopping ...")
            self.started = False
            self.serial_timer.stop()

            if self.com_connected:
                self.serial_port.write("0\r")
                self.serial_port.close()
                self.com_connected = False

    def SerialRead(self):

        if self.com_connected:
            line = self.serial_port.readline().rstrip("\n\r").lstrip("\n\r")
            #self.debug_console.append(line)
            sensor_values = string.split(line, ' ')
            if len(sensor_values) >= 6:
                #self.debug_console.append("z = " + sensor_values[2])
                self.serial_port.write(str(self.throttle_value) + "\r")
                try:
                    self.sensor_reading_plot.InsertAccelReadings(
                        float(sensor_values[0]),
                        float(sensor_values[1]),
                        float(sensor_values[2]),
                        float(sensor_values[3]),
                        float(sensor_values[4]),
                        float(sensor_values[5]),
                        float(sensor_values[6]),
                        float(sensor_values[7]))
                except Exception as e:
                    pass

    def ThrottleChanged(self, value):

        self.throttle_value = value

if __name__ == '__main__':

    app = QtGui.QApplication(sys.argv)
    t = Tester()
    sys.exit(app.exec_())
