#ifndef GENERATEWORKER_H
#define GENERATEWORKER_H

#include <QObject>
#include <memory>

class GenerateDataWorkerPrivate;
/*!
 * \brief Класс генератора координат точки
 */
class GenerateDataWorker : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Конструктор класса генератора
     */
    explicit GenerateDataWorker();
    ~GenerateDataWorker() override;

    /*!
     * \brief Остановка генерации
     */
    void stopGeneration();

    /*!
     * \brief Пауза генерации
     */
    void pauseGeneration();

    /*!
     * \brief Продолжение генерации
     */
    void unpauseGeneration();

public slots:
    /*!
     * \brief Слот генерации точки с сигналом для графика
     */
    void run();

signals:
    /*!
     * \brief Сигнал завершения слота run
     */
    void finished();

    /*!
     * \brief Сигнал с отправкой данных на график
     * \param xValue - координата X
     * \param yValue - координата Y
     */
    void sendGeneratedData(double xValue, double yValue);

private:
    /*!
     * \brief Приаптный класс с данными
     */
    std::unique_ptr<GenerateDataWorkerPrivate> m_p;
};

#endif // GENERATEWORKER_H
