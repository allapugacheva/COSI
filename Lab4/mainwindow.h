#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QChart>
#include <QLineSeries>
#include <vector>
#include <complex>
#include <cmath>
#include "interactivechartview.h"
#include <QPushButton>
#include <QLineEdit>
#include <QObject>
#include <QDoubleValidator>
#include <QTimer>
#include <QVBoxLayout>
#include <omp.h>
#include <algorithm>
#include <QRandomGenerator>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    int N;
    double T;
    double step;

    double prev_out;

    std::vector<double> signal;
    std::vector<double> noiseSignal;

    std::vector<double> dftSignal;
    std::vector<double> dftFilteredSignal;

    std::vector<double> filter1;
    std::vector<double> dftFilter1;

    std::vector<double> filtered1;
    std::vector<double> filtered2;

    void makeSignal();
    std::vector<double> dft(const std::vector<double>& vec);
    std::vector<double> conv(const std::vector<double>& vec1, const std::vector<double>& vec2);
    void makeFilter1();
    double process(double input, double fc, double fs);

    QChart* drawSignal();
    QChart* drawFilter1();
    QChart* drawDftFilter1();
    QChart* drawDft();
    QChart* drawFiltered1();
    QChart* drawDftFiltered();
    QChart* drawNoise();
    QChart* drawFilter2();
    QChart* drawFiltered2();

private slots:
    void onTabChanged(int index);
    void onTabClosed(int index);
};
#endif // MAINWINDOW_H
