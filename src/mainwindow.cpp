#include <generateWorker.h>
#include <mainwindow.h>
#include <ui_mainwindow.h>

#include <QChart>
#include <QDebug>
//#include <QLineSeries>
#include <QRandomGenerator>
#include <QScatterSeries>
#include <QSplineSeries>
#include <QValueAxis>
#include <QVector>

#include <QThread>

QT_CHARTS_USE_NAMESPACE

void initResources()
{
    Q_INIT_RESOURCE(resourses);
}

void deinitResources()
{
    Q_CLEANUP_RESOURCE(resourses);
}

namespace {
constexpr int ICONS_SIZE = 65;
} // namespace

// класс генератора точек в отдельном потоке
/* ----------------------------- GenerateDataWorker ------------------------ */

/* ----------------------------- MainWindowPrivate ------------------------- */
class MainWindowPrivate
{
public:
    explicit MainWindowPrivate()
        : ui()
        //        , serires1(new QScatterSeries)
        , serires1(new QSplineSeries)
        , thread(nullptr)
        , generator(new GenerateDataWorker)
    {
       
    }

    ~MainWindowPrivate() = default;

    Ui::MainWindow ui;
    QChart chart;
    //    QScatterSeries *serires1;
    QSplineSeries *serires1;
    QVector<double> x, y;

    // поток для генератора
    std::unique_ptr<QThread> thread;
    GenerateDataWorker *generator;
};

/* --------------------------------- MainWindow ---------------------------- */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_p(std::make_unique<MainWindowPrivate>())

{
    setupUi();
    createConnections();
}

MainWindow::~MainWindow()
{
    deinitResources();
    // удаление потока
    deleteThread();
}

void MainWindow::drawChart(double x, double y)
{
    m_p->serires1->append(x, y);
}

void MainWindow::setupUi()
{
    initResources();
    m_p->ui.setupUi(this);

    // настройка графика
    m_p->ui.graphicsView->setChart(&m_p->chart);
    m_p->chart.setTitle(tr("График"));
    m_p->chart.addSeries(m_p->serires1);
    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, 15);
    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0, 15);
    m_p->chart.setAxisX(axisX, m_p->serires1);
    m_p->chart.setAxisY(axisY, m_p->serires1);

    // настройка кнопок
    m_p->ui.playButton->setIcon(QIcon(":/icons/play"));
    m_p->ui.playButton->setIconSize(QSize(ICONS_SIZE, ICONS_SIZE));
    m_p->ui.playButton->setToolTip(tr("Старт генерации значений"));
    m_p->ui.playButton->setCheckable(true);
    //    m_p->ui.playButton->setStyleSheet("background:transparent;");

    m_p->ui.pauseButton->setIcon(QIcon(":/icons/pause"));
    m_p->ui.pauseButton->setIconSize(QSize(ICONS_SIZE, ICONS_SIZE));
    m_p->ui.pauseButton->setToolTip(tr("Пауза генерации"));

    m_p->ui.stopButton->setIcon(QIcon(":/icons/stop"));
    m_p->ui.stopButton->setIconSize(QSize(ICONS_SIZE, ICONS_SIZE));
    m_p->ui.stopButton->setToolTip(tr("Остановить генерацию и очистить график"));
}

void MainWindow::setupThread()
{
    if (nullptr == m_p->thread) {
        return;
    }
    m_p->thread = std::make_unique<QThread>();
    m_p->thread.get()->setObjectName("GenerateDataWorkerThread");
    m_p->generator->moveToThread(m_p->thread.get());
    connect(m_p->generator, &GenerateDataWorker::sendGeneratedData, this, &MainWindow::drawChart);
    m_p->thread->start();
}

void MainWindow::createConnections()
{
    connect(m_p->ui.pauseButton, &QPushButton::clicked, this, []() { qDebug() << "pause clicked"; });
    connect(m_p->ui.stopButton, &QPushButton::clicked, this, []() { qDebug() << "stop clicked"; });

    connect(m_p->ui.playButton, &QPushButton::clicked, m_p->generator, [this](bool state) {
        setupThread();
        m_p->generator->generateData();
    });

    connect(m_p->ui.playButton,
            &QPushButton::clicked,
            m_p->generator,
            &GenerateDataWorker::onPlayStateChaged);

    connect(m_p->ui.stopButton, &QPushButton::clicked, [this]() {
        m_p->generator->resetData();
        m_p->serires1->clear();
        deleteThread();
    });
}

void MainWindow::deleteThread()
{
    if (m_p->thread != nullptr) {
        m_p->thread->quit();
        m_p->thread->wait();
        m_p->thread.reset();
    }
}
