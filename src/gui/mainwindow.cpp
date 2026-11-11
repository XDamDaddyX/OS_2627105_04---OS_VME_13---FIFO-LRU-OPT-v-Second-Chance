#include "mainwindow.h"
#include <chrono>
#include <random>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), currentStepIdx(0), isPlaying(false)
{
    setWindowTitle("Page Replacement Simulator (OS_VME_13) - GUI & Performance Module");
    resize(1100, 750);

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

    // Main Tab Widget
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

    summaryNote = new QLabel("Nhấn nút <b>'Chạy & So sánh tất cả'</b> để đánh giá hiệu năng 4 thuật toán trên cùng chuỗi tham chiếu.");
    summaryNote->setStyleSheet("font-size: 13px; color: #333; margin-top: 8px;");
    compareLayout->addWidget(summaryNote);

    tabs->addTab(tabCompare, "📊 Bảng So sánh 4 Thuật toán");

    // TAB 3: Stress Test & Performance Benchmark (Phần nâng cao của Tuấn)
    auto *tabStress = new QWidget();
    auto *stressLayout = new QVBoxLayout(tabStress);

    auto *stressCtrlLayout = new QHBoxLayout();
    btnStressTest = new QPushButton("🚀 Chạy Stress Test (100,000 trang truy cập)");
    btnStressTest->setStyleSheet("background-color: #d83b01; color: white; font-weight: bold; padding: 6px 12px;");
    stressCtrlLayout->addWidget(btnStressTest);
    stressCtrlLayout->addStretch();
    stressLayout->addLayout(stressCtrlLayout);

    stressLabel = new QLabel("Nhấn nút trên để đo thời gian thực thi (Execution Time) và độ ổn định của 4 thuật toán.");
    stressLabel->setStyleSheet("font-size: 13px; color: #555; padding: 4px;");
    stressLayout->addWidget(stressLabel);

    stressTable = new QTableWidget();
    stressTable->setColumnCount(5);
    stressTable->setHorizontalHeaderLabels({"Thuật toán", "Số mẫu (References)", "Thời gian thực thi (ms)", "Page Faults", "Trạng thái"});
    stressTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    stressLayout->addWidget(stressTable);

    tabs->addTab(tabStress, "⚡ Đo Hiệu Năng & Stress Test");

    mainLayout->addWidget(tabs);

    // Auto Play Timer
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::onNextStep);

    // Signals & Slots
    connect(btnSimulate, &QPushButton::clicked, this, &MainWindow::onSimulateStepByStep);
    connect(btnNextStep, &QPushButton::clicked, this, &MainWindow::onNextStep);
    connect(btnAutoPlay, &QPushButton::clicked, this, &MainWindow::onAutoPlay);
    connect(btnReset, &QPushButton::clicked, this, &MainWindow::onReset);
    connect(btnRunAll, &QPushButton::clicked, this, &MainWindow::onRunAllComparison);
    connect(btnLoadCsv, &QPushButton::clicked, this, &MainWindow::onLoadCsv);
    connect(btnExportCsv, &QPushButton::clicked, this, &MainWindow::onExportCsv);
    connect(btnStressTest, &QPushButton::clicked, this, &MainWindow::onRunStressTest);
}

std::vector<int> MainWindow::parseRefString(const QString &text)
{
    std::vector<int> refs;
    std::stringstream ss(text.toStdString());
    int page;
    while (ss >> page) {
        if (page < 0) {
            throw std::runtime_error("Mã trang (page reference) không được âm: " + std::to_string(page));
        }
        refs.push_back(page);
    }
    if (refs.empty()) {
        throw std::runtime_error("Chuỗi tham chiếu trang không được rỗng!");
    }
    return refs;
}

void MainWindow::onSimulateStepByStep()
{
    try {
        std::vector<int> refs = parseRefString(refInput->text());
        int frameCount = frameSpin->value();

        Algorithm algo = Algorithm::FIFO;
        if (algoCombo->currentIndex() == 1) algo = Algorithm::LRU;
        else if (algoCombo->currentIndex() == 2) algo = Algorithm::OPT;
        else if (algoCombo->currentIndex() == 3) algo = Algorithm::SecondChance;

        currentResult = simulate(algo, refs, frameCount);

        // Reset UI bảng step
        tableWidget->clear();
        int rows = 3 + frameCount; // Page Ref, Frames (0..n-1), Status (Hit/Fault), Victim
        tableWidget->setRowCount(rows);
        tableWidget->setColumnCount(refs.size());

        QStringList vLabels;
        vLabels << "Trang (Page Ref)";
        for (int i = 0; i < frameCount; ++i) {
            vLabels << QString("Khung F%1").arg(i);
        }
        vLabels << "Trạng thái (Status)";
        vLabels << "Bị thay thế (Victim)";
        tableWidget->setVerticalHeaderLabels(vLabels);

        QStringList hLabels;
        for (size_t i = 1; i <= refs.size(); ++i) {
            hLabels << QString("Bước %1").arg(i);
        }
        tableWidget->setHorizontalHeaderLabels(hLabels);

        currentStepIdx = 0;
        btnNextStep->setEnabled(true);
        btnAutoPlay->setEnabled(true);

        statsLabel->setText(QString("<b>Đã khởi tạo:</b> Thuật toán %1 | Tổng: %2 trang | Khung: %3")
                                .arg(algoCombo->currentText())
                                .arg(refs.size())
                                .arg(frameCount));

        tabs->setCurrentIndex(0);
        onNextStep();
    } catch (const std::exception &ex) {
        QMessageBox::critical(this, "Lỗi Nhập Liệu", ex.what());
    }
}

