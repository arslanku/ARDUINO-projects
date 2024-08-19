import sys
import matplotlib.pyplot as plt
from PyQt5.QtWidgets import QApplication, QMainWindow, QLabel, QPushButton, QComboBox, QVBoxLayout, QSplitter, QWidget
from PyQt5.QtGui import QPixmap, QFont
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure

class TemperatureChart(QWidget):
    def __init__(self, temp):
        super().__init__()
        self.temp = temp
        self.initUI()

    def initUI(self):
        self.figure = Figure()
        self.canvas = FigureCanvas(self.figure)
        layout = QVBoxLayout()
        layout.addWidget(self.canvas)
        self.setLayout(layout)

        self.ax = self.figure.add_subplot(111)
        self.ax.set_xlabel('Time')
        self.ax.set_ylabel('Temperature (°C)')
        self.line, = self.ax.plot(range(len(self.temp)), self.temp)
        self.canvas.draw()

    def update_temperatures(self, new_temp):
        self.temp = new_temp
        self.line.set_ydata(self.temp)
        self.ax.relim()
        self.ax.autoscale_view(True, True)
        self.canvas.draw()

class MainWindow(QMainWindow):
    def __init__(self, temp):
        super().__init()
        self.temp = temp
        self.initUI()

    def initUI(self):
        image_label = QLabel(self)
        pixmap = QPixmap('human_structure.png')
        image_label.setPixmap(pixmap)

        temp_chart = TemperatureChart(self.temp)

        splitter = QSplitter(self)
        splitter.addWidget(image_label)
        splitter.addWidget(temp_chart)

        top_widget = QWidget(self)
        top_layout = QVBoxLayout()
        top_widget.setLayout(top_layout)

        button_layout = QHBoxLayout()

        connect_button = QPushButton('Подключение', self)
        button_layout.addWidget(connect_button)

        disconnect_button = QPushButton('Отключение', self)
        button_layout.addWidget(disconnect_button)

        refresh_button = QPushButton('Обновить порты', self)
        button_layout.addWidget(refresh_button)

        port_combo_box = QComboBox(self)
        top_layout.addLayout(button_layout)
        top_layout.addWidget(port_combo_box)

        self.setMenuWidget(top_widget)

        self.setWindowTitle('Отображение изображения и температуры')
        self.setGeometry(100, 100, pixmap.width(), pixmap.height())

def main():
    temp = [25, 26, 27, 28, 29, 30, 31]
    app = QApplication(sys.argv)
    window = MainWindow(temp)
    window.show()

    new_temp = [32, 33, 34, 35, 36, 37, 38]
    window.update_temperatures(new_temp)

    sys.exit(app.exec_())

if __name__ == '__main__':
    main()
