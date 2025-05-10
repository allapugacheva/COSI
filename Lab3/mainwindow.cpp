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

QChart* MainWindow::drawDftSignal() {

    QPen linePen(Qt::green);
    linePen.setWidth(2);

    QChart *chart = new QChart();

    int x = 0;
    #pragma omp parallel for
    for (int i = 0; i < dftSignal.size(); i++) {
        QLineSeries *line = new QLineSeries();
        line->append(i, 0);
        line->append(i, std::abs(dftSignal[i]));
        line->setPen(linePen);
        chart->addSeries(line);
    }

    chart->createDefaultAxes();
    chart->legend()->hide();

    return chart;
}

QChart* MainWindow::drawNoiseSignal() {

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

QChart* MainWindow::drawL(int l) {

    QChart *chart = new QChart();
    QPen linePen(Qt::red);
    linePen.setWidth(2);

    QLineSeries *series = new QLineSeries();
    series->setPen(linePen);

    double x = 0;
    #pragma omp parallel for
    for (auto& s : aL[l]) {
        series->append(x, s);
        x += step;
    }
    chart->addSeries(series);

    linePen.setColor(Qt::blue);
    series = new QLineSeries();
    series->setPen(linePen);

    x = 0;
    #pragma omp parallel for
    for (auto& s : dL[l]) {
        series->append(x, s);
        x += step;
    }
    chart->addSeries(series);

    chart->createDefaultAxes();
    chart->legend()->hide();

    return chart;
}

QChart* MainWindow::drawIL(int l) {

    QChart *chart = new QChart();
    QPen linePen(Qt::red);
    linePen.setWidth(2);

    QLineSeries *series = new QLineSeries();
    series->setPen(linePen);

    double x = 0;
    #pragma omp parallel for
    for (auto& s : iaL[2 - l]) {
        series->append(x, s);
        x += step;
    }
    chart->addSeries(series);

    linePen.setColor(Qt::blue);
    series = new QLineSeries();
    series->setPen(linePen);

    x = 0;
    #pragma omp parallel for
    for (auto& s : idL[l - 1]) {
        series->append(x, s);
        x += step;
    }
    chart->addSeries(series);

    chart->createDefaultAxes();
    chart->legend()->hide();

    return chart;
}

QChart* MainWindow::drawISignal() {

    QLineSeries *series = new QLineSeries();

    double x = 0;
    #pragma omp parallel for
    for (auto& s : iaL[2]) {
        series->append(x, s);
        x += step;
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->legend()->hide();

    return chart;
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), N(2048), T(10.0), step(T / N) {

    ui->setupUi(this);

    InteractiveChartView *chartView = new InteractiveChartView(nullptr);
    chartView->setRenderHint(QPainter::Antialiasing);
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(chartView);
    ui->signalTab->setLayout(layout);

    chartView = new InteractiveChartView(nullptr);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout = new QVBoxLayout();
    layout->addWidget(chartView);
    ui->dftTab->setLayout(layout);

    chartView = new InteractiveChartView(nullptr);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout = new QVBoxLayout();
    layout->addWidget(chartView);
    ui->noiseTab->setLayout(layout);

    chartView = new InteractiveChartView(nullptr);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout = new QVBoxLayout();
    layout->addWidget(chartView);
    ui->l1Tab->setLayout(layout);

    chartView = new InteractiveChartView(nullptr);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout = new QVBoxLayout();
    layout->addWidget(chartView);
    ui->l2Tab->setLayout(layout);

    chartView = new InteractiveChartView(nullptr);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout = new QVBoxLayout();
    layout->addWidget(chartView);
    ui->l3Tab->setLayout(layout);

    chartView = new InteractiveChartView(nullptr);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout = new QVBoxLayout();
    layout->addWidget(chartView);
    ui->il2Tab->setLayout(layout);

    chartView = new InteractiveChartView(nullptr);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout = new QVBoxLayout();
    layout->addWidget(chartView);
    ui->il1Tab->setLayout(layout);

    chartView = new InteractiveChartView(nullptr);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout = new QVBoxLayout();
    layout->addWidget(chartView);
    ui->iSignalTab->setLayout(layout);

    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::onTabClosed);

    ui->l1ComboBox->addItem("L1: Ничего");
    ui->l1ComboBox->addItem("L1: Обнулить");
    ui->l1ComboBox->addItem("L1: Уменьшить в 2 раза");
    ui->l1ComboBox->addItem("L1: Увеличить в 2 раза");

    ui->l2ComboBox->addItem("L2: Ничего");
    ui->l2ComboBox->addItem("L2: Обнулить");
    ui->l2ComboBox->addItem("L2: Уменьшить в 2 раза");
    ui->l2ComboBox->addItem("L2: Увеличить в 2 раза");

    ui->l3ComboBox->addItem("L3: Ничего");
    ui->l3ComboBox->addItem("L3: Обнулить");
    ui->l3ComboBox->addItem("L3: Уменьшить в 2 раза");
    ui->l3ComboBox->addItem("L3: Увеличить в 2 раза");

    connect(ui->l1ComboBox, &QComboBox::currentIndexChanged, this, &MainWindow::onL1ComboBoxChanged);
    connect(ui->l2ComboBox, &QComboBox::currentIndexChanged, this, &MainWindow::onL2ComboBoxChanged);
    connect(ui->l3ComboBox, &QComboBox::currentIndexChanged, this, &MainWindow::onL3ComboBoxChanged);

    makeSignal();
    ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::onL1ComboBoxChanged(int index) {

    if (index == 0)
        idL[0] = dL[0];
    else if (index == 1)
        std::fill(idL[0].begin(), idL[0].end(), 0);
    else if (index == 2) {
        idL[0] = dL[0];
        for (auto& val : idL[0])
            val /= 2;
    }
    else {
        idL[0] = dL[0];
        for (auto& val : idL[0])
            val *= 2;
    }

    refresh();
}

void MainWindow::onL2ComboBoxChanged(int index) {

    if (index == 0)
        idL[1] = dL[1];
    else if (index == 1)
        std::fill(idL[1].begin(), idL[1].end(), 0);
    else if (index == 2) {
        idL[1] = dL[1];
        for (auto& val : idL[1])
            val /= 2;
    }
    else {
        idL[1] = dL[1];
        for (auto& val : idL[1])
            val *= 2;
    }

    refresh();
}

void MainWindow::onL3ComboBoxChanged(int index) {

    if (index == 0)
        idL[2] = dL[2];
    else if (index == 1)
        std::fill(idL[2].begin(), idL[2].end(), 0);
    else if (index == 2) {
        idL[2] = dL[2];
        for (auto& val : idL[2])
            val /= 2;
    }
    else {
        idL[2] = dL[2];
        for (auto& val : idL[2])
            val *= 2;
    }

    refresh();
}

void MainWindow::refresh() {

    // iL2
    idwt(aL[2], idL[2], g, h, iaL[0]);
    iaL[0].resize(aL[1].size());

    // iL1
    idwt(iaL[0], idL[1], g, h, iaL[1]);
    iaL[1].resize(aL[0].size());

    // signal
    idwt(iaL[1], idL[0], g, h, iaL[2]);
    iaL[2].resize(signal.size());

    onTabChanged(ui->tabWidget->currentIndex());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dft() {

    #pragma omp parallel for
    for (int k = 0; k < N; k++) {
        std::complex<double> sum(0, 0);
        #pragma omp parallel for
        for (int n = 0; n < N; n++) {
            double angle = (-2.0 * M_PI * k * n) / N;
            sum += signal[n] * std::complex<double>(cos(angle), sin(angle));
        }
        dftSignal.push_back(sum);
    }
}

void MainWindow::conv(const std::vector<double> s, const std::vector<double> f1, const std::vector<double> f2, std::vector<double>& a, std::vector<double>& d) {

    a.clear();
    d.clear();

    #pragma omp parallel for
    for (size_t n = 0; n < s.size() + f1.size() - 1; n += 2) {
        double sumA = 0, sumD = 0;
        #pragma omp parallel for
        for (size_t k = 0; k < s.size(); k++)
            if (n - k < f1.size()) {
                sumA += s[k] * f1[n - k];
                sumD += s[k] * f2[n - k];
            }
        a.push_back(sumA);
        d.push_back(sumD);
    }
}

void MainWindow::idwt(const std::vector<double> a, const std::vector<double> d, const std::vector<double> f1, const std::vector<double> f2, std::vector<double>& res) {

    res.clear();
    res.resize(a.size() * 2);

    #pragma omp parallel for
    for (int i = 0; i < a.size(); i++)
    #pragma omp parallel for
        for (int j = 0; j < f1.size(); j++)
            res[(2 * i - j) % (2 * a.size())] += a[i] * f1[j] + d[i] * f2[j];
}

void MainWindow::makeSignal() {

    // Исходный сигнал
    double x = 0;
    while (x <= T) {
        signal.push_back(qCos(14 * x));
        x += step;
    }

    dft();

    // Наложение шума
    #pragma omp parallel for
    for (int i = 0; i < signal.size(); i++)
        noiseSignal.push_back(signal[i] + QRandomGenerator::global()->generateDouble() * 2 - 1);

    // L1
    conv(noiseSignal, g, h, aL[0], dL[0]);
    idL[0] = dL[0];

    // L2
    conv(aL[0], g, h, aL[1], dL[1]);
    idL[1] = dL[1];

    // L3
    conv(aL[1], g, h, aL[2], dL[2]);
    idL[2] = dL[2];

    // iL2
    idwt(aL[2], idL[2], g, h, iaL[0]);
    iaL[0].resize(aL[1].size());

    // iL1
    idwt(iaL[0], idL[1], g, h, iaL[1]);
    iaL[1].resize(aL[0].size());

    // signal
    idwt(iaL[1], idL[0], g, h, iaL[2]);
    iaL[2].resize(signal.size());
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
        chartView->setChart(drawDftSignal());
    else if (index == 2)
        chartView->setChart(drawNoiseSignal());
    else if (index == 3)
        chartView->setChart(drawL(0));
    else if (index == 4)
        chartView->setChart(drawL(1));
    else if (index == 5)
        chartView->setChart(drawL(2));
    else if (index == 6)
        chartView->setChart(drawIL(2));
    else if (index == 7)
        chartView->setChart(drawIL(1));
    else if (index == 8)
        chartView->setChart(drawISignal());

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