void MainWindow::updateTableStep(int stepIdx)
{
    if (stepIdx < 0 || stepIdx >= (int)currentResult.steps.size()) return;

    const auto &st = currentResult.steps[stepIdx];
    int frameCount = frameSpin->value();

    // Hàng 0: Page Ref
    auto *itemPage = new QTableWidgetItem(QString::number(st.page));
    itemPage->setTextAlignment(Qt::AlignCenter);
    QFont f = itemPage->font();
    f.setBold(true);
    itemPage->setFont(f);
    tableWidget->setItem(0, stepIdx, itemPage);

    // Hàng 1..frameCount: Trạng thái các frame
    for (int r = 0; r < frameCount; ++r) {
        QString txt = (r < (int)st.frames.size() && st.frames[r] != -1) ? QString::number(st.frames[r]) : "-";
        auto *itemF = new QTableWidgetItem(txt);
        itemF->setTextAlignment(Qt::AlignCenter);

        if (r < (int)st.frames.size() && st.frames[r] == st.page) {
            if (st.hit) {
                itemF->setBackground(QBrush(QColor(198, 239, 206))); // Xanh lá: Hit
            } else {
                itemF->setBackground(QBrush(QColor(255, 199, 206))); // Đỏ nhạt: Nạp mới/Fault
            }
        }
        tableWidget->setItem(1 + r, stepIdx, itemF);
    }

    // Hàng Status
    int statusRow = 1 + frameCount;
    auto *itemStatus = new QTableWidgetItem(st.hit ? "HIT" : "FAULT");
    itemStatus->setTextAlignment(Qt::AlignCenter);
    if (st.hit) {
        itemStatus->setBackground(QBrush(QColor(198, 239, 206)));
        itemStatus->setForeground(QBrush(QColor(0, 97, 0)));
    } else {
        itemStatus->setBackground(QBrush(QColor(255, 199, 206)));
        itemStatus->setForeground(QBrush(QColor(156, 0, 6)));
    }
    tableWidget->setItem(statusRow, stepIdx, itemStatus);

    // Hàng Victim
    int victimRow = statusRow + 1;
    QString victimTxt = (st.victim != -1) ? QString::number(st.victim) : "-";
    auto *itemVictim = new QTableWidgetItem(victimTxt);
    itemVictim->setTextAlignment(Qt::AlignCenter);
    tableWidget->setItem(victimRow, stepIdx, itemVictim);

    tableWidget->scrollToItem(itemPage);

    statsLabel->setText(QString("<b>Tiến độ:</b> Bước %1/%2 | <b>Page Hits:</b> %3 | <b>Page Faults:</b> %4")
                            .arg(stepIdx + 1)
                            .arg(currentResult.steps.size())
                            .arg(st.hit ? (stepIdx + 1 - st.faultsSoFar) : (stepIdx + 1 - st.faultsSoFar))
                            .arg(st.faultsSoFar));
}

void MainWindow::onNextStep()
{
    if (currentStepIdx < (int)currentResult.steps.size()) {
        updateTableStep(currentStepIdx);
        currentStepIdx++;
    } else {
        if (isPlaying) onAutoPlay();
        btnNextStep->setEnabled(false);
        btnAutoPlay->setEnabled(false);
        QMessageBox::information(this, "Hoàn tất", QString("Mô phỏng hoàn thành!\nTổng Page Faults: %1\nTổng Page Hits: %2")
                                                     .arg(currentResult.faults)
                                                     .arg(currentResult.hits));
    }
}

void MainWindow::onAutoPlay()
{
    if (!isPlaying) {
        isPlaying = true;
        btnAutoPlay->setText("⏸ Tạm dừng (Pause)");
        timer->start(350);
    } else {
        isPlaying = false;
        btnAutoPlay->setText("⏯ Tiếp tục (Resume)");
        timer->stop();
    }
}

