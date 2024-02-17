#include <mainwindow.h>
#include <ui_mainwindow.h>

#include <QChart>
#include <QDebug>
#include <QLineSeries>
#include <QRandomGenerator>
#include <QValueAxis>
#include <QVector>

void InitResources()
{
    Q_INIT_RESOURCE(resourses);
}

void DeInitResources()
{
    Q_CLEANUP_RESOURCE(resourses);
}

namespace {
constexpr int ICONS_SIZE = 65;
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
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_p(std::make_unique<MainWindowPrivate>())

{
    setupUi();
    createConnections();
    drawChart();
}

MainWindow::~MainWindow()
{
    DeInitResources();
}

void MainWindow::drawChart()
{
    m_p->chart.setTitle("График");
    //    y = l*x + b;
    int k = 4;
    QtCharts::QLineSeries *serires1 = new QtCharts::QLineSeries();
    for (int i = 0; i < 200; ++i) {
        serires1->append(i, QRandomGenerator::global()->bounded(1, 4));
    }

    m_p->chart.addSeries(serires1);

    QtCharts::QValueAxis *axisX = new QtCharts::QValueAxis;
    axisX->setRange(0, 15);
    QtCharts::QValueAxis *axisY = new QtCharts::QValueAxis;
    axisY->setRange(0, 15);
    m_p->chart.setAxisX(axisX, serires1);
    m_p->chart.setAxisY(axisY, serires1);
}

void MainWindow::setupUi()
{
    InitResources();
    m_p->ui.setupUi(this);
    m_p->ui.graphicsView->setChart(&m_p->chart);

    m_p->ui.playButton->setIcon(QIcon(":/icons/play"));
    m_p->ui.playButton->setIconSize(QSize(ICONS_SIZE, ICONS_SIZE));
    m_p->ui.playButton->setToolTip(tr("Старт генерации значений"));
    //    m_p->ui.playButton->setStyleSheet("background:transparent;");

    m_p->ui.pauseButton->setIcon(QIcon(":/icons/pause"));
    m_p->ui.pauseButton->setIconSize(QSize(ICONS_SIZE, ICONS_SIZE));
    m_p->ui.pauseButton->setToolTip(tr("Пауза генерации"));

    m_p->ui.stopButton->setIcon(QIcon(":/icons/stop"));
    m_p->ui.stopButton->setIconSize(QSize(ICONS_SIZE, ICONS_SIZE));
    m_p->ui.stopButton->setToolTip(tr("Остановить генерацию и очистить график"));
}

void MainWindow::createConnections()
{
    connect(m_p->ui.playButton, &QPushButton::clicked, this, []() { qDebug() << "start clicked"; });
    connect(m_p->ui.pauseButton, &QPushButton::clicked, this, []() { qDebug() << "pause clicked"; });
    connect(m_p->ui.stopButton, &QPushButton::clicked, this, []() { qDebug() << "stop clicked"; });
}
