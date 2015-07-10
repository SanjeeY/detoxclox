#ifndef SETPERIODDIALOG_H
#define SETPERIODDIALOG_H

#include <QDialog>

namespace Ui {
class setPeriodDialog;
}

class setPeriodDialog : public QDialog
{
    Q_OBJECT

public:
    explicit setPeriodDialog(QWidget *parent = 0);
    ~setPeriodDialog();
    int returnPeriod();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::setPeriodDialog *ui;
};

#endif // SETPERIODDIALOG_H
