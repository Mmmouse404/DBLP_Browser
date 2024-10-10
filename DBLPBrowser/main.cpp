#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include<QSettings>
#include<QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //起个名字
    QCoreApplication::setOrganizationName("SCUT");
    QCoreApplication::setApplicationName("DBLBrowser");
    QSettings settings;
    QTranslator *translator=new QTranslator;
    QString language = "system";
    //如果系统没有设置就默认是系统语言，如果设置了Chinese则显示为中文
    if(settings.contains("language")){
        qDebug()<<"Yes";
        language = settings.value("language").toString();
    }
    if(language == "system" && QLocale::system().name() == "zh_CN"){
        qDebug()<<"Yes";
        language = "chinese";
    }
    if(language == "chinese"){
        if(translator->load("translation/DBLPBrowser_zh_CN.qm")){
            qDebug()<<"Yes";qDebug()<<"Yes";
            a.installTranslator(translator);
        }
        else qDebug()<<"No";
    }
    //创建窗体
    MainWindow w;
    w.show();
    return a.exec();
}
