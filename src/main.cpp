#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QFileDialog>
#include <QTabWidget>
#include <QGroupBox>
#include <QFile>
#include <QTextStream>
#include <QTimer>
#include <sstream>
#include <vector>

#include "core/page_replacement.h"
#include "io/input_csv.h"

class PageReplacementWindow : public QMainWindow
{
    Q_OBJECT

public:
    PageReplacementWindow(QWidget *parent = nullptr)
        : QMainWindow(parent), currentStepIdx(0), isPlaying(false)
    {
        setWindowTitle("Page Replacement Simulator (FIFO - LRU - OPT - Second-Chance) - OS_VME_13");
        resize(1050, 720);

        auto *centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);

        auto *mainLayout = new QVBoxLayout(centralWidget);
        mainLayout->setSpacing(12);

        // Header Card
        auto *headerBox = new QGroupBox("Cấu Hình Tham Số Mô Phỏng");
        auto *headerLayout = new QGridLayout(headerBox);

        auto *lblRef = new QLabel("<b>Chuỗi tham chiếu trang (Reference String):</b>");
        refInput = new QLineEdit("7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1");
        refInput->setPlaceholderText("VD: 7 0 1 2 0 3 0 4 hoặc nạp từ file CSV");

        btnLoadCsv = new QPushButton("📂 Nạp từ CSV");
        btnExportCsv = new QPushButton("💾 Xuất CSV kết quả");

        auto *lblFrames = new QLabel("<b>Số Khung Trang (Frames):</b>");
        frameSpin = new QSpinBox();
        frameSpin->setRange(1, 15);
        frameSpin->setValue(3);

        auto *lblAlgo = new QLabel("<b>Thuật toán:</b>");
        algoCombo = new QComboBox();
        algoCombo->addItems({"FIFO (First-In, First-Out)", 
                             "LRU (Least Recently Used)", 
                             "OPT (Optimal)", 
                             "Second-Chance (Clock)"});

        btnRunAll = new QPushButton("⚡ Chạy & So sánh tất cả (Compare 4 Algos)");
        btnRunAll->setStyleSheet("background-color: #2b5797; color: white; font-weight: bold; padding: 6px;");

        headerLayout->addWidget(lblRef, 0, 0);
        headerLayout->addWidget(refInput, 0, 1, 1, 2);
        headerLayout->addWidget(btnLoadCsv, 0, 3);
        headerLayout->addWidget(btnExportCsv, 0, 4);

        headerLayout->addWidget(lblFrames, 1, 0);
        headerLayout->addWidget(frameSpin, 1, 1);
        headerLayout->addWidget(lblAlgo, 1, 2);
        headerLayout->addWidget(algoCombo, 1, 3);
        headerLayout->addWidget(btnRunAll, 1, 4);

        mainLayout->addWidget(headerBox);

        // Control Toolbar for Step-by-Step
        auto *ctrlLayout = new QHBoxLayout();
        btnSimulate = new QPushButton("▶ Mô phỏng từng bước (Step-by-step)");
        btnSimulate->setStyleSheet("background-color: #0078d7; color: white; font-weight: bold; padding: 6px 12px;");

        btnNextStep = new QPushButton("⏩ Bước tiếp theo");
        btnNextStep->setEnabled(false);

        btnAutoPlay = new QPushButton("⏯ Tự động chạy (Auto Play)");
        btnAutoPlay->setEnabled(false);

        btnReset = new QPushButton("🔄 Đặt lại (Reset)");

        ctrlLayout->addWidget(btnSimulate);
        ctrlLayout->addWidget(btnNextStep);
        ctrlLayout->addWidget(btnAutoPlay);
        ctrlLayout->addWidget(btnReset);
        ctrlLayout->addStretch();

        mainLayout->addLayout(ctrlLayout);

        // Main Tab Widget: Tab 1 (Chi tiết từng bước) & Tab 2 (Bảng So sánh 4 thuật toán)
        tabs = new QTabWidget();

        // TAB 1: Chi tiết từng bước
        auto *tabStep = new QWidget();
        auto *stepLayout = new QVBoxLayout(tabStep);

