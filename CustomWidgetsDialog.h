#ifndef CUSTOMWIDGETSDIALOG_H
#define CUSTOMWIDGETSDIALOG_H

#include <QDialog>

namespace Ui {
class CustomWidgetsDialog;
}

class CustomWidgetsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CustomWidgetsDialog(QWidget *parent = nullptr);
    ~CustomWidgetsDialog();

private slots:
    void on_btn_next_clicked();

    void on_btn_Prev_clicked();

    void on_checkBox_auto_stateChanged(int arg1);

    void on_pushButton_show_clicked();

    void on_pushButton_card_clicked();

private:
    Ui::CustomWidgetsDialog *ui;

    void InitUi();
};

#endif // CUSTOMWIDGETSDIALOG_H
