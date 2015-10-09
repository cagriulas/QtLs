#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QDebug>
#include <QStringList>
#include <QRegExpValidator>
#include <QCompleter>
#include <QDirModel>

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
    void on_pushButton_Run_clicked();

    void on_pushButton_Help_clicked();

private:
    Ui::MainWindow *ui;

    QStringList *argumentList;
    QStringList *argumentOptionList;

    QRegExpValidator *validator;
    QCompleter *dirCompleter;
    QDirModel *dirModel;
};

#endif // MAINWINDOW_H
