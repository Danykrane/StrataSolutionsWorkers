#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>

class MainWindowPrivate;
/*!
 * \brief Класс основного окна
 */
class MainWindow final : public QMainWindow
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(MainWindow)
public:
    /*!
     * \brief конструктор основного окна
     * \param parent - родительский класс
     */
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

public slots:
    /*!
     * \brief Слот отрисовки графика по точке
     * \param x - координата по x
     * \param y - координата по y
     */
    void drawChart(double x, double y);

private:
    /*!
     * \brief Первичная настройка UI
     */
    void setupUi();

    /*!
     * \brief Настройка потока
     */
    void setupThread();

    /*!
     * \brief Создание коннектов
     */
    void createConnections();

private:
    /*!
     * \brief Приватный класс данных окна
     */
    std::unique_ptr<MainWindowPrivate> m_p;
};
#endif // MAINWINDOW_H
