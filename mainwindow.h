#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <QTimer>
#include "setperioddialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_setDetoxButton_clicked();
    void updateInterface();
    void on_startButton_clicked();

    void on_restartButton_clicked();

    void on_timeElapsed_clicked();

    void on_timeLeft_clicked();

    void on_percentCompleted_clicked();

    void on_percentLeft_clicked();

    void on_currentTime_clicked();

private:
    Ui::MainWindow *ui;
    int detoxPeriod;
    bool periodEnabled;
    bool isPaused;
    QTimer *timer;
    QDateTime start, end, paused;
    int displayFlag;
    QString generateTime(qint64);
    QFont f;
};

#endif // MAINWINDOW_H