        statsLabel = new QLabel("<b>Thống kê:</b> Nhấn 'Mô phỏng' để bắt đầu tính toán.");
        statsLabel->setStyleSheet("font-size: 14px; color: #107c41; padding: 4px;");
        stepLayout->addWidget(statsLabel);

        tableWidget = new QTableWidget();
        tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        tableWidget->setAlternatingRowColors(true);
        stepLayout->addWidget(tableWidget);

        tabs->addTab(tabStep, "🔍 Trực quan hóa Từng bước (Step Matrix)");

        // TAB 2: So sánh 4 thuật toán
        auto *tabCompare = new QWidget();
        auto *compareLayout = new QVBoxLayout(tabCompare);

        compareTable = new QTableWidget();
        compareTable->setColumnCount(5);
        compareTable->setHorizontalHeaderLabels({"Thuật toán", "Tổng truy cập", "Page Hits", "Page Faults", "Tỉ lệ Lỗi (Fault Rate)"});
        compareTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        compareLayout->addWidget(compareTable);

        summaryNote = new QLabel("<b>Đánh giá:</b> Chạy so sánh để xem thuật toán nào tối ưu nhất cho chuỗi tham chiếu hiện tại.");
        summaryNote->setWordWrap(true);
        compareLayout->addWidget(summaryNote);

        tabs->addTab(tabCompare, "📊 Bảng Tổng hợp & So sánh (Benchmark)");

        mainLayout->addWidget(tabs);

        // Timer for auto play
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &PageReplacementWindow::onNextStep);

        // Signals & Slots
        connect(btnSimulate, &QPushButton::clicked, this, &PageReplacementWindow::onStartSimulation);
        connect(btnNextStep, &QPushButton::clicked, this, &PageReplacementWindow::onNextStep);
        connect(btnAutoPlay, &QPushButton::clicked, this, &PageReplacementWindow::onToggleAutoPlay);
        connect(btnReset, &QPushButton::clicked, this, &PageReplacementWindow::onReset);
        connect(btnRunAll, &QPushButton::clicked, this, &PageReplacementWindow::onCompareAll);
        connect(btnLoadCsv, &QPushButton::clicked, this, &PageReplacementWindow::onLoadCsv);
        connect(btnExportCsv, &QPushButton::clicked, this, &PageReplacementWindow::onExportCsv);
    }

