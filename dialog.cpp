#include "dialog.h"
#include "ui_dialog.h"
#include <QMessageBox>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    isStop = true;
    ui->setupUi(this);
    ui->lineEdit_minVal->setText(QString::number(op_.minVal));
    ui->lineEdit_maxVal->setText(QString::number(op_.maxVal));
    ui->sleep->setText(QString::number(op_.freq));
}

Dialog::~Dialog()
{
    delete ui;
    if (!isStop.exchange(true))
    {
        backEndThread.join();
        return;
    }
}

void Dialog::BackEndThreadFunc(const Option &op) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(op.minVal, op.maxVal);
    while (!isStop.load()) {
        int ranNum = dis(gen);
        QString out = QString::number(ranNum, 10);
        ui -> result -> display(out);
        std::this_thread::sleep_for(
                    std::chrono::milliseconds(op.freq));
    }
}

void Dialog::on_start_clicked()
{
    ui->start->setDisabled(true);
    op_.minVal = ui -> lineEdit_minVal -> text().toInt();
    op_.maxVal = ui -> lineEdit_maxVal -> text().toInt();
    op_.freq = ui->sleep->text().toUInt();

    if (op_.minVal > op_.maxVal) {
       auto temp = op_.maxVal;
       op_.maxVal = op_.minVal;
       op_.minVal = temp;
    }
    if (op_.minVal < 0 ||
        op_.minVal > 100000 ||
        op_.maxVal < 0 ||
        op_.maxVal > 100000)
    {
        QMessageBox::about(this, tr("错误信息"), tr("抽奖范围必须是大于0，小于100000的正整数"));
        ui->start->setDisabled(false);
        return;
    }
    if (isStop.exchange(false)) {
        ui->start->setText("停止");
        backEndThread = std::thread(&Dialog::BackEndThreadFunc, this, op_);
        ui->start->setDisabled(false);
        return;
    }

    if (!isStop.exchange(true))
    {
        ui -> start -> setText("开始");
        backEndThread.join();
        ui->start->setDisabled(false);
        return;
    }
}

