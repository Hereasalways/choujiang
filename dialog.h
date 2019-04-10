#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSound>
#include "windows.h"
#include "time.h"
#include <thread>

namespace Ui {
class Dialog;
}

struct Option {
   int maxVal;
   int minVal;
   uint32_t freq;
   Option()
       : maxVal(160),
         minVal(70),
         freq(20) {}
};

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    Ui::Dialog *ui;

    void BackEndThreadFunc(const Option &op);

private slots:
    void on_start_clicked();

private:
    Option op_;
    std::thread backEndThread;
    std::atomic_bool isStop;
};

#endif // DIALOG_H
