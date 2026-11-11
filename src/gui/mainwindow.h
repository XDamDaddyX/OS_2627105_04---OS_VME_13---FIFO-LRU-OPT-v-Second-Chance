#pragma once

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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

private slots:
    void onSimulateStepByStep();
    void onNextStep();
    void onAutoPlay();
    void onReset();
    void onRunAllComparison();
    void onLoadCsv();
    void onExportCsv();
    void onRunStressTest();

private:
    std::vector<int> parseRefString(const QString &text);
    void updateTableStep(int stepIdx);

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
    QPushButton *btnStressTest;

    QTabWidget *tabs;
    QTableWidget *tableWidget;
    QTableWidget *compareTable;
    QTableWidget *stressTable;
    QLabel *statsLabel;
    QLabel *summaryNote;
    QLabel *stressLabel;

    Result currentResult;
    int currentStepIdx;
    bool isPlaying;
    QTimer *timer;
};

