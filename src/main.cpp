
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QTextEdit>
#include <QFormLayout>

int main(int argc, char *argv[])
{
    // 1. Khoi tao ung dung Qt
    QApplication app(argc, argv);

    // 2. Tao cua so
    QWidget window;
    window.setWindowTitle("OS - Page Replacement Simulator");
    window.resize(650, 450);

    // 3. Tao bo cuc
    QVBoxLayout *layout = new QVBoxLayout(&window);

    QLabel *title = new QLabel("Page Replacement Simulator");
    layout->addWidget(title);

    // 4. Tao cac thanh phan nhap lieu
    QFormLayout *form = new QFormLayout();

    QLineEdit *input = new QLineEdit();
    input->setText("7 0 1 2 0 3 0 4");

    QSpinBox *frames = new QSpinBox();
    frames->setRange(1, 20);
    frames->setValue(3);

    QComboBox *algorithm = new QComboBox();
    algorithm->addItems({
        "FIFO",
        "LRU",
        "OPT",
        "Second-Chance"
    });

    form->addRow("Page String:", input);
    form->addRow("Frames:", frames);
    form->addRow("Algorithm:", algorithm);

    layout->addLayout(form);

    // 5. Tao nut Run
    QPushButton *runButton = new QPushButton("Run Simulation");
    layout->addWidget(runButton);

    // 6. Tao khu vuc ket qua
    QTextEdit *output = new QTextEdit();
    output->setReadOnly(true);
    layout->addWidget(output);

    // 7. Xu ly su kien nhan nut
    QObject::connect(runButton, &QPushButton::clicked,
                     &window, [=]() {
        QString text = QString("Pages: %1\nFrames: %2\nAlgorithm: %3")
            .arg(input->text())
            .arg(frames->value())
            .arg(algorithm->currentText());

        text += "\n\nAlgorithm not implemented yet.";

        output->setPlainText(text);
    });

    // 8. Hien thi cua so
    window.show();

    return app.exec();
}
