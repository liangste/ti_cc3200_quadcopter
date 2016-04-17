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
from matplotlib.ticker import MultipleLocator
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

        self.gyro_pitch_data = numpy.empty(g_sensor_plot_points)
        self.gyro_pitch_data.fill(0)
        self.gyro_roll_data = numpy.empty(g_sensor_plot_points)
        self.gyro_roll_data.fill(0)

        self.kalman_pitch_data = numpy.empty(g_sensor_plot_points)
        self.kalman_pitch_data.fill(0)
        self.kalman_roll_data = numpy.empty(g_sensor_plot_points)
        self.kalman_roll_data.fill(0)

        self.throttle_m1 = numpy.empty(g_sensor_plot_points)
        self.throttle_m1.fill(0)
        self.throttle_m2 = numpy.empty(g_sensor_plot_points)
        self.throttle_m2.fill(0)
        self.throttle_m3 = numpy.empty(g_sensor_plot_points)
        self.throttle_m3.fill(0)
        self.throttle_m4 = numpy.empty(g_sensor_plot_points)
        self.throttle_m4.fill(0)

        plt.subplot(5, 1, 1, axisbg='gray')
        plt.grid(True)
        self.ax_plot, = plt.plot(common_x_data, self.a_x_data, 'r')
        self.ay_plot, = plt.plot(common_x_data, self.a_y_data, 'g')
        self.az_plot, = plt.plot(common_x_data, self.a_z_data, 'b')
        plt.legend(['x', 'y', 'z'], bbox_to_anchor=(1.1, 0.5))
        plt.title("accelerometer readings, x, y, z")
        plt.ylabel("(g)")
        plt.ylim([-4, 4])

        plt.subplot(5, 1, 2, axisbg='gray')
        plt.grid(True)
        self.gx_plot, = plt.plot(common_x_data, self.g_x_data, 'r')
        self.gy_plot, = plt.plot(common_x_data, self.g_y_data, 'g')
        self.gz_plot, = plt.plot(common_x_data, self.g_z_data, 'b')
        plt.legend(['x', 'y', 'z'], bbox_to_anchor=(1.1, 0.5))
        plt.title("gyroscope readings, x, y, z")
        plt.ylabel('(degrees/s)')
        plt.ylim([-500, 500])

        plt.subplot(5, 1, 3, axisbg='gray')
        plt.grid(True)
        self.pitch_plot, = plt.plot(common_x_data, self.pitch_data, 'm')
        self.gyro_pitch_plot, = plt.plot(common_x_data, self.gyro_pitch_data, 'c')
        self.kalman_pitch_plot, = plt.plot(common_x_data, self.kalman_pitch_data, 'k')
        plt.legend(['a', 'g', 'k'], bbox_to_anchor=(1.1, 0.5))
        plt.title("Pitch, accelerometer, gyroscope, kalman")
        plt.ylabel('(radians)')
        plt.ylim([-math.pi, math.pi])

        plt.subplot(5, 1, 4, axisbg='gray')
        plt.grid(True)
        self.roll_plot, = plt.plot(common_x_data, self.roll_data, 'm')
        self.gyro_roll_plot, = plt.plot(common_x_data, self.gyro_roll_data, 'c')
        self.kalman_roll_plot, = plt.plot(common_x_data, self.kalman_roll_data, 'k')
        plt.legend(['a', 'g', 'k'], bbox_to_anchor=(1.1, 0.5))
        plt.title("Roll, accelerometer, gyroscope, kalman")
        plt.ylabel('(radians)')
        plt.ylim([-math.pi, math.pi])

        plt.subplot(5, 1, 5, axisbg='gray')
        plt.grid(True)
        self.throttle_m1_plot, = plt.plot(common_x_data, self.throttle_m1, 'r')
        self.throttle_m2_plot, = plt.plot(common_x_data, self.throttle_m2, 'g')
        self.throttle_m3_plot, = plt.plot(common_x_data, self.throttle_m3, 'b')
        self.throttle_m4_plot, = plt.plot(common_x_data, self.throttle_m4, 'k')
        plt.legend(['m1', 'm2', 'm3', 'm4'], bbox_to_anchor=(1.1, 0.5))
        plt.title("Motor throttle values after PID correction")
        plt.ylabel('[0, 255]')
        plt.ylim([0, 260])

    def InsertAccelReadings(self,
                            x, y, z,
                            g_x, g_y, g_z,
                             pitch, roll,
                             g_pitch, g_roll,
                             k_pitch, k_roll,
                             m1, m2, m3, m4):

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

        self.gyro_pitch_data = numpy.roll(self.gyro_pitch_data, -1)
        self.gyro_pitch_data[g_sensor_plot_points - 1] = g_pitch
        self.gyro_pitch_plot.set_ydata(self.gyro_pitch_data)

        self.kalman_pitch_data = numpy.roll(self.kalman_pitch_data, -1)
        self.kalman_pitch_data[g_sensor_plot_points - 1] = k_pitch
        self.kalman_pitch_plot.set_ydata(self.kalman_pitch_data)

        # roll data
        self.roll_data = numpy.roll(self.roll_data, -1)
        self.roll_data[g_sensor_plot_points - 1] = roll
        self.roll_plot.set_ydata(self.roll_data)

        self.gyro_roll_data = numpy.roll(self.gyro_roll_data, -1)
        self.gyro_roll_data[g_sensor_plot_points - 1] = g_roll
        self.gyro_roll_plot.set_ydata(self.gyro_roll_data)

        self.kalman_roll_data = numpy.roll(self.kalman_roll_data, -1)
        self.kalman_roll_data[g_sensor_plot_points - 1] = k_roll
        self.kalman_roll_plot.set_ydata(self.kalman_roll_data)

        # throttle data
        self.throttle_m1 = numpy.roll(self.throttle_m1, -1)
        self.throttle_m1[g_sensor_plot_points - 1] = m1
        self.throttle_m1_plot.set_ydata(self.throttle_m1)

        self.throttle_m2 = numpy.roll(self.throttle_m2, -1)
        self.throttle_m2[g_sensor_plot_points - 1] = m2
        self.throttle_m2_plot.set_ydata(self.throttle_m2)

        self.throttle_m3 = numpy.roll(self.throttle_m3, -1)
        self.throttle_m3[g_sensor_plot_points - 1] = m3
        self.throttle_m3_plot.set_ydata(self.throttle_m3)

        self.throttle_m4 = numpy.roll(self.throttle_m4, -1)
        self.throttle_m4[g_sensor_plot_points - 1] = m4
        self.throttle_m4_plot.set_ydata(self.throttle_m4)

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

        self.throttle_label = QtGui.QLabel()
        self.throttle_label.setText("Throttle value")
        self.throttle_label.setAlignment(QtCore.Qt.AlignRight | QtCore.Qt.AlignVCenter)
        self.throttle_spinbox = QtGui.QSpinBox()
        self.throttle_spinbox.setMaximum(255)
        self.throttle_spinbox.setMinimum(0)
        self.throttle_spinbox.setSingleStep(15)
        self.throttle_spinbox.setKeyboardTracking(False)
        self.throttle_spinbox.valueChanged.connect(self.ThrottleChanged)

        self.sensor_reading_plot = SensorPlot()

        # layouts
        self.main_control_layout = QtGui.QVBoxLayout()
        self.top_level_layout = QtGui.QHBoxLayout()
        self.control_btn_layout = QtGui.QHBoxLayout()
        self.throttle_layout = QtGui.QHBoxLayout()

        # layout insertions
        self.control_btn_layout.addWidget(self.start_button)
        self.control_btn_layout.addWidget(self.stop_button)

        self.throttle_layout.addWidget(self.throttle_label)
        self.throttle_layout.addWidget(self.throttle_spinbox)

        # main control widget
        self.main_control_widget = QtGui.QWidget()

        self.main_control_layout.addWidget(self.debug_console)
        self.main_control_layout.addLayout(self.control_btn_layout)
        self.main_control_layout.addLayout(self.throttle_layout)

        self.main_control_widget.setLayout(self.main_control_layout)
        self.main_control_widget.setFixedWidth(280)

        self.top_level_layout.addWidget(self.main_control_widget)
        self.top_level_layout.addWidget(self.sensor_reading_plot)

        self.setLayout(self.top_level_layout)

        # member variables
        self.started = False
        self.com_connected = False
        self.throttle_value = 0

        # set color
        pallet = QtGui.QPalette()
        pallet.setColor(self.backgroundRole(), QtCore.Qt.darkGray)
        self.setPalette(pallet)

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
            sensor_values = string.split(line, ' ')
            if len(sensor_values) >= 6:
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
                        float(sensor_values[7]),
                        float(sensor_values[8]),
                        float(sensor_values[9]),
                        float(sensor_values[10]),
                        float(sensor_values[11]),
                        float(sensor_values[12]),
                        float(sensor_values[13]),
                        float(sensor_values[14]),
                        float(sensor_values[15]))
                except Exception as e:
                    pass

    def ThrottleChanged(self, value):
        self.debug_console.append("Throttle changed to " + str(value))
        self.throttle_value = value

if __name__ == '__main__':

    app = QtGui.QApplication(sys.argv)
    t = Tester()
    t.setWindowTitle("cc3200 quadcopter tester")
    sys.exit(app.exec_())
