#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    detoxPeriod=0;
    periodEnabled=false;
    isPaused=true;
    ui->setupUi(this);
    isMinimized=false;
    create_tray_icon();
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateInterface()));
    timer->start(50);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::create_tray_icon()
{
   QSystemTrayIcon *m_tray_icon;
   m_tray_icon = new QSystemTrayIcon(QIcon(":/icons/clock_mini-128.png"), this);
   this->setWindowIcon(QIcon(":/icons/clock_mini-128.png"));

   connect( m_tray_icon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(on_show_hide(QSystemTrayIcon::ActivationReason)) );

   QAction *quit_action = new QAction( "Exit", m_tray_icon );
   connect( quit_action, SIGNAL(triggered()), this, SLOT(on_exit()) );

   QAction *hide_action = new QAction( "Show/Hide", m_tray_icon );
   connect( hide_action, SIGNAL(triggered()), this, SLOT(on_show_hide()));

   QMenu *tray_icon_menu = new QMenu;
   tray_icon_menu->addAction( hide_action );
   tray_icon_menu->addAction( quit_action );

   m_tray_icon->setContextMenu( tray_icon_menu );

   m_tray_icon->show();
 }

void MainWindow::on_show_hide( QSystemTrayIcon::ActivationReason reason )
{
   if( reason )
   {
       if( reason != QSystemTrayIcon::DoubleClick)
       return;
   }
   if( this->isVisible() )
   {
       hide();
   }
   else
   {
       show();
       raise();
       setFocus();
   }

}
void MainWindow::on_show_hide()
{
   if( this->isVisible() )
   {
       hide();
   }
   else
   {
       show();
       raise();
       setFocus();
   }

}


void MainWindow::on_exit()
{
    qApp->exit();
}



void MainWindow::on_setDetoxButton_clicked()
{
    setPeriodDialog p(this);
    if(isPaused && p.exec()== QDialog::Accepted)
    {
        detoxPeriod = p.returnPeriod();
        this->statusBar()->showMessage((QString)"Detox Period Set: " + QString::number(detoxPeriod) + " hours");
        periodEnabled = false;

    }
    else if (!isPaused)
    {
        this->statusBar()->showMessage((QString)"Please disable detox clock first.");
    }
}

void MainWindow::on_startButton_clicked()
{
    if(!periodEnabled&&isPaused&&detoxPeriod!=0)
    {
        periodEnabled = true;
        start = QDateTime::currentDateTime();
        end = start.addSecs(detoxPeriod*60*60);
        isPaused = false;
        timer->start(50);
        ui->startButton->setText("Stop");
    }
    else if (!isPaused)
    {
        isPaused = true;
        paused = QDateTime::currentDateTime();
        timer->stop();
        ui->startButton->setText("Start");
    }
    else if (isPaused)
    {
        isPaused = false;
        qint64 pausedTime = paused.msecsTo(QDateTime::currentDateTime());
        start = start.addMSecs(pausedTime);
        end = end.addMSecs(pausedTime);
        timer->start();
        ui->startButton->setText("Stop");
    }
    else if (detoxPeriod==0)
    {
        this->statusBar()->showMessage((QString)"Please set detox period first!");
    }

}

void MainWindow::updateInterface()
{
    if(periodEnabled&&ui->percentCompleted->isChecked())
    {
        double percentage = (1 - (double)(QDateTime::currentDateTime().msecsTo(end)) / (double)(start.msecsTo(end))) * 100;
        if (percentage >= 100)
            ui->label->setText("CLOCK FINISHED");
        else
            ui->label->setText(QString::number(percentage,'f',6) + "%");
    }
    else if(periodEnabled&&ui->percentLeft->isChecked())
    {
        double percentage = ((double)(QDateTime::currentDateTime().msecsTo(end)) / (double)(start.msecsTo(end))) * 100;
        if (percentage >= 100)
            ui->label->setText("CLOCK FINISHED");
        else
            ui->label->setText(QString::number(percentage,'f',6) + "%");
    }
    else if (periodEnabled&&ui->timeElapsed->isChecked())
    {
        qint64 elapsed = start.msecsTo(QDateTime::currentDateTime());
        ui->label->setText(generateTime(elapsed));

    }
    else if (periodEnabled&&ui->timeLeft->isChecked())
    {
        qint64 left = QDateTime::currentDateTime().msecsTo(end);
        if (left <= 0)
            ui->label->setText("CLOCK FINISHED");
        else
            ui->label->setText(generateTime(left));

    }
    else if (ui->currentTime->isChecked())
    {

        ui->label->setText(QDateTime::currentDateTime().toString("hh:mm:ss.zzz ap"));

    }
    else if (!periodEnabled)
        ui->label->setText(" ");

}

QString MainWindow::generateTime(qint64 msecsTotal)
{
    QString msecs = QString().sprintf("%03d",msecsTotal % 1000);
    QString secs =  QString().sprintf("%02d",msecsTotal / 1000 % 60);
    QString minutes =  QString().sprintf("%02d",msecsTotal / 60000 % 60);
    QString hours =  QString().sprintf("%02d",msecsTotal / 60000 / 60);
    return ( hours + "<font color=\"gray\">H</font>" + minutes + "<font color=\"gray\">M</font>" + secs + "<font color=\"gray\">S</font>" + msecs + "<font color=\"gray\">MS</font>");

}

void MainWindow::on_restartButton_clicked()
{
    if(periodEnabled&&!isPaused)
    {
        timer->stop();
        start = QDateTime::currentDateTime();
        end = start.addSecs(detoxPeriod*60*60);
        timer->start();
    }
    else if(periodEnabled&&isPaused)
    {
        paused = QDateTime::currentDateTime();
        start = QDateTime::currentDateTime();
        end = start.addSecs(detoxPeriod*60*60);
        updateInterface();
    }
    else
        this->statusBar()->showMessage((QString)"Please set detox period first!");


}


void MainWindow::on_timeElapsed_clicked()
{
    f = ui->label->font();
    f.setPointSize(32);
    ui->label->setFont(f);
}

void MainWindow::on_timeLeft_clicked()
{
    f = ui->label->font();
    f.setPointSize(32);
    ui->label->setFont(f);
}

void MainWindow::on_percentCompleted_clicked()
{
    f = ui->label->font();
    f.setPointSize(48);
    ui->label->setFont(f);
}

void MainWindow::on_percentLeft_clicked()
{

    f = ui->label->font();
    f.setPointSize(48);
    ui->label->setFont(f);
}

void MainWindow::on_currentTime_clicked()
{
    f = ui->label->font();
    f.setPointSize(32);
    ui->label->setFont(f);
\
}
