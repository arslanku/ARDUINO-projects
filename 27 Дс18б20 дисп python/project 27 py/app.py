import datetime
import sys

from PyQt5.QtCore import QIODevice, Qt
from PyQt5.QtGui import QPixmap
from PyQt5.QtSerialPort import QSerialPort, QSerialPortInfo
from PyQt5.QtWidgets import QApplication, QMainWindow, QVBoxLayout, QMessageBox, QWidget, \
    QPushButton, QComboBox, QLabel, QTextEdit, QHBoxLayout, QGraphicsView, QGraphicsScene, QGraphicsPixmapItem


class MainWindow(QMainWindow):
    def __init__(self, baudRate):
        super().__init__()
        self.initImage()

        self.setWindowTitle("Temp")
        self.setGeometry(100, 100, 800, 600)

        self.central_widget = QWidget(self)
        self.setCentralWidget(self.central_widget)
        self.layout = QVBoxLayout(self.central_widget)

        self.port_layout = QHBoxLayout()
        self.layout.addLayout(self.port_layout)
        self.port_label = QLabel("Выберите порт:", self)
        self.port_layout.addWidget(self.port_label)

        self.port_combo_box = QComboBox(self)
        self.port_layout.addWidget(self.port_combo_box)

        self.refresh_button = QPushButton("Обновить порты", self)
        self.port_layout.addWidget(self.refresh_button)
        self.refresh_button.clicked.connect(self.refresh_ports)

        self.connect_button = QPushButton("Подключиться", self)
        self.port_layout.addWidget(self.connect_button)
        self.connect_button.clicked.connect(self.connect_button_clicked)

        self.disconnect_button = QPushButton("Отключиться", self)
        self.port_layout.addWidget(self.disconnect_button)
        self.disconnect_button.clicked.connect(self.disconnect_button_clicked)

        # self.log_label = QLabel("Логи:", self)
        # self.layout.addWidget(self.log_label)
        #
        # self.log_text_edit = QTextEdit(self)
        # self.layout.addWidget(self.log_text_edit)
        #
        # self.clear_button = QPushButton("Очистить", self)
        # self.layout.addWidget(self.clear_button)
        # self.clear_button.clicked.connect(self.clear_log)

        self.serial = None
        self.baudRate = baudRate

    # def initImage(self):
    #     pixmap = QPixmap('human_structure.png')
    #
    #     self.view = QGraphicsView(self)
    #     self.scene = QGraphicsScene(self)
    #     self.view.setScene(self.scene)
    #
    #     item = QGraphicsPixmapItem(pixmap)
    #     self.scene.addItem(item)
    #
    #     self.view.setSceneRect(item.boundingRect())
    #     self.setCentralWidget(self.view)
    #     self.view.setAlignment(Qt.AlignCenter)
    #     self.resize(pixmap.width(), pixmap.height())
    def initImage(self):
        self.image_view = QGraphicsView(self)
        self.layout.addWidget(self.image_view)

        self.image_scene = QGraphicsScene()
        self.image_view.setScene(self.image_scene)

        self.image = QPixmap('human_structure.png')
        self.image_item = QGraphicsPixmapItem(self.image)
        self.image_scene.addItem(self.image_item)

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
                    self.port_label.setText(f"Подключен к порту: {port_name}")
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
            self.port_label.setText("Выберите порт:")
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

                    temp = text.split(" ")
                    print(temp)

    def process_data(self, data):
        """
        Обработка поступивших данных и вывод
        """
        text = f'{datetime.datetime.now()}:    {data}\n'
        # self.log_text_edit.append(text)

    def clear_log(self):
        """
        Очистка лога
        """
        self.log_text_edit.clear()

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


if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = MainWindow(baudRate=9600)  # НЕ ЗАБУДЬ УСТАНОВИТЬ СКОРОСТЬ ПОРТА !
    window.show()
    sys.exit(app.exec_())
