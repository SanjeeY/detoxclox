#include "setperioddialog.h"
#include "ui_setperioddialog.h"

setPeriodDialog::setPeriodDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setPeriodDialog)
{
    ui->setupUi(this);
}

setPeriodDialog::~setPeriodDialog()
{
    delete ui;
}

void setPeriodDialog::on_buttonBox_accepted()
{

}

int setPeriodDialog::returnPeriod()
{
    return ui->spinBox->value();
}
