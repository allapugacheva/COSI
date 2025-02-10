#include "mainwindow.h"
#include "./ui_mainwindow.h"

double MainWindow::getFunctionValue(double x) {

    return qSin(14 * x) + 2 * qCos(69 * x);
}

void MainWindow::DFT() {

    dft.clear();

    for (int k = 0; k < N; ++k) {
        std::complex<double> sum(0, 0);
        for (int n = 0; n < N; ++n) {
            double angle = -2.0 * M_PI * k * n / N;
            sum += signal[n] * std::complex<double>(cos(angle), sin(angle));
        }
        dft.push_back(sum);
    }
}

void MainWindow::IDFT() {

    idft.clear();

    for (int n = 0; n < N; ++n) {
        std::complex<double> sum(0, 0);
        for (int k = 0; k < N; ++k) {
            double angle = 2.0 * M_PI * k * n / N;
            sum += dft[k] * std::complex<double>(cos(angle), sin(angle));
        }
        idft.push_back(sum.real() / N);
    }
}

void MainWindow::drawDefaultFunction() {

    QLineSeries *series = new QLineSeries();

    double x = 0;
    for (int i = 0; i < N; i++, x += step)
        series->append(x, signal[i]);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->legend()->hide();

    QWidget* tab = ui->tabWidget->widget(0);
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(tab->layout());
    InteractiveChartView* chartView = qobject_cast<InteractiveChartView*>(layout->itemAt(0)->widget());
    chartView->setChart(chart);
    chartView->resetView();
}

void MainWindow::drawDftFunction() {

    QPen linePen(Qt::green);
    linePen.setWidth(2);

    QChart* chart = new QChart();
    for (int k = 0; k < N; ++k) {
        if (std::abs(dft[k]) == 0)
            continue;

        QLineSeries* line = new QLineSeries();
        line->append((double)k / T, 0);
        line->append((double)k / T, std::abs(dft[k]));
        line->setPen(linePen);
        chart->addSeries(line);
    }

    chart->createDefaultAxes();
    chart->legend()->hide();

    QWidget* tab = ui->tabWidget->widget(1);
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(tab->layout());
    InteractiveChartView* chartView = qobject_cast<InteractiveChartView*>(layout->itemAt(0)->widget());
    chartView->setChart(chart);
    chartView->resetView();
}

void MainWindow::drawIdftFunction() {

    QLineSeries* series = new QLineSeries();
    double x = 0;
    for (int i = 0; i < N; i++, x += step)
        series->append(x, idft[i]);

    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->legend()->hide();

    QWidget* tab = ui->tabWidget->widget(2);
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(tab->layout());
    InteractiveChartView* chartView = qobject_cast<InteractiveChartView*>(layout->itemAt(0)->widget());
    chartView->setChart(chart);
    chartView->resetView();
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), N(2600), T(1.0), step(T / N) {

    ui->setupUi(this);

    ui->tabWidget->setTabText(0, "Default");
    ui->tabWidget->setTabText(1, "DFT");
    ui->tabWidget->setTabText(2, "IDFT");

    InteractiveChartView *chartView = new InteractiveChartView(nullptr);
    chartView->setRenderHint(QPainter::Antialiasing);
    QVBoxLayout *layout = new QVBoxLayout();
    QPushButton* button = new QPushButton("Сохранить");
    button->setFixedSize(100, 25);
    connect(button, &QPushButton::clicked, this, &MainWindow::saveDefault);
    layout->addWidget(chartView);
    layout->addWidget(button);
    ui->defaultTab->setLayout(layout);

    chartView = new InteractiveChartView(nullptr);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout = new QVBoxLayout();
    QHBoxLayout* hLayout = new QHBoxLayout();
    button = new QPushButton("Уменьшить амплитуду");
    button->setFixedSize(150, 25);
    connect(button, &QPushButton::clicked, this, &MainWindow::decreaseAmplitude);
    hLayout->addWidget(button);
    QLineEdit* line = new QLineEdit();
    hLayout->addWidget(line);
    button = new QPushButton("Обнулить до");
    button->setFixedSize(150, 25);
    connect(button, &QPushButton::clicked, this, &MainWindow::makeZero);
    hLayout->addWidget(button);
    line = new QLineEdit();
    hLayout->addWidget(line);
    button = new QPushButton("Восстановить");
    button->setFixedSize(150, 25);
    connect(button, &QPushButton::clicked, this, &MainWindow::returnDefaultDft);
    hLayout->addWidget(button);
    layout->addWidget(chartView);
    layout->addLayout(hLayout);
    ui->dftTab->setLayout(layout);

    chartView = new InteractiveChartView(nullptr);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout = new QVBoxLayout();
    button = new QPushButton("Сохранить");
    button->setFixedSize(100, 25);
    connect(button, &QPushButton::clicked, this, &MainWindow::saveIdft);
    layout->addWidget(chartView);
    layout->addWidget(button);
    ui->idftTab->setLayout(layout);

    double x = 0;
    while (x <= T) {
        double val = getFunctionValue(x);
        signal.push_back(val);
        x += step;
    }

    drawDefaultFunction();

    DFT();
    drawDftFunction();

    IDFT();
    drawIdftFunction();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::saveDefault() {

    saveWav("outputDefault.wav", signal);
}

void MainWindow::saveIdft() {

    saveWav("outputIdft.wav", idft);
}

void MainWindow::decreaseAmplitude() {

    QWidget* tab = ui->tabWidget->widget(1);
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(tab->layout());
    QHBoxLayout* hLayout = qobject_cast<QHBoxLayout*>(layout->itemAt(1)->layout());
    QLineEdit* line = qobject_cast<QLineEdit*>(hLayout->itemAt(1)->widget());

    if (!line->text().isEmpty()) {
        double value = line->text().toDouble();
        if (value < 0)
            return;

        for (auto& num : dft)
            num /= value;

        drawDftFunction();
        IDFT();
        drawIdftFunction();
    }
    line->clear();
}

void MainWindow::makeZero() {

    QWidget* tab = ui->tabWidget->widget(1);
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(tab->layout());
    QHBoxLayout* hLayout = qobject_cast<QHBoxLayout*>(layout->itemAt(1)->layout());
    QLineEdit* line = qobject_cast<QLineEdit*>(hLayout->itemAt(3)->widget());

    if (!line->text().isEmpty()) {
        double value = line->text().toDouble();
        if (value < 0 || value > N / (2.0 * T))
            return;

        std::fill(dft.begin(), dft.begin() + value, std::complex<double>());
        std::fill(dft.end() - value, dft.end(), std::complex<double>());

        drawDftFunction();
        IDFT();
        drawIdftFunction();
    }
    line->clear();
}

void MainWindow::returnDefaultDft() {

    DFT();
    drawDftFunction();

    IDFT();
    drawIdftFunction();
}
