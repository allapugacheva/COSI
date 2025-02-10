#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QChart>
#include <QLineSeries>
#include <vector>
#include <complex>
#include <cmath>
#include <Eigen/Dense>
#include <unsupported/Eigen/FFT>
#include "interactivechartview.h"
#include <QPushButton>
#include <QLineEdit>
#include <QObject>
#include <QDoubleValidator>
#include "wavsaver.h"
#include <QTimer>

using namespace Eigen;

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

    std::vector<double> signal;
    std::vector<std::complex<double>> dft;
    std::vector<double> idft;

    void drawDefaultFunction();
    void drawDftFunction();
    void drawIdftFunction();
    void IDFT();
    void DFT();
    double getFunctionValue(double x);

private slots:
    void saveDefault();
    void saveIdft();
    void decreaseAmplitude();
    void makeZero();
    void returnDefaultDft();
};
#endif // MAINWINDOW_H
