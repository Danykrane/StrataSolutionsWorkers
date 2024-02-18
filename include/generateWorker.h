#ifndef GENERATEWORKER_H
#define GENERATEWORKER_H

#include <QObject>
#include <memory>

class GenerateDataWorkerPrivate;
class GenerateDataWorker : public QObject
{
    Q_OBJECT
public:
    GenerateDataWorker();

public:
    void generateData();
    void resetData();

public slots:
    void onPlayStateChaged(bool state);

signals:
    void sendGeneratedData(double xValue, double yValue);

private:
    std::unique_ptr<GenerateDataWorkerPrivate> m_p;
};

#endif // GENERATEWORKER_H
