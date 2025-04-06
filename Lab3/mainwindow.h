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
#include <QComboBox>

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

    std::vector<std::complex<double>> dftSignal;

    std::vector<double> noiseSignal;

    std::vector<double> aL[3], dL[3];
    std::vector<double> iaL[3], idL[3];

    const std::vector<double> h = {
        0.3326705529500826,
        0.8068915093110928,
        0.4598775021184915,
        -0.1350110200102546,
        -0.0854412738822415,
        0.0352262918857095
    };

    const std::vector<double> g = {
        -0.0352262918857095,
        -0.0854412738822415,
        0.1350110200102546,
        0.4598775021184915,
        -0.8068915093110928,
        0.3326705529500826
    };

    void makeSignal();
    void dft();
    void conv(const std::vector<double> s, const std::vector<double> f1, const std::vector<double> f2, std::vector<double>& a, std::vector<double>& d);
    void idwt(const std::vector<double> a, const std::vector<double> b, const std::vector<double> f1, const std::vector<double> f2, std::vector<double>& res);
    void refresh();

    QChart* drawSignal();
    QChart* drawDftSignal();
    QChart* drawNoiseSignal();
    QChart* drawL(int l);
    QChart* drawIL(int l);
    QChart* drawISignal();

private slots:
    void onTabChanged(int index);
    void onTabClosed(int index);
    void onL1ComboBoxChanged(int index);
    void onL2ComboBoxChanged(int index);
    void onL3ComboBoxChanged(int index);
};
#endif // MAINWINDOW_H
