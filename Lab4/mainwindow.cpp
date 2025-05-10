#include "mainwindow.h"
#include "./ui_mainwindow.h"

QChart* MainWindow::drawSignal() {

    QLineSeries *series = new QLineSeries();

    double x = 0;
#pragma omp parallel for
    for (auto& s : signal) {
        series->append(x, s);
        x += step;
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->legend()->hide();

    return chart;
}

QChart* MainWindow::drawFilter1() {

    QLineSeries *series = new QLineSeries();

#pragma omp parallel for
    for (int i = 0; i < filter1.size(); i++)
        series->append(i, filter1[i]);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->legend()->hide();

    return chart;
}

QChart* MainWindow::drawDftFilter1() {

    QPen linePen(Qt::green);
    linePen.setWidth(2);

    QChart *chart = new QChart();

#pragma omp parallel for
    for (int i = 0; i < dftFilter1.size(); i++) {
        QLineSeries *line = new QLineSeries();
        line->append(i, 0);
        line->append(i, dftFilter1[i]);
        line->setPen(linePen);
        chart->addSeries(line);
    }

    chart->createDefaultAxes();
    chart->legend()->hide();

    return chart;
}

QChart* MainWindow::drawDft() {

    QPen linePen(Qt::green);
    linePen.setWidth(2);

    QChart *chart = new QChart();

#pragma omp parallel for
    for (int i = 0; i < dftSignal.size(); i++) {
        QLineSeries *line = new QLineSeries();
        line->append(i, 0);
        line->append(i, dftSignal[i]);
        line->setPen(linePen);
        chart->addSeries(line);
    }

    chart->createDefaultAxes();
    chart->legend()->hide();

    return chart;
}

QChart* MainWindow::drawFiltered1() {

    QLineSeries *series = new QLineSeries();

    double x = 0;
#pragma omp parallel for
    for (auto& s : filtered1) {
        series->append(x, s);
        x += step;
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->legend()->hide();

    return chart;
}

QChart* MainWindow::drawDftFiltered() {

    QPen linePen(Qt::green);
    linePen.setWidth(2);

    QChart *chart = new QChart();

#pragma omp parallel for
    for (int i = 0; i < dftFilteredSignal.size(); i++) {
        QLineSeries *line = new QLineSeries();
        line->append(i, 0);
        line->append(i, dftFilteredSignal[i]);
        line->setPen(linePen);
        chart->addSeries(line);
    }

    chart->createDefaultAxes();
    chart->legend()->hide();

    return chart;
}

QChart* MainWindow::drawNoise() {

    QLineSeries *series = new QLineSeries();

    double x = 0;
#pragma omp parallel for
    for (auto& s : noiseSignal) {
        series->append(x, s);
        x += step;
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->legend()->hide();

    return chart;
}

QChart* MainWindow::drawFiltered2() {

    QLineSeries *series = new QLineSeries();

    double x = 0;
#pragma omp parallel for
    for (auto& s : filtered2) {
        series->append(x, s);
        x += step;
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->legend()->hide();

    return chart;
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), N(2000), T(1.0), step(T / N), prev_out(0.0) {

    ui->setupUi(this);

    InteractiveChartView *chartView = new InteractiveChartView(nullptr);
    chartView->setRenderHint(QPainter::Antialiasing);
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(chartView);
    ui->funTab->setLayout(layout);

    chartView = new InteractiveChartView(nullptr);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout = new QVBoxLayout();
    layout->addWidget(chartView);
    ui->filter1Tab->setLayout(layout);

    chartView = new InteractiveChartView(nullptr);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout = new QVBoxLayout();
    layout->addWidget(chartView);
    ui->dftFilter1Tab->setLayout(layout);

    chartView = new InteractiveChartView(nullptr);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout = new QVBoxLayout();
    layout->addWidget(chartView);
    ui->dftTab->setLayout(layout);

    chartView = new InteractiveChartView(nullptr);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout = new QVBoxLayout();
    layout->addWidget(chartView);
    ui->filtered1Tab->setLayout(layout);

    chartView = new InteractiveChartView(nullptr);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout = new QVBoxLayout();
    layout->addWidget(chartView);
    ui->dftFTab->setLayout(layout);

    chartView = new InteractiveChartView(nullptr);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout = new QVBoxLayout();
    layout->addWidget(chartView);
    ui->noiseTab->setLayout(layout);

    chartView = new InteractiveChartView(nullptr);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout = new QVBoxLayout();
    layout->addWidget(chartView);
    ui->filtered2Tab->setLayout(layout);

    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::onTabClosed);

    makeSignal();
    ui->tabWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

std::vector<double> MainWindow::dft(const std::vector<double>& vec) {

    std::vector<double> result;

#pragma omp parallel for
    for (int k = 0; k < vec.size(); k++) {
        std::complex<double> sum(0, 0);
#pragma omp parallel for
        for (int n = 0; n < vec.size(); n++) {
            double angle = (-2.0 * M_PI * k * n) / vec.size();
            sum += vec[n] * std::complex<double>(cos(angle), sin(angle));
        }
        result.push_back(std::abs(sum));
    }

    return result;
}

std::vector<double> MainWindow::conv(const std::vector<double>& vec1, const std::vector<double>& vec2) {

    std::vector<double> result;

#pragma omp parallel for
    for (size_t n = 0; n < vec1.size() + vec2.size() - 1; n ++) {
        double sum = 0;
#pragma omp parallel for
        for (size_t k = 0; k < vec1.size(); k++)
            if (n - k < vec2.size())
                sum += vec1[k] * vec2[n - k];
        result.push_back(sum);
    }

    return result;
}

void compute_low_pass(std::vector<double>& a, double fc, int M) {

    for (int i = 0; i <= M; i++) {
        if (i == M / 2)
            a.push_back(2 * M_PI * fc);
        else
            a.push_back(sin(2 * M_PI * fc * (i - M / 2)) / (i - M / 2));

        a[i] *= 0.42 - 0.5 * cos (2 * M_PI * i / M) + 0.08 * cos(4 * M_PI * i / M);
    }

    double sum = 0;
    for (double i : a) sum += i;
    for (double& i : a) i /= sum;
}

void invert_filter(std::vector<double>& b, int M) {
    for (int i = 0; i <= M; i++)
        b[i] = -b[i];

    b[M / 2] += 1;
}

void MainWindow::makeFilter1() {

    const double fs = N / T; // частоты дискретизации
    const double f1 = 50.0;  // нижняя граница полосы
    const double f2 = 100.0; // верхняя граница полосы

    double f1_norm = f1 / fs;
    double f2_norm = f2 / fs;

    int _N = 6.64 / (f2_norm - f1_norm);   // длина фильтра
    if (_N % 2 == 0)
        _N += 1;

    const double M = _N - 1;

    std::vector<double> a, b;

    compute_low_pass(a, f1_norm, M);
    compute_low_pass(b, f2_norm, M);
    invert_filter(b, M);

    for (int i = 0; i <= M; i++)
        filter1.push_back(a[i] + b[i]);

    invert_filter(filter1, M);
}

double MainWindow::process(double input, double fc, double fs) {

    double omega = 2.0 * M_PI * fc;
    double alpha = omega / (omega + fs);
    double output = alpha * input + (1.0 - alpha) * prev_out;
    prev_out = output;
    return output;
}

void MainWindow::makeSignal() {

    double x = 0;
    while (x <= T) {
        signal.push_back(5 * cos(40 * x) + 3 * sin(123 * x));
        x += step;
    }

    makeFilter1();
    dftFilter1 = dft(filter1);
    dftSignal = dft(signal);
    filtered1 = conv(signal, filter1);
    dftFilteredSignal = dft(filtered1);

#pragma omp parallel for
    for (int i = 0; i < signal.size(); i++)
        noiseSignal.push_back(signal[i] + QRandomGenerator::global()->generateDouble() * 2 - 1);

    double fs = N / T, fc = 100.0;

#pragma omp parallel for
    for (auto& s : noiseSignal)
        filtered2.push_back(process(process(process(process(s, fc, fs), fc, fs), fc, fs), fc, fs));
}

void MainWindow::onTabChanged(int index)
{
    if (index < 0) return;

    QWidget* tab = ui->tabWidget->widget(index);
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(tab->layout());
    InteractiveChartView* chartView = qobject_cast<InteractiveChartView*>(layout->itemAt(0)->widget());

    if (index == 0)
        chartView->setChart(drawSignal());
    else if (index == 1)
        chartView->setChart(drawFilter1());
    else if (index == 2)
        chartView->setChart(drawDftFilter1());
    else if (index == 3)
        chartView->setChart(drawDft());
    else if (index == 4)
        chartView->setChart(drawFiltered1());
    else if (index == 5)
        chartView->setChart(drawDftFiltered());
    else if (index == 6)
        chartView->setChart(drawNoise());
    else if (index == 7)
        chartView->setChart(drawFiltered2());

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
