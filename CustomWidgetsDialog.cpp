#include "CustomWidgetsDialog.h"
#include "ui_CustomWidgetsDialog.h"
#include "CustomWidgets/QToast.h"
#include "CustomWidgets/QCardDialog.h"
#include <QDebug>

CustomWidgetsDialog::CustomWidgetsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomWidgetsDialog)
{
    ui->setupUi(this);
    InitUi();
}

CustomWidgetsDialog::~CustomWidgetsDialog()
{
    delete ui;
}

void CustomWidgetsDialog::InitUi()
{
    QStringList stepList;

    stepList<<"step1"<<"step2"<<"step3"<<"step4"<<"step5";

    ui->label_circle1->SetProgressBarStyle(QFlowProgressBar::Styles::PROGRESS_BAR_CIRCLE_1);
    ui->label_circle1->SetStepMessageList(stepList);
    ui->label_circle1->SetAutoTextWidth(true);

    ui->label_circle2->SetProgressBarStyle(QFlowProgressBar::Styles::PROGRESS_BAR_CIRCLE_2);
    ui->label_circle2->SetStepMessageList(stepList);
    ui->label_circle2->SetAutoTextWidth(true);

    ui->label_rectangle->SetProgressBarStyle(QFlowProgressBar::Styles::PROGRESS_BAR_RECT);
    ui->label_rectangle->SetStepMessageList(stepList);
    ui->label_rectangle->SetAutoTextWidth(true);

    ui->label_tabstyle->SetStepMessageList(stepList);
    ui->label_tabstyle->SetAutoTextWidth(true);

    qDebug()<<QCoreApplication::applicationDirPath();

    QStringList pictureLst;
    pictureLst<<QCoreApplication::applicationDirPath()+"\\1.jpg";
    pictureLst<<QCoreApplication::applicationDirPath()+"\\2.jpg";
    pictureLst<<QCoreApplication::applicationDirPath()+"\\3.jpg";
    pictureLst<<QCoreApplication::applicationDirPath()+"\\4.gif";
    ui->label_pictures->AddPictures(pictureLst);


    QStringList posList;
    posList<<"上部居中"<<"居中"<<"底部居中";
    ui->comboBox_pos->addItems(posList);
    ui->comboBox_pos->setCurrentIndex(0);

    QStringList showTimeList;
    showTimeList<<"短"<<"长";
    ui->comboBox_time->addItems(showTimeList);
    ui->comboBox_time->setCurrentIndex(0);

    ui->textEdit_toast->setText("测试显示");

    QStringList galleryList;
    galleryList<<QCoreApplication::applicationDirPath()+"\\gallery\\1.jpg";
    galleryList<<QCoreApplication::applicationDirPath()+"\\gallery\\2.jpg";
    galleryList<<QCoreApplication::applicationDirPath()+"\\gallery\\3.jpeg";
    galleryList<<QCoreApplication::applicationDirPath()+"\\gallery\\4.jpg";
    galleryList<<QCoreApplication::applicationDirPath()+"\\gallery\\5.jpg";

    ui->gallery_main->InitImageList(galleryList);
}

void CustomWidgetsDialog::on_btn_next_clicked()
{
    ui->label_circle1->NextStep();
    ui->label_circle2->NextStep();
    ui->label_rectangle->NextStep();
    ui->label_tabstyle->NextStep();
}

void CustomWidgetsDialog::on_btn_Prev_clicked()
{
    ui->label_circle1->BackToPrevStep();
    ui->label_circle2->BackToPrevStep();
    ui->label_rectangle->BackToPrevStep();
    ui->label_tabstyle->BackToPrevStep();
}

void CustomWidgetsDialog::on_checkBox_auto_stateChanged(int arg1)
{
    if(arg1==Qt::Checked)
    {
        int nInterval=ui->lineEdit->text().toInt();
        if(nInterval>=3000)
        {
            ui->label_pictures->SetChangeInterval(nInterval);
        }
        ui->label_pictures->SetAutoChange(true);
    }
    else
    {
        ui->label_pictures->SetAutoChange(false);
    }
}

void CustomWidgetsDialog::on_pushButton_show_clicked()
{
    QToast* pToast=nullptr;
    if(ui->comboBox_time->currentIndex()==0)
    {
        pToast=QToast::CreateToast(ui->textEdit_toast->toPlainText(),QToast::LENGTH_SHORT);
    }
    else
    {
        pToast=QToast::CreateToast(ui->textEdit_toast->toPlainText(),QToast::LENGTH_LONG);
    }
    pToast->SetAutoDelete(true);

    if(ui->comboBox_pos->currentIndex()==0)
    {
        pToast->SetToastPos(QToast::TOAST_POS::TOP);
    }
    else if(ui->comboBox_pos->currentIndex()==1)
    {
        pToast->SetToastPos(QToast::TOAST_POS::CENTER);
    }
    else
    {
        pToast->SetToastPos(QToast::TOAST_POS::BOTTOM);
    }
    pToast->show();
}

void CustomWidgetsDialog::on_pushButton_card_clicked()
{
    QCardDialog tmpCardDialog;
    tmpCardDialog.exec();
}

void CustomWidgetsDialog::on_pushButton_nextStep_clicked()
{
    ui->gallery_main->MoveStep(0);
}

void CustomWidgetsDialog::on_pushButton_prev_clicked()
{
    ui->gallery_main->MoveStep(1);
}
