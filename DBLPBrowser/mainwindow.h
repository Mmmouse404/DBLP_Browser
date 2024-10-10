#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "finder.h"
#include "parser.h"
#include "settingdialog.h"
#include "parserdialog.h"
#include "util.h"
#include "recorder.h"
#include "port.h"

#include <QMainWindow>
#include <QThread>

class Parser;
class ParserDialog;
class Finder;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void Resize(QResizeEvent* event) ;
    QStringList wordList;
    QString previousText;
private slots:
    //关于How To Use
    void on_action_HowToUse_triggered();
    //退出动作
    void on_action_Exit_triggered();
    //打开动作
    void on_action_Open_triggered();
    //状态动作
    void on_action_Status_triggered();
    //设置动作
    void on_action_Language_triggered();
    //作者统计动作
    void on_action_AuthorCount_triggered();
    //模糊搜索开启动作
    void on_action_FuzzyOpen_triggered();
    //作者统计动作
    void on_action_KeyWordCount_triggered();
    //聚团分析动作
    void on_action_GroupAnalyze_triggered();
    //清除索引动作
    void on_action_Clear_triggered();

    //查询按钮
    void on_searchButton_clicked();
    //作者按钮
    void on_AuthorButton_clicked();
    //标题按钮
    void on_TitleButton_clicked();
    //共同作者按钮
    void on_CoauthorButton_clicked();
    //按关键词key搜索按钮
    void on_KeyButton_clicked();
    //模糊搜索按钮
    void on_FuzzyButton_clicked();
    void onFuzzyChanged();
private:
    Ui::MainWindow *ui;
    Parser *main_parser;
    Finder *main_finder;
    QTimer *searchTimer;
};
#endif // MAINWINDOW_H
