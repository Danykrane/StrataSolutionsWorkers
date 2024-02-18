#include <generateWorker.h>
#include <mainwindow.h>
#include <ui_mainwindow.h>

#include <QChart>
#include <QDebug>

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

const int MIN_AXIS_VALUE = 0;
const int MAX_AXIS_VALUE = 15;

const int STEP_INC_AXIS = 10;
} // namespace

/* ----------------------------- MainWindowPrivate ------------------------- */
/*!
 * \brief Приватный класс с данными окна
 */
class MainWindowPrivate
{
public:
    /*!
     * \brief Конструктор приватного класса
     */
    explicit MainWindowPrivate()
        : ui()
        , serires1(new QSplineSeries)
        , axisX(new QValueAxis)
        , axisY(new QValueAxis)
        , generator(std::make_unique<GenerateDataWorker>())
    {}

    ~MainWindowPrivate() = default;

    /*!
     * \brief Компонента UI
     */
    Ui::MainWindow ui;

    /*!
     * \brief График 
     */
    QChart chart;

    /*!
     * \brief Сплайн для графика
     */
    QSplineSeries *serires1;

    /*!
     * \brief Ось X
     */
    QValueAxis *axisX;

    /*!
     * \brief Ось Y
     */
    QValueAxis *axisY;

    /*!
     * \brief Поток для генератора координат точек
     */
    QThread thread;

    /*!
     * \brief Генератор коородинат точки
     */
    std::unique_ptr<GenerateDataWorker> generator;
};

/* --------------------------------- MainWindow ---------------------------- */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_p(std::make_unique<MainWindowPrivate>())

{
    setupUi();
    setupThread();
    createConnections();
}

MainWindow::~MainWindow()
{
    m_p->generator->stopGeneration();
    m_p->thread.terminate();
    m_p->thread.wait();
    deinitResources();
}

void MainWindow::drawChart(double x, double y)
{
    if (x > m_p->axisX->max()) {
        m_p->axisX->setRange(0, m_p->axisX->max() + STEP_INC_AXIS);
    }

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
    m_p->serires1->setColor(QColor::fromRgb(54, 110, 214));
    m_p->axisX->setRange(MIN_AXIS_VALUE, MAX_AXIS_VALUE);
    m_p->axisY->setRange(MIN_AXIS_VALUE, MAX_AXIS_VALUE);
    m_p->chart.addAxis(m_p->axisX, Qt::AlignBottom);
    m_p->chart.addAxis(m_p->axisY, Qt::AlignLeft);
    m_p->serires1->attachAxis(m_p->axisY);
    m_p->serires1->attachAxis(m_p->axisX);

    // настройка кнопок
    m_p->ui.playButton->setIcon(QIcon(":/icons/play"));
    m_p->ui.playButton->setIconSize(QSize(ICONS_SIZE, ICONS_SIZE));
    m_p->ui.playButton->setToolTip(tr("Старт генерации значений"));

    m_p->ui.pauseButton->setIcon(QIcon(":/icons/pause"));
    m_p->ui.pauseButton->setIconSize(QSize(ICONS_SIZE, ICONS_SIZE));
    m_p->ui.pauseButton->setToolTip(tr("Пауза генерации"));

    m_p->ui.stopButton->setIcon(QIcon(":/icons/stop"));
    m_p->ui.stopButton->setIconSize(QSize(ICONS_SIZE, ICONS_SIZE));
    m_p->ui.stopButton->setToolTip(tr("Остановить генерацию и очистить график"));
}

void MainWindow::setupThread()
{
    connect(&m_p->thread, &QThread::started, m_p->generator.get(), &GenerateDataWorker::run);
    connect(m_p->generator.get(), &GenerateDataWorker::finished, &m_p->thread, &QThread::terminate);
    m_p->generator->moveToThread(&m_p->thread);
}

void MainWindow::createConnections()
{
    connect(m_p->ui.playButton, &QPushButton::clicked, this, [this]() {
        m_p->generator->unpauseGeneration();
        m_p->thread.start();
    });

    connect(m_p->ui.pauseButton, &QPushButton::clicked, this, [this]() {
        m_p->generator->pauseGeneration();
    });
    connect(m_p->ui.stopButton, &QPushButton::clicked, [this](bool) {
        m_p->generator->stopGeneration();
        m_p->serires1->clear();
        m_p->axisX->setRange(MIN_AXIS_VALUE, MAX_AXIS_VALUE);
    });

    connect(m_p->generator.get(),
            &GenerateDataWorker::sendGeneratedData,
            this,
            &MainWindow::drawChart);
}
