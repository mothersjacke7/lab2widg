#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QFile file;
    globPath = QFileDialog::getOpenFileName(nullptr,"","C:\\Qt\\lab2\\json","*.json");
    file.setFileName(globPath);
    if (file.open(QIODevice::ReadOnly|QFile::Text))
    {
        doc = QJsonDocument::fromJson(QByteArray(file.readAll()),&docError);
    }
    file.close();


    if (docError.errorString().toInt() == QJsonParseError::NoError)
    {
        QStandardItemModel* model = new QStandardItemModel(nullptr);
        model->setHorizontalHeaderLabels(QStringList()<<"name"<<"group"<<"dir");

        docAr = QJsonValue(doc.object().value("students")).toArray();
        for (int i = 0; i < docAr.count(); i++)
        {
            QStandardItem* item_col_1 = new QStandardItem(docAr.at(i).toObject().value("name").toString());
            QStandardItem* item_col_2 = new QStandardItem(QString::number(docAr.at(i).toObject().value("group").toInt()));

            QJsonArray docDir = QJsonValue(QJsonObject(docAr.at(i).toObject()).value("dir")).toArray();
            QString sost="";
            for (int j = 0; j<docDir.count();j++)
            {
                if (sost=="")
                {
                    sost = docDir[j].toString();
                }
                else
                {
                    sost = sost +";" + docDir[j].toString();
                }
            }

            QStandardItem* item_col_3 = new QStandardItem(sost);
            model->appendRow(QList<QStandardItem*>()<<item_col_1<<item_col_2<<item_col_3);

        }

        ui->tableView->setModel(model);
        ui->tableView->resizeColumnsToContents();
    }
}

