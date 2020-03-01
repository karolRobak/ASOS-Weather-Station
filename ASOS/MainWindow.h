#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QVector>
#include "asos.h"
#include "enteredDate.h"
#include <QTime>
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
    void on_dial_valueChanged(int value);
    void OpenFile();
    void showData();

    void selectedDateChanged();
    void setMinMaxDate(QVector<asos> data);


private:
    Ui::MainWindow *ui;
    QString OpenFileName;
    QVector<asos> data;
    QTime shownTime;
    int clockValue;
    enteredDate shownDate;
    QString airportID;
    QString label;
    QDate min,max;
    int isPrinted;

};

#endif // MAINWINDOW_H
