#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    detoxPeriod=0;
    periodEnabled=false;
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
    if(!periodEnabled && p.exec()== QDialog::Accepted)
    {
        detoxPeriod = p.returnPeriod();
        this->statusBar()->showMessage((QString)"Detox Period Set: " + QString::number(detoxPeriod) + " hours");
        periodEnabled = true;
        ui->startButton->setEnabled(true);

    }
    else if (periodEnabled &&  p.exec()== QDialog::Accepted)
    {
        timer->stop();
        detoxPeriod = p.returnPeriod();
        this->statusBar()->showMessage((QString)"Detox Period Set: " + QString::number(detoxPeriod) + " hours");
        ui->startButton->setText("Start");
        updateInterface();
    }
}

void MainWindow::on_startButton_clicked()
{     
        ui->saveButton->setEnabled(true);
        start = QDateTime::currentDateTime();
        end = start.addSecs(detoxPeriod*60*60);
        timer->start(50);
        ui->startButton->setText("Restart");
}

void MainWindow::updateInterface()
{
    if (ui->currentTime->isChecked())
        ui->label->setText(QDateTime::currentDateTime().toString("hh:mm:ss.zzz ap"));
    else if(periodEnabled)
    {
        if(ui->percentCompleted->isChecked())
        {
            double percentage = (1 - (double)(QDateTime::currentDateTime().msecsTo(end)) / (double)(start.msecsTo(end))) * 100;
            if (percentage >= 100)
                ui->label->setText("CLOCK FINISHED");
            else
                ui->label->setText(QString::number(percentage,'f',6) + "%");
        }
        else if(ui->percentLeft->isChecked())
        {
            double percentage = ((double)(QDateTime::currentDateTime().msecsTo(end)) / (double)(start.msecsTo(end))) * 100;
            if (percentage >= 100)
                ui->label->setText("CLOCK FINISHED");
            else
                ui->label->setText(QString::number(percentage,'f',6) + "%");
        }
        else if (ui->timeElapsed->isChecked())
        {
            qint64 elapsed = start.msecsTo(QDateTime::currentDateTime());
            ui->label->setText(generateTime(elapsed));
        }
        else if (ui->timeLeft->isChecked())
        {
            qint64 left = QDateTime::currentDateTime().msecsTo(end);
            if (left <= 0)
                ui->label->setText("CLOCK FINISHED");
            else
                ui->label->setText(generateTime(left));
        }
    }
    else if (!periodEnabled)
        ui->label->setText("Detoxclox not active");

}

QString MainWindow::generateTime(qint64 msecsTotal)
{
    QString msecs = QString().sprintf("%03d",msecsTotal % 1000);
    QString secs =  QString().sprintf("%02d",msecsTotal / 1000 % 60);
    QString minutes =  QString().sprintf("%02d",msecsTotal / 60000 % 60);
    QString hours =  QString().sprintf("%02d",msecsTotal / 60000 / 60);
    return ( hours + "<font color=\"gray\">H</font>" + minutes + "<font color=\"gray\">M</font>" + secs + "<font color=\"gray\">S</font>" + msecs + "<font color=\"gray\">MS</font>");

}


void MainWindow::on_timeElapsed_clicked()
{
    f = ui->label->font();
    ui->label->setFont(f);
}

void MainWindow::on_timeLeft_clicked()
{
    f = ui->label->font();
    ui->label->setFont(f);
}

void MainWindow::on_percentCompleted_clicked()
{
    f = ui->label->font();
    ui->label->setFont(f);
}

void MainWindow::on_percentLeft_clicked()
{

    f = ui->label->font();
    ui->label->setFont(f);
}

void MainWindow::on_currentTime_clicked()
{
    f = ui->label->font();
    ui->label->setFont(f);
}

void MainWindow::on_saveButton_clicked()
{
    if(periodEnabled)
    {
        QString fn = QFileDialog::getSaveFileName(this, tr("Save File..."),
                                                   QString(), tr("dat files (*.dat);;All Files (*)"));
         if (!fn.isEmpty())
         {
            QFile file(fn);
            file.open(QIODevice::WriteOnly);
            QDataStream out(&file);
            out << start.toMSecsSinceEpoch() << end.toMSecsSinceEpoch() << detoxPeriod;
            file.flush();
            file.close();
            this->statusBar()->showMessage("Detox period saved.");
         }
    }
    else
    {
        this->statusBar()->showMessage("Cannot save. No period set.");
    }
}

void MainWindow::on_loadButton_clicked()
{
    QString fn = QFileDialog::getOpenFileName(this, tr("Open File..."),
                                              QString(), tr("dat files (*.dat);;All Files (*)"));
    if (!fn.isEmpty())
    {
        QFile file(fn);
        file.open(QIODevice::ReadOnly);
        QDataStream in(&file);
        qint64 s, e;
        in >> s;
        in >> e;
        start.setMSecsSinceEpoch(s);
        end.setMSecsSinceEpoch(e);
        in >> detoxPeriod;
        file.close();
        periodEnabled=true;
        this->statusBar()->showMessage((QString)"Detox Period Set: " + QString::number(detoxPeriod) + " hours");
        ui->saveButton->setDisabled(true);
        ui->startButton->setText("Restart");
        ui->startButton->setEnabled(true);
    }
}
