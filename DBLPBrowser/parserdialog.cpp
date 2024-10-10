#include "parserdialog.h"
#include "ui_parserDialog.h"
#include "parser.h"

ParserDialog::ParserDialog(QWidget *parent):
    QDialog{parent},
    ui(new Ui::parserDialog)
{
    ui->setupUi(this);
    clear();
}
ParserDialog::~ParserDialog()
{
    delete ui;  // 释放从UI文件中创建的用户界面对象
}

void ParserDialog::showStatus(const QString &msg)
{
    ui->textBrowser->append(msg);  // 在文本浏览部件中显示指定的消息
}

void ParserDialog::clear()
{
    ui->closeButton->setEnabled(false);  // 禁用按钮
}

void ParserDialog::activeButton()
{
    ui->closeButton->setEnabled(true);  // 启用按钮
}

void ParserDialog::on_closeButton_clicked()
{
    close();  // 点击按钮后关闭对话框
}
