#include "mainwindow.h"
#include <fstream>
#include "asos.h"
#include "enteredDate.h"
#include "ui_mainwindow.h"
#include <QTime>
MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionOpenFile,SIGNAL(triggered(bool)),this,SLOT(OpenFile()));
    connect(ui->calendarWidget,SIGNAL(selectionChanged()),this,SLOT(selectedDateChanged()));
    connect(ui->actionCloseProgram,SIGNAL(triggered(bool)),this,SLOT(close()));

    shownTime.setHMS(0,0,0);
    shownDate.year = "";
    shownDate.month = "";
    shownDate.day = "";
    label = "";
    airportID = "";

    ui->label_label->setText(shownDate.year);
    isPrinted = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::selectedDateChanged()
{
    shownDate.year = QString::number(ui->calendarWidget->yearShown());

    if(ui->calendarWidget->monthShown() >= 10)
    {
        shownDate.month = QString::number(ui->calendarWidget->monthShown());
    } else
    {
        shownDate.month = "0" + QString::number(ui->calendarWidget->monthShown());
    }

    if(ui->calendarWidget->selectedDate().day() >= 10)
    {
        shownDate.day = QString::number(ui->calendarWidget->selectedDate().day());
    } else
    {
        shownDate.day = "0" + QString::number(ui->calendarWidget->selectedDate().day());
    }

    showData();
}
void MainWindow::on_dial_valueChanged(int value)
{
    int diffHour = 0;
    int diffMinute = 0;
    for(int i=0; i < (value%2); i++){diffMinute += 30;}
    for(int k=0; k < (value/2); k++){diffHour++;}
    shownTime.setHMS(diffHour,diffMinute,0);
    ui->lcdNumber_time->display(shownTime.toString("hh:mm"));

    showData();
}
void MainWindow::setMinMaxDate(QVector<asos> data)
{
    min.setDate(data[0].year.toInt(), data[0].month.toInt(), data[0].day.toInt());
    int vectorLength = data.length() - 1;
    max.setDate(data[vectorLength].year.toInt(), data[vectorLength].month.toInt(), data[vectorLength].day.toInt());
}
void MainWindow::showData()
{

    for(auto itr = data.begin(); itr!=data.end(); ++itr)
    {
        if((*itr).year == shownDate.year)
        {
            if((*itr).month == shownDate.month)
            {
                if((*itr).day == shownDate.day)
                {
                    if((*itr).hour == shownTime.toString("hh"))
                    {
                        if((*itr).minute == shownTime.toString("mm"))
                        {
                            ui->progressBar_temperature->setValue((*itr).temperature.toDouble());
                            ui->progressBar_humidity->setValue((*itr).humidity.toDouble());
                            ui->progressBar_windDirection->setValue((*itr).windDirection.toDouble());
                            ui->progressBar_windSpeed->setValue((*itr).windSpeed.toDouble());
                            ui->progressBar_visibility->setValue((*itr).visibility.toDouble());
                            ui->lcdNumber_temperature->display((*itr).temperature.toDouble());
                            ui->lcdNumber_humidity->display((*itr).humidity.toDouble());
                            ui->lcdNumber_windDirection->display((*itr).windDirection.toDouble());
                            ui->lcdNumber_windSpeed->display((*itr).windSpeed.toDouble());
                            ui->lcdNumber_visibility->display((*itr).visibility.toDouble());

                            label = airportID + " " + shownDate.year + "-" + shownDate.month + "-" + shownDate.day
                                    + " " + shownTime.toString("hh") + ":" + shownTime.toString("mm");
                            ui->label_label->setText(label);
                            isPrinted = 1;
                            break;
                        }
                    }
                }
            }
        }
    }
    if(isPrinted == 0)
    {
        ui->progressBar_temperature->setValue(-25.0);
        ui->progressBar_humidity->setValue(0);
        ui->progressBar_windDirection->setValue(0);
        ui->progressBar_windSpeed->setValue(0);
        ui->progressBar_visibility->setValue(0);
        ui->lcdNumber_temperature->display(0);
        ui->lcdNumber_humidity->display(0);
        ui->lcdNumber_windDirection->display(0);
        ui->lcdNumber_windSpeed->display(0);
        ui->lcdNumber_visibility->display(0);

        label = airportID + " " + shownDate.year + "-" + shownDate.month + "-" + shownDate.day
                + " " + shownTime.toString("hh") + ":" + shownTime.toString("mm");
        ui->label_label->setText(label);
        isPrinted = 1;
    }

    isPrinted = 0;
}
void MainWindow::OpenFile()
{
    OpenFileName = QFileDialog::getOpenFileName(this, tr("Open..."), tr("\\home\\"), tr("ASOS file (*.asos)"));
    if(!OpenFileName.isEmpty())
    {
        QFile file(OpenFileName);
        file.open(QIODevice::ReadOnly|QIODevice::Text);
        QTextStream stream(&file);
        QString text = stream.readAll();
        QStringList list1 = text.split(QRegExp("\\s+"));


        for(int i=0; i < list1.length()-8; i+=8)
        {
            asos temp;
            temp.code=list1[i];
            temp.year=list1[i+1].mid(0,4);
            temp.month=list1[i+1].mid(5,2);
            temp.day=list1[i+1].mid(8,2);
            temp.hour=list1[i+2].mid(0,2);
            temp.minute=list1[i+2].mid(3,2);
            temp.temperature=list1[i+3];
            temp.humidity=list1[i+4];
            temp.windDirection=list1[i+5];
            temp.windSpeed=list1[i+6];
            temp.visibility=list1[i+7];
            data.push_back(temp);
        }
        airportID = text.mid(0,4);
        ui->label_label->setText(airportID);

        file.close();
        MainWindow::setMinMaxDate(data);
        ui->calendarWidget->setDateRange(min,max);

        showData();
    }
}



