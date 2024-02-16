#include "qlineseries.h"
#include <mainwindow.h>
#include <ui_mainwindow.h>

#include <QChart>
#include <QValueAxis>
#include <QVector>

namespace Ui {
class MainWindow;
}

class MainWindowPrivate
{
public:
    MainWindowPrivate()
        : ui()
    {}

public:
    Ui::MainWindow ui;

    QtCharts::QChart chart;

    QVector<double> x, y;

public:
    void drawChart();
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_p(std::make_unique<MainWindowPrivate>())

{
    m_p->ui.setupUi(this);
    m_p->ui.graphicsView->setChart(&m_p->chart);
    m_p->drawChart();
}

MainWindow::~MainWindow() = default;

void MainWindowPrivate::drawChart()
{
    chart.setTitle("График");
    //    y = l*x + b;
    int k = 4;
    QtCharts::QLineSeries *serires1 = new QtCharts::QLineSeries();
    for (int i = 0; i < 200; ++i) {
        serires1->append(i, k * i / 2);
    }

    chart.addSeries(serires1);

    QtCharts::QValueAxis *axisX = new QtCharts::QValueAxis;
    axisX->setRange(0, 15);
    QtCharts::QValueAxis *axisY = new QtCharts::QValueAxis;
    axisY->setRange(0, 15);
    chart.setAxisX(axisX, serires1);
    chart.setAxisY(axisY, serires1);
}
