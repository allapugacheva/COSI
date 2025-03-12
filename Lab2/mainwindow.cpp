#include "mainwindow.h"
#include "./ui_mainwindow.h"

QChart* MainWindow::drawSinFunction() {

    QLineSeries *series = new QLineSeries();

    double x = 0;
    #pragma omp parallel for
    for (auto& signal : sinSignal) {
        series->append(x, signal);
        x += step;
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->legend()->hide();

    return chart;
}

QChart* MainWindow::drawCosFunction() {

    QLineSeries *series = new QLineSeries();

    double x = 0;
    #pragma omp parallel for
    for (auto& signal : cosSignal) {
        series->append(x, signal);
        x += step;
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->legend()->hide();

    return chart;
}

QChart* MainWindow::drawConvFunction() {

    QLineSeries *series = new QLineSeries();

    double x = 0;
    #pragma omp parallel for
    for (auto& signal : convSignal) {
        series->append(x, signal);
        x += step;
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->legend()->hide();

    return chart;
}

QChart* MainWindow::drawCorrFunction() {

    QLineSeries *series = new QLineSeries();

    double x = 0;
    #pragma omp parallel for
    for (auto& signal : corrSignal) {
        series->append(x, signal);
        x += step;
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->legend()->hide();

    return chart;
}

QChart* MainWindow::drawFurConvFunction() {

    QLineSeries *series = new QLineSeries();

    double x = 0;
    #pragma omp parallel for
    for (auto& signal : furConvSignal) {
        series->append(x, signal);
        x += step;
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->legend()->hide();

    return chart;
}

QChart* MainWindow::drawFurCorrFunction() {

    QLineSeries *series = new QLineSeries();

    double x = 0;
    #pragma omp parallel for
    for (auto& signal : furCorrSignal) {
        series->append(x, signal);
        x += step;
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->legend()->hide();

    return chart;
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), N(2000), T(10.0), step(T / N) {

    ui->setupUi(this);

    InteractiveChartView *chartView = new InteractiveChartView(nullptr);
    chartView->setRenderHint(QPainter::Antialiasing);
    QVBoxLayout *layout = new QVBoxLayout();
    QPushButton* button = new QPushButton("Сохранить");
    button->setFixedSize(100, 25);
    connect(button, &QPushButton::clicked, this, &MainWindow::saveSin);
    layout->addWidget(chartView);
    layout->addWidget(button);
    ui->sinTab->setLayout(layout);

    chartView = new InteractiveChartView(nullptr);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout = new QVBoxLayout();
    button = new QPushButton("Сохранить");
    button->setFixedSize(100, 25);
    connect(button, &QPushButton::clicked, this, &MainWindow::saveCos);
    layout->addWidget(chartView);
    layout->addWidget(button);
    ui->cosTab->setLayout(layout);

    chartView = new InteractiveChartView(nullptr);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout = new QVBoxLayout();
    button = new QPushButton("Сохранить");
    button->setFixedSize(100, 25);
    connect(button, &QPushButton::clicked, this, &MainWindow::saveConv);
    layout->addWidget(chartView);
    layout->addWidget(button);
    ui->convTab->setLayout(layout);

    chartView = new InteractiveChartView(nullptr);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout = new QVBoxLayout();
    layout->addWidget(chartView);
    ui->corrTab->setLayout(layout);

    chartView = new InteractiveChartView(nullptr);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout = new QVBoxLayout();
    button = new QPushButton("Сохранить");
    button->setFixedSize(100, 25);
    connect(button, &QPushButton::clicked, this, &MainWindow::saveFurConv);
    layout->addWidget(chartView);
    layout->addWidget(button);
    ui->furConvTab->setLayout(layout);

    chartView = new InteractiveChartView(nullptr);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout = new QVBoxLayout();
    layout->addWidget(chartView);
    ui->furCorrTab->setLayout(layout);

    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::onTabClosed);

    makeSignal();
    ui->tabWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dft() {

    #pragma omp parallel for
    for (int k = 0; k < N; k++) {
        std::complex<double> sumSin(0, 0), sumCos(0, 0);
        #pragma omp parallel for
        for (int n = 0; n < N; n++) {
            double angle = (-2.0 * M_PI * k * n) / N;
            sumSin += sinSignal[n] * std::complex<double>(cos(angle), sin(angle));
            sumCos += cosSignal[n] * std::complex<double>(cos(angle), sin(angle));
        }
        dftSin.push_back(sumSin);
        dftCos.push_back(sumCos);
    }
}

void MainWindow::makeSignal() {

    // Сигналы sin и cos
    double x = 0;
    while (x <= T) {
        sinSignal.push_back(qSin(7 * x));
        cosSignal.push_back(qCos(2 * x));
        x += step;
    }

    // Свертка
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        double temp = 0;
        #pragma omp parallel for
        for (int k = 0; k < N; k++)
            temp += sinSignal[k] * cosSignal[(i - k + N) % N];
        convSignal.push_back(temp);
    }

    // Корреляция
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        double temp = 0;
        #pragma omp parallel for
        for (int k = 0; k < N; k++)
            temp += sinSignal[k] * cosSignal[(i + k) % N];
        corrSignal.push_back(temp);
    }

    dft();

    // Свертка Фурье
    #pragma omp parallel for
    for (int n = 0; n < N; n++) {
        std::complex<double> sum(0, 0);
        #pragma omp parallel for
        for (int k = 0; k < N; k++) {
            double angle = (2.0 * M_PI * k * n) / N;
            sum += (dftSin[k] * dftCos[k]) * std::complex<double>(cos(angle), sin(angle));
        }
        furConvSignal.push_back(sum.real() / N);
    }

    // Корреляция Фурье
    #pragma omp parallel for
    for (int n = 0; n < N; n++) {
        std::complex<double> sum(0, 0);
        #pragma omp parallel for
        for (int k = 0; k < N; k++) {
            double angle = (2.0 * M_PI * k * n) / N;
            sum += (std::conj(dftSin[k]) * dftCos[k]) * std::complex<double>(cos(angle), sin(angle));
        }
        furCorrSignal.push_back(sum.real() / N);
    }
}

void MainWindow::saveSin() {

    saveWav("sin.wav", sinSignal, N / T);
}

void MainWindow::saveCos() {

    saveWav("cos.wav", cosSignal, N / T);
}

void MainWindow::saveConv() {

    saveWav("conv.wav", convSignal, N / T);
}

void MainWindow::saveFurConv() {

    saveWav("furConv.wav", furConvSignal, N / T);
}

void MainWindow::onTabChanged(int index)
{
    if (index < 0) return;

    QWidget* tab = ui->tabWidget->widget(index);
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(tab->layout());
    InteractiveChartView* chartView = qobject_cast<InteractiveChartView*>(layout->itemAt(0)->widget());

    if (index == 0)
        chartView->setChart(drawSinFunction());
    else if (index == 1)
        chartView->setChart(drawCosFunction());
    else if (index == 2)
        chartView->setChart(drawConvFunction());
    else if (index == 3)
        chartView->setChart(drawCorrFunction());
    else if (index == 4)
        chartView->setChart(drawFurConvFunction());
    else if (index == 5)
        chartView->setChart(drawFurCorrFunction());

    chartView->resetView();
}

void MainWindow::onTabClosed(int index)
{
    QWidget* tab = ui->tabWidget->widget(index);
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(tab->layout());
    InteractiveChartView* chartView = qobject_cast<InteractiveChartView*>(layout->itemAt(0)->widget());

    if (chartView) {
        delete chartView->chart();
        chartView->setChart(nullptr);
    }
}
