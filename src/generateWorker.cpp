#include <generateWorker.h>

#include <QMutex>
#include <QRandomGenerator>
#include <QThread>

namespace {
const int MIN_POINT_VALUE = 0;
const int MAX_POINT_VALUE = 15;
} // namespace

class GenerateDataWorkerPrivate
{
public:
    QMutex mutex;

    int xCounter = 0;
    bool playState = false;
};

GenerateDataWorker::GenerateDataWorker()
    : m_p(std::make_unique<GenerateDataWorkerPrivate>())
{}

void GenerateDataWorker::generateData()
{
    QMutexLocker locker(&m_p->mutex);
    //    while (m_p->playState) {
    double x = m_p->xCounter;
    double y = QRandomGenerator::global()->bounded(MIN_POINT_VALUE, MAX_POINT_VALUE);

    m_p->xCounter++;
    emit sendGeneratedData(x, y);
    QThread::msleep(200);
    //    }
}

void GenerateDataWorker::resetData()
{
    m_p->xCounter = 0;
}

void GenerateDataWorker::onPlayStateChaged(bool state)
{
    m_p->playState = state;
}
