#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* [1]
     *  ls's all arguments with options
     */
    const QString argumentString(
                "-a -A --author -b --block-size=0 -B -c -C --color -d "
                "-D -f -F --file-type --format=1 --full-time -g "
                "--group-directories-first -G -h --si -H "
                "--dereference-command-line-symlink-to-dir --hide=p "
                "--indicator-style=2 -i --ignore=p -k -l -L "
                "-m -n -N -o -p -q --show-control-chars -Q "
                "--quoting-style=3 -r -R -s -S --sort=4 --time=5 "
                "--time-style=6 -t --tabsize=p -u -U -v --width=p "
                "-x -X -Z -1");

    const QString argumentOptionString(
                "K,M,G,T,P,E,Z,Y,KB,MB,GB,TB,PB,EB,ZB,YB "
                "across,commas,horizontal,long,single-column,verbose,vertical "
                "none,slash,file-type,classify "
                "literal,locale,shell,shell-always,c,escape "
                "none,size,time,version,extension "
                "atime,ctime "
                "full-iso, long-iso, iso, locale");
    /*-[1]-*/

    argumentList = new QStringList();
    *argumentList = argumentString.split(" ");

    argumentOptionList = new QStringList();
    *argumentOptionList = argumentOptionString.split(" ");

    //! [combobox indexes]
    ui->comboBox_blocksize->addItems(
                QStringList() << "K" << "M" << "G" << "T" <<
                                 "P" << "E" << "Z" << "Y" <<
                                 "KB" << "MB" << "GB" << "TB" <<
                                 "PB" << "EB" << "ZB" << "YB");

    ui->comboBox_format->addItems(
                QStringList() << "across -x" << "commas -m" <<
                                 "horizontal -x" << "long -l" <<
                                 "single-column -1" << "verbose -l" <<
                                 "vertical -C");

    ui->comboBox_indicatorstyle->addItems(
                QStringList() << "none (default)" << "slash (-p)" <<
                                 "file-type (--file-type)" << "classify (-F)");

    ui->comboBox_quotingstyle->addItems(
                QStringList() << "literal" << "locale" <<
                                 "shell" << "shell-always" <<
                                 "c" << "escape");

    ui->comboBox_sort->addItems(
                QStringList() << "none (-U)" << "size (-S)" <<
                                 "time (-t)" << "version (-v)" <<
                                 "extension (-X)");

    ui->comboBox_time->addItems(
                QStringList() << "atime/access/use" << "ctime/status");

    ui->comboBox_timestyle->addItems(
                QStringList() << "full-iso" << "long-iso" <<
                                 "iso" << "locale");
    //! [combobox indexes]
    //! [linedit restrictions]
    QRegExp rx("[1-9]\\d{0,18}");
    validator = new QRegExpValidator(rx);
    ui->lineEdit_tabsize->setValidator(validator);
    ui->lineEdit_width->setValidator(validator);
    //! [linedit restrictions]
    //! [dir completer]
    dirModel = new QDirModel(this);
    dirCompleter = new QCompleter(dirModel, this);
    dirCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    dirCompleter->setCompletionMode(QCompleter::InlineCompletion);
    ui->lineEdit_path->setCompleter(dirCompleter);
    //! [dir completer]
}

MainWindow::~MainWindow()
{
    delete ui;

    delete argumentList;
    delete argumentOptionList;
    delete validator;
}

void MainWindow::on_pushButton_Run_clicked()
{
    //get checked checkboxes
    QList<QAbstractButton *> argumentBoxes = ui->checkboxGroup->buttons();

    QStringList selectedArgumentsWithOptions;

    for (int i = 0; i < argumentBoxes.size(); ++i)
    {
        if (argumentBoxes.at(i)->isChecked())
        {
            QString argument = argumentList->at(i);
            QString option = "";

            if (argument.contains('='))
            {
                QStringList argumentFragments = argument.split("=");
                argument = argumentFragments.at(0) + "=";
                QString argumentOptionType = argumentFragments.at(1);

                if ("p" == argumentOptionType)
                {
                    switch (i) {
                    case 23:
                        //hide p
                        option = ui->lineEdit_hide->text();
                        break;
                    case 26:
                        //ignore
                        option = ui->lineEdit_ignore->text();
                        break;
                    case 47:
                        //tab size
                        if (ui->lineEdit_tabsize->text().isEmpty())
                        {
                            option = QString("8");
                            break;
                        }
                        option = ui->lineEdit_tabsize->text();
                        break;
                    case 51:
                        //width
                        if (ui->lineEdit_width->text().isEmpty())
                        {
                            option = QString("8");
                            break;
                        }
                        option = ui->lineEdit_width->text();
                        break;
                    default:
                        break;
                    }
                }
                else
                {
                    QStringList argumentOptions = argumentOptionList->at(argumentOptionType.toInt()).split(",");

                    switch (i) {
                    case 4:
                        //block size
                        option = argumentOptions.at(ui->comboBox_blocksize->currentIndex());
                        break;
                    case 14:
                        //format
                        option = argumentOptions.at(ui->comboBox_format->currentIndex());
                        break;
                    case 24:
                        //indicator style
                        option = argumentOptions.at(ui->comboBox_indicatorstyle->currentIndex());
                        break;
                    case 38:
                        //quoting style
                        option = argumentOptions.at(ui->comboBox_quotingstyle->currentIndex());
                        break;
                    case 43:
                        //sort
                        option = argumentOptions.at(ui->comboBox_sort->currentIndex());
                        break;
                    case 44:
                        //time
                        option = argumentOptions.at(ui->comboBox_time->currentIndex());
                        break;
                    case 45:
                        //time style
                        option = argumentOptions.at(ui->comboBox_timestyle->currentIndex());
                        break;
                    default:
                        break;
                    }
                }
            }

            selectedArgumentsWithOptions << (argument + option);
        }
    }
    //get path argument
    selectedArgumentsWithOptions << ui->lineEdit_path->text();
    /*
     * selected arguments are stored like this
     * ("--author", "-c", "--sort=size")
     */

    QString programString = "ls " + selectedArgumentsWithOptions.join(" ");
    /*
     * programString example
     * "ls --author -c --sort=size "
     */
    qDebug() << programString;

    QProcess ls(this);
    ls.start(programString);

    ls.waitForFinished();

    QByteArray result = ls.readAllStandardOutput();
    if (result.isEmpty())
    {
        result = ls.readAllStandardError();
    }

    QString delimeter = "\n______________________________________________________________\n";
    ui->textBrowser->setText(programString + delimeter + result);
}

void MainWindow::on_pushButton_Help_clicked()
{
    QProcess ls(this);
    ls.start("ls", QStringList() << "--help");

    ls.waitForFinished();

    QByteArray result = ls.readAllStandardOutput();

    ui->textBrowser->setText(result);
}