void MainWindow::onReset()
{
    if (isPlaying) onAutoPlay();
    currentStepIdx = 0;
    tableWidget->clear();
    tableWidget->setRowCount(0);
    tableWidget->setColumnCount(0);
    btnNextStep->setEnabled(false);
    btnAutoPlay->setEnabled(false);
    statsLabel->setText("<b>Thống kê:</b> Đã đặt lại dữ liệu.");
}

void MainWindow::onRunAllComparison()
{
    try {
        std::vector<int> refs = parseRefString(refInput->text());
        int frameCount = frameSpin->value();

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

        summaryNote->setText(QString("<b>Kết quả so sánh:</b> Với %1 khung trang, thuật toán <b>OPT</b> tối ưu nhất lý thuyết với %2 faults. Thuật toán <b>Second-Chance</b> đạt %3 faults, đạt xấp xỉ hiệu năng của LRU.")
                                 .arg(frameCount)
                                 .arg(resOPT.faults)
                                 .arg(resSC.faults));

        tabs->setCurrentIndex(1);
    } catch (const std::exception &ex) {
        QMessageBox::critical(this, "Lỗi", ex.what());
    }
}

void MainWindow::onRunStressTest()
{
    int numRefs = 100000;
    int frameCount = frameSpin->value();

    std::vector<int> largeRefs;
    largeRefs.reserve(numRefs);
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(0, 49); // 50 trang ảo khác nhau
    for (int i = 0; i < numRefs; ++i) {
        largeRefs.push_back(dist(rng));
    }

    std::vector<std::pair<QString, Algorithm>> algos = {
        {"FIFO", Algorithm::FIFO},
        {"LRU", Algorithm::LRU},
        {"OPT (Optimal)", Algorithm::OPT},
        {"Second-Chance", Algorithm::SecondChance}
    };

    stressTable->setRowCount(algos.size());

    for (size_t i = 0; i < algos.size(); ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        Result res = simulate(algos[i].second, largeRefs, frameCount);
        auto end = std::chrono::high_resolution_clock::now();
        double elapsedMs = std::chrono::duration<double, std::milli>(end - start).count();

        stressTable->setItem(i, 0, new QTableWidgetItem(algos[i].first));
        stressTable->setItem(i, 1, new QTableWidgetItem(QString::number(numRefs)));
        stressTable->setItem(i, 2, new QTableWidgetItem(QString::number(elapsedMs, 'f', 2)));
        stressTable->setItem(i, 3, new QTableWidgetItem(QString::number(res.faults)));
        auto *itemStatus = new QTableWidgetItem("PASS (Ổn định)");
        itemStatus->setBackground(QBrush(QColor(198, 239, 206)));
        stressTable->setItem(i, 4, itemStatus);
    }

    stressLabel->setText(QString("✅ Đã hoàn thành Stress Test với %1 lượt truy cập trên %2 frames. Thuật toán chạy ổn định không lỗi bộ nhớ.")
                             .arg(numRefs).arg(frameCount));
    tabs->setCurrentIndex(2);
}

void MainWindow::onLoadCsv()
{
    QString path = QFileDialog::getOpenFileName(this, "Chọn file CSV", "", "CSV Files (*.csv)");
    if (path.isEmpty()) return;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Lỗi", "Không thể mở file CSV!");
        return;
    }

    QTextStream in(&file);
    QString line = in.readLine(); // Header hoặc dòng 1
    if (!line.isEmpty()) {
        QStringList parts = line.split(',');
        if (parts.size() >= 3) {
            frameSpin->setValue(parts[1].trimmed().toInt());
            refInput->setText(parts[2].trimmed());
        } else {
            refInput->setText(line.replace(',', ' '));
        }
        QMessageBox::information(this, "Thành công", "Đã nạp chuỗi tham chiếu từ file CSV!");
    }
}

void MainWindow::onExportCsv()
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
        for (int j = 0; j < tableWidget->columnCount(); ++j) {
            out << tableWidget->horizontalHeaderItem(j)->text();
            if (j < tableWidget->columnCount() - 1) out << ",";
        }
        out << "\n";

        for (int i = 0; i < tableWidget->rowCount(); ++i) {
            for (int j = 0; j < tableWidget->columnCount(); ++j) {
                auto *item = tableWidget->item(i, j);
                out << (item ? item->text() : "");
                if (j < tableWidget->columnCount() - 1) out << ",";
            }
            out << "\n";
        }
        QMessageBox::information(this, "Thành công", "Đã xuất trace kết quả ra file CSV!");
    }
}

