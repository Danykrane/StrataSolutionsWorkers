#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>

class MainWindowPrivate;

class MainWindow final : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    void drawChart();

    // первичная настройка ui
    void setupUi();
    void createConnections();

private:
    std::unique_ptr<MainWindowPrivate> m_p;
};
#endif // MAINWINDOW_H

// namespace

// pragma
