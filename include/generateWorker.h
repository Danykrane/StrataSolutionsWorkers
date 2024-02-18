#ifndef GENERATEWORKER_H
#define GENERATEWORKER_H

#include <QObject>
#include <memory>

class GenerateDataWorkerPrivate;
class GenerateDataWorker : public QObject
{
    Q_OBJECT
public:
    explicit GenerateDataWorker();
    ~GenerateDataWorker() override;

    void stopGeneration();
    void pauseGeneration();
    void unpauseGeneration();

public slots:
    void run();

signals:

    void finished();
    void sendGeneratedData(double xValue, double yValue);

private:
    std::unique_ptr<GenerateDataWorkerPrivate> m_p;
};

#endif // GENERATEWORKER_H
