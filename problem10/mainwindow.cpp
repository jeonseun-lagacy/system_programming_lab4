#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_add_text_clicked()
{
    QString text;
    text.clear();
    text.append("hello world!\n");
    ui->tb_text->insertPlainText(text);

}

void MainWindow::on_btn_remove_clicked()
{
    QStringList lines = ui->tb_text->toPlainText().split('\n');
    lines.removeAt(lines.size()-2);
    ui->tb_text->setPlainText(lines.join('\n'));
}

void MainWindow::on_btn_exit_clicked()
{
    exit(1);
}
