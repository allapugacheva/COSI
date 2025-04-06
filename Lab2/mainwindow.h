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
#include "wavsaver.h"
#include <QTimer>
#include <QVBoxLayout>
#include <omp.h>
#include <algorithm>

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

    std::vector<double> sinSignal;
    std::vector<double> cosSignal;

    std::vector<double> convSignal;
    std::vector<double> corrSignal;

    std::vector<std::complex<double>> dftSin;
    std::vector<std::complex<double>> dftCos;

    std::vector<double> furConvSignal;
    std::vector<double> furCorrSignal;

    void makeSignal();
    void dft();

    QChart* drawSinFunction();
    QChart* drawCosFunction();
    QChart* drawConvFunction();
    QChart* drawCorrFunction();
    QChart* drawFurConvFunction();
    QChart* drawFurCorrFunction();

private slots:
    void saveSin();
    void saveCos();
    void saveConv();
    void saveFurConv();
    void onTabChanged(int index);
    void onTabClosed(int index);
};
#endif // MAINWINDOW_H
