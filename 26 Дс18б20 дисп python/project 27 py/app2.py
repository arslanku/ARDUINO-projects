import sys
from PyQt5.QtWidgets import QApplication, QMainWindow, QGraphicsView, QGraphicsScene, QGraphicsPixmapItem
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QPixmap


class HumanStructureApp(QMainWindow):
    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):
        pixmap = QPixmap('human_structure.png')

        self.view = QGraphicsView(self)
        self.scene = QGraphicsScene(self)
        self.view.setScene(self.scene)

        item = QGraphicsPixmapItem(pixmap)
        self.scene.addItem(item)

        self.view.setSceneRect(item.boundingRect())
        self.setCentralWidget(self.view)

        self.view.setAlignment(Qt.AlignCenter)

        self.resize(pixmap.width(), pixmap.height())

        self.setWindowTitle('Строение человека')


def main():
    app = QApplication(sys.argv)
    window = HumanStructureApp()
    window.show()
    sys.exit(app.exec_())


if __name__ == '__main__':
    main()
