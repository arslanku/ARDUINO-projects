import sys

from PyQt5.QtCore import QIODevice
from PyQt5.QtSerialPort import QSerialPortInfo, QSerialPort
from PyQt5.QtWidgets import QApplication, QMainWindow, QLabel, QPushButton, QComboBox, QHBoxLayout, QVBoxLayout, \
    QSplitter, QWidget, QMessageBox
from PyQt5.QtGui import QPixmap, QFont


class MainWindow(QMainWindow):
    def __init__(self, baudRate):
        super().__init__()

        self.temp = [0, 0, 0, 0, 0, 0, 0]
        self.baudRate = baudRate

        image_label = QLabel(self)
        pixmap = QPixmap('human_structure.png')
        image_label.setPixmap(pixmap)

        temp_layout = QVBoxLayout()

        self.temp_labels = []
        for i, t in enumerate(self.temp):
            temp_label = QLabel(f'Temp{i + 1}: {t}°C', self)
            font = QFont()
            font.setPointSize(16)
            temp_label.setFont(font)

            temp_layout.addWidget(temp_label)
            self.temp_labels.append(temp_label)

        splitter = QSplitter(self)
        splitter.addWidget(image_label)

        temp_widget = QWidget(self)
        temp_widget.setLayout(temp_layout)
        splitter.addWidget(temp_widget)

        top_widget = QWidget(self)
        top_layout = QVBoxLayout()
        top_widget.setLayout(top_layout)

        button_layout = QHBoxLayout()

        """"""
        self.connect_button = QPushButton('Подключение', self)
        button_layout.addWidget(self.connect_button)
        self.connect_button.clicked.connect(self.connect_button_clicked)

        self.disconnect_button = QPushButton('Отключение', self)
        button_layout.addWidget(self.disconnect_button)
        self.disconnect_button.clicked.connect(self.disconnect_button_clicked)

        self.refresh_button = QPushButton("Обновить порты", self)
        button_layout.addWidget(self.refresh_button)
        self.refresh_button.clicked.connect(self.refresh_ports)

        self.port_combo_box = QComboBox(self)
        button_layout.addWidget(self.port_combo_box)
        """"""

        top_layout.addLayout(button_layout)

        self.setMenuWidget(top_widget)
        self.setCentralWidget(splitter)
        self.setWindowTitle('Отображение температуры')
        self.setGeometry(100, 100, pixmap.width(), pixmap.height())

    # def update_temp(self):
    #     for i, t in enumerate(self.temp):
    #         # temp_label = QLabel(f'Temp{i + 1}: {t} °C', self)
    #         self.temp_label.append(QLabel(f'Temp{i + 1}: {t} °C', self))
    #         font = QFont()
    #         font.setPointSize(16)
    #         self.temp_label[i].setFont(font)
    #         # self.temp_layout.addWidget(temp_label)
    #         self.temp_layout.update(self.temp_layout)
    #         # print(self.temp_layout.update)

    def update_temp(self, new_temp):
        for i, temp_label in enumerate(self.temp_labels):
            if i < len(new_temp):
                temp_label.setText(f'Temp{i + 1}: {new_temp[i]}°C')

    def refresh_ports(self):
        """
        Перезагрузка доступных COM портов
        """
        self.port_combo_box.clear()

        ports = QSerialPortInfo.availablePorts()
        for port in ports:
            self.port_combo_box.addItem(port.portName())

    def connect_button_clicked(self):
        """
        Подключение к COM порту при нажатии кнопки
        """
        port_name = self.port_combo_box.currentText()
        QMessageBox.critical(self, "Ошибка скорости порта", "Установите правильную скорость порта") \
            if self.baudRate == 0 or type(self.baudRate) != int else 0
        if port_name:
            try:
                self.serial = QSerialPort()
                self.serial.setPortName(port_name)
                self.serial.setBaudRate(self.baudRate)
                self.serial.readyRead.connect(self.read_from_serial)

                if self.serial.open(QIODevice.ReadWrite):
                    self.connect_button.setEnabled(False)
                    self.disconnect_button.setEnabled(True)
                    self.refresh_button.setEnabled(False)
                    self.port_combo_box.setEnabled(False)
                    # self.port_label.setText(f"Подключен к порту: {port_name}")
                    # self.log_text_edit.append(f"[INFO] Подключен к порту: {port_name}\n")
                    self.read_from_serial()
                else:
                    QMessageBox.critical(self, "Ошибка", "Не удалось подключиться к порту.")
            except:
                QMessageBox.critical(self, "Ошибка", "Ошибка подключения к COM порту")
        else:
            QMessageBox.warning(self, "Предупреждение", "Выберите порт для подключения.")

    def disconnect_button_clicked(self):
        """
        Отключение от COM порта при нажатии кнопки
        """
        if self.serial is not None and self.serial.isOpen():
            self.serial.close()
            self.connect_button.setEnabled(True)
            self.disconnect_button.setEnabled(False)
            self.refresh_button.setEnabled(True)
            self.port_combo_box.setEnabled(True)
            # self.port_label.setText("Выберите порт:")
            # self.log_text_edit.append(f"[INFO] Отключено\n")

    def read_from_serial(self):
        """
        Чтение данных из порта
        """
        if self.serial is not None and self.serial.isOpen():
            if self.serial.canReadLine():
                data = self.serial.readLine()
                if data:
                    text = data.data().decode('ascii', errors='ignore').strip()  # или использовать это
                    self.process_data(text)

    def process_data(self, data):
        """
        Обработка поступивших данных и вывод
        """
        self.temp = data.split(" ")
        self.update_temp(self.temp)
        print(self.temp)

    def showEvent(self, event):
        """
        При открытии программы
        """
        if event.type() == event.Show:
            self.refresh_ports()

        super(MainWindow, self).showEvent(event)

    def closeEvent(self, event):
        """
        При закрытии программы
        """
        # if self.serial is not None and self.serial.isOpen():
        #     self.serial.close()
        self.serial.close()

        super(MainWindow, self).closeEvent(event)


if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = MainWindow(baudRate=9600)
    window.show()
    sys.exit(app.exec_())