private slots:
    std::vector<int> parseRefs()
    {
        std::vector<int> refs;
        std::stringstream ss(refInput->text().toStdString());
        int val;
        while (ss >> val) {
            refs.push_back(val);
        }
        return refs;
    }

    void onStartSimulation()
    {
        auto refs = parseRefs();
        if (refs.empty()) {
            QMessageBox::warning(this, "Lỗi Nhập Liệu", "Vui lòng nhập ít nhất một trang tham chiếu!");
            return;
        }

        int frameCount = frameSpin->value();
        int algoIdx = algoCombo->currentIndex();
        Algorithm algo = Algorithm::FIFO;
        if (algoIdx == 1) algo = Algorithm::LRU;
        else if (algoIdx == 2) algo = Algorithm::OPT;
        else if (algoIdx == 3) algo = Algorithm::SecondChance;

        try {
            currentResult = simulate(algo, refs, frameCount);
        } catch (const std::exception &ex) {
            QMessageBox::critical(this, "Lỗi", ex.what());
            return;
        }

        currentStepIdx = 0;

        // Thiết lập bảng
        int cols = 4 + frameCount; // Step, Page, Result, Victim, Frame 0..N-1
        tableWidget->setColumnCount(cols);
        tableWidget->setRowCount(0);

        QStringList headers;
        headers << "Bước" << "Trang (Page)" << "Kết quả" << "Trang bị thay (Victim)";
        for (int i = 0; i < frameCount; ++i) {
            headers << QString("Frame %1").arg(i);
        }
        tableWidget->setHorizontalHeaderLabels(headers);

        btnNextStep->setEnabled(true);
        btnAutoPlay->setEnabled(true);
        statsLabel->setText(QString("<b>Đang mô phỏng:</b> %1 | Tổng truy cập: %2")
                                .arg(algoCombo->currentText())
                                .arg(refs.size()));

        tabs->setCurrentIndex(0);
        onNextStep(); // Bước đầu tiên
    }

    void onNextStep()
    {
        if (currentStepIdx >= (int)currentResult.steps.size()) {
            if (isPlaying) onToggleAutoPlay();
            btnNextStep->setEnabled(false);
            statsLabel->setText(QString("<b>Hoàn tất!</b> Hits: %1 | Faults: %2 | Tỉ lệ lỗi: %3%")
                                    .arg(currentResult.hits)
                                    .arg(currentResult.faults)
                                    .arg(QString::number((double)currentResult.faults * 100.0 / currentResult.steps.size(), 'f', 2)));
            return;
        }

        const auto &s = currentResult.steps[currentStepIdx];
        int row = tableWidget->rowCount();
        tableWidget->insertRow(row);

        tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(s.index)));
        tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(s.page)));

        auto *resItem = new QTableWidgetItem(s.hit ? "HIT" : "PAGE FAULT");
        if (s.hit) {
            resItem->setBackground(QBrush(QColor(198, 239, 206)));
            resItem->setForeground(QBrush(QColor(0, 97, 0)));
        } else {
            resItem->setBackground(QBrush(QColor(255, 199, 206)));
            resItem->setForeground(QBrush(QColor(156, 0, 6)));
        }
        tableWidget->setItem(row, 2, resItem);

        tableWidget->setItem(row, 3, new QTableWidgetItem(s.victim == -1 ? "-" : QString::number(s.victim)));

        for (int i = 0; i < (int)s.frames.size(); ++i) {
            int p = s.frames[i];
            auto *frameItem = new QTableWidgetItem(p == -1 ? "-" : QString::number(p));
            if (p == s.page) {
                frameItem->setBackground(QBrush(QColor(255, 235, 156))); // Highlight trang vừa truy xuất
            }
            tableWidget->setItem(row, 4 + i, frameItem);
        }

        tableWidget->scrollToBottom();
        currentStepIdx++;

        statsLabel->setText(QString("Bước %1 / %2 | Faults hiện tại: %3")
                                .arg(currentStepIdx)
                                .arg(currentResult.steps.size())
                                .arg(s.faultsSoFar));
    }

    void onToggleAutoPlay()
    {
        if (isPlaying) {
            timer->stop();
            isPlaying = false;
            btnAutoPlay->setText("⏯ Tự động chạy (Auto Play)");
        } else {
            timer->start(350); // 350ms mỗi bước
            isPlaying = true;
            btnAutoPlay->setText("⏸ Tạm dừng");
        }
    }

    void onReset()
    {
        if (isPlaying) onToggleAutoPlay();
        currentStepIdx = 0;
        tableWidget->setRowCount(0);
        btnNextStep->setEnabled(false);
        btnAutoPlay->setEnabled(false);
        statsLabel->setText("<b>Trạng thái:</b> Đã đặt lại.");
    }

    void onCompareAll()
    {
        auto refs = parseRefs();
        if (refs.empty()) {
            QMessageBox::warning(this, "Lỗi Nhập Liệu", "Vui lòng nhập chuỗi tham chiếu!");
            return;
        }

        int frameCount = frameSpin->value();

        try {
            Result resFIFO = simulate(Algorithm::FIFO, refs, frameCount);
            Result resLRU = simulate(Algorithm::LRU, refs, frameCount);
            Result resOPT = simulate(Algorithm::OPT, refs, frameCount);
            Result resSC = simulate(Algorithm::SecondChance, refs, frameCount);

            compareTable->setRowCount(4);

            auto populateRow = [&](int row, QString name, const Result &r) {
                compareTable->setItem(row, 0, new QTableWidgetItem(name));
                compareTable->setItem(row, 1, new QTableWidgetItem(QString::number(refs.size())));
                compareTable->setItem(row, 2, new QTableWidgetItem(QString::number(r.hits)));
                compareTable->setItem(row, 3, new QTableWidgetItem(QString::number(r.faults)));
                double rate = (double)r.faults * 100.0 / refs.size();
                auto *itemRate = new QTableWidgetItem(QString("%1%").arg(QString::number(rate, 'f', 2)));
                if (r.algo == Algorithm::OPT) itemRate->setBackground(QBrush(QColor(198, 239, 206)));
                compareTable->setItem(row, 4, itemRate);
            };

            populateRow(0, "FIFO", resFIFO);
            populateRow(1, "LRU", resLRU);
            populateRow(2, "OPT (Optimal)", resOPT);
            populateRow(3, "Second-Chance (Clock)", resSC);

            summaryNote->setText(QString("<b>Kết quả phân tích:</b> Với %1 khung trang, thuật toán <b>OPT</b> tối ưu nhất lý thuyết với %2 faults. Thuật toán <b>Second-Chance</b> đạt %3 faults, tiệm cận tốt hiệu năng của LRU mà chi phí tính toán thấp hơn.")
                                     .arg(frameCount)
                                     .arg(resOPT.faults)
                                     .arg(resSC.faults));

            tabs->setCurrentIndex(1); // Chuyển sang Tab So sánh
        } catch (const std::exception &ex) {
            QMessageBox::critical(this, "Lỗi", ex.what());
        }
    }

    void onLoadCsv()
    {
        QString path = QFileDialog::getOpenFileName(this, "Chọn file CSV", "", "CSV Files (*.csv)");
        if (path.isEmpty()) return;

        try {
            auto cases = loadInputCsv(path.toStdString());
            if (!cases.empty()) {
                const auto &first = cases[0];
                frameSpin->setValue(first.frameCount);
                QString refStr;
                for (size_t i = 0; i < first.refs.size(); ++i) {
                    refStr += QString::number(first.refs[i]);
                    if (i < first.refs.size() - 1) refStr += " ";
                }
                refInput->setText(refStr);
                QMessageBox::information(this, "Thành công", QString("Đã nạp bộ dữ liệu %1 (Frames: %2, Pages: %3) từ CSV!").arg(QString::fromStdString(first.testId)).arg(first.frameCount).arg(first.refs.size()));
            }
        } catch (const std::exception &ex) {
            QMessageBox::warning(this, "Lỗi đọc CSV", ex.what());
        }
    }

    void onExportCsv()
    {
        if (tableWidget->rowCount() == 0) {
            QMessageBox::warning(this, "Cảnh báo", "Chưa có dữ liệu mô phỏng để xuất!");
            return;
        }

        QString path = QFileDialog::getSaveFileName(this, "Lưu file CSV", "trace_result.csv", "CSV Files (*.csv)");
        if (path.isEmpty()) return;

        QFile file(path);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            // Header
            for (int j = 0; j < tableWidget->columnCount(); ++j) {
                out << tableWidget->horizontalHeaderItem(j)->text();
                if (j < tableWidget->columnCount() - 1) out << ",";
            }
            out << "\n";

            // Rows
            for (int i = 0; i < tableWidget->rowCount(); ++i) {
                for (int j = 0; j < tableWidget->columnCount(); ++j) {
                    out << tableWidget->item(i, j)->text();
                    if (j < tableWidget->columnCount() - 1) out << ",";
                }
                out << "\n";
            }
            QMessageBox::information(this, "Thành công", "Đã xuất kết quả ra file CSV thành công!");
        }
    }

private:
    QLineEdit *refInput;
    QSpinBox *frameSpin;
    QComboBox *algoCombo;
    QPushButton *btnSimulate;
    QPushButton *btnNextStep;
    QPushButton *btnAutoPlay;
    QPushButton *btnReset;
    QPushButton *btnRunAll;
    QPushButton *btnLoadCsv;
    QPushButton *btnExportCsv;

    QTabWidget *tabs;
    QTableWidget *tableWidget;
    QTableWidget *compareTable;
    QLabel *statsLabel;
    QLabel *summaryNote;

    Result currentResult;
    int currentStepIdx;
    bool isPlaying;
    QTimer *timer;
};

#include "main.moc"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setStyle("Fusion"); // Giao diện chuẩn hiện đại

    PageReplacementWindow window;
    window.show();

    return app.exec();
}
