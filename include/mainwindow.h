#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>

class MainWindowPrivate;

class MainWindow final : public QMainWindow
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(MainWindow)
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

public slots:
    void drawChart(double x, double y);

private:
    // первичная настройка ui
    void setupUi();

    void setupThread();
    void createConnections();

private:
    std::unique_ptr<MainWindowPrivate> m_p;
};
#endif // MAINWINDOW_H
