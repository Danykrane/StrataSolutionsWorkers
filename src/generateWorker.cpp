#include <generateWorker.h>

#include <QMutex>
#include <QRandomGenerator>
#include <QThread>

namespace {
const int MIN_POINT_VALUE = 0;
const int MAX_POINT_VALUE = 15;
} // namespace

/*!
 * \brief Приватный класс с данными
 */
class GenerateDataWorkerPrivate
{
public:
    /*!
     * \brief Конструктор класса
     */
    explicit GenerateDataWorkerPrivate()
        : isRunning{ false }
        , isPaused{ false }
    {}

    /*!
     * \brief Мьютекс
     */
    QMutex mutex;

    /*!
     * \brief Cчетчик текущей координаты X
     */
    int xCounter = 0;

    /*!
     * \brief Состояние рабатающего генератора
     */
    bool isRunning;

    /*!
     * \brief Состояник генератора, поставленного на паузу
     */
    bool isPaused;
};

GenerateDataWorker::GenerateDataWorker()
    : m_p(std::make_unique<GenerateDataWorkerPrivate>())
{}

GenerateDataWorker::~GenerateDataWorker()
{
    if (m_p->isRunning)
    {
        stopGeneration();
    }
}

void GenerateDataWorker::run()
{
    m_p->isRunning = true;

    while (m_p->isRunning)
    {
        if (!m_p->isPaused)
        {
            double x = m_p->xCounter;
            double y = QRandomGenerator::global()->bounded(MIN_POINT_VALUE, MAX_POINT_VALUE);

            m_p->xCounter++;
            emit sendGeneratedData(x, y);
            QThread::msleep(250);
        }
        QThread::msleep(1);
    }
    emit finished();
}

void GenerateDataWorker::stopGeneration()
{
    QMutexLocker locker(&m_p->mutex);
    m_p->isRunning = false;
    m_p->xCounter = 0;
}

void GenerateDataWorker::pauseGeneration()
{
    QMutexLocker locker(&m_p->mutex);
    m_p->isPaused = true;
}

void GenerateDataWorker::unpauseGeneration()
{
    QMutexLocker locker(&m_p->mutex);
    m_p->isPaused = false;
}
