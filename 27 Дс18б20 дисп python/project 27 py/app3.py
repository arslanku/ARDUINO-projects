import sys
from PyQt5.QtWidgets import QApplication, QMainWindow, QLabel, QPushButton, QComboBox, QVBoxLayout, QSplitter, QWidget
from PyQt5.QtGui import QPixmap, QFont


class MainWindow(QMainWindow):
    def __init__(self, temp):
        super().__init__()
        self.temp = temp
        self.initUI()

    def initUI(self):
        # Создаем QLabel для отображения изображения
        image_label = QLabel(self)
        pixmap = QPixmap('human_structure.png')
        image_label.setPixmap(pixmap)

        # Создаем QVBoxLayout для окошек с температурой
        temp_layout = QVBoxLayout()

        # Создаем QLabel для каждой температуры и добавляем их в QVBoxLayout
        for i, t in enumerate(self.temp):
            temp_label = QLabel(f'Temp{i + 1}: {t} °C', self)

            # Устанавливаем желаемый шрифт для текста температуры
            font = QFont()
            font.setPointSize(16)  # Размер шрифта (в пунктах)
            temp_label.setFont(font)

            temp_layout.addWidget(temp_label)

        # Создаем вертикальный сплиттер для разделения изображения и температур
        splitter = QSplitter(self)
        splitter.addWidget(image_label)

        # Создаем виджет для температур и добавляем его в сплиттер
        temp_widget = QWidget(self)
        temp_widget.setLayout(temp_layout)
        splitter.addWidget(temp_widget)

        # Устанавливаем сплиттер как центральный виджет главного окна
        self.setCentralWidget(splitter)

        # Создаем верхний виджет для кнопок и комбо-бокса
        top_widget = QWidget(self)
        top_layout = QVBoxLayout()
        top_widget.setLayout(top_layout)

        # Кнопка "Подключение"
        connect_button = QPushButton('Подключение', self)
        top_layout.addWidget(connect_button)

        # Кнопка "Отключение"
        disconnect_button = QPushButton('Отключение', self)
        top_layout.addWidget(disconnect_button)

        refresh_button = QPushButton("Обновить порты", self)
        top_layout.addWidget(refresh_button)

        # Комбо-бокс
        port_combo_box = QComboBox(self)
        top_layout.addWidget(port_combo_box)

        # Устанавливаем верхний виджет в качестве верхней области окна
        self.setMenuWidget(top_widget)

        # Устанавливаем заголовок окна и размер
        self.setWindowTitle('Отображение изображения и температуры')
        self.setGeometry(100, 100, pixmap.width(), pixmap.height())


def main():
    temp = [25, 26, 27, 28, 29, 30, 31]  # Пример массива температур
    app = QApplication(sys.argv)
    window = MainWindow(temp)
    window.show()
    sys.exit(app.exec_())


if __name__ == '__main__':
    main()
