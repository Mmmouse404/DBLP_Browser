#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QSettings>
#include <QStandardPaths>
#include <QCompleter>
#include <QListView>
#include <QTimer>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Table_1->setColumnWidth(0, static_cast<int>(width() * 0.3));
    ui->action_AuthorCount->setChecked(false);
    main_parser = new Parser(this);
    main_finder = new Finder(this);
    Finder::init();
    connect(main_parser, &Parser::done,
            main_finder, &Finder::init);
    searchTimer = new QTimer(this);
    searchTimer->setSingleShot(true);
    searchTimer->setInterval(200);  // 设置延迟时间为500毫秒（根据需要调整）
    connect(ui->fuzzy, &QCheckBox::stateChanged, this, [this](int state){
        if (state == Qt::Checked) {
            if(ui->action_FuzzyOpen->isChecked()){
                connect(ui->lineEdit, &QLineEdit::textChanged, this, &MainWindow::onFuzzyChanged);
                connect(searchTimer, &QTimer::timeout, this, &MainWindow::onFuzzyChanged);
            }
            else {
                QMessageBox:: information(this, "tips", "not open fuzzy！");
                ui->fuzzy->setCheckState(Qt::Unchecked);
            }
        }
        else {
            disconnect(ui->lineEdit, &QLineEdit::textChanged, this, &MainWindow::onFuzzyChanged);
            disconnect(searchTimer, &QTimer::timeout, this, &MainWindow::onFuzzyChanged);
        }
    });
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(ui->Table_1);
    layout->addWidget(ui->Table_2);
    ui->centralwidget->setLayout(layout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_HowToUse_triggered()
{
    QMessageBox msgBox;
    msgBox.setText(tr("fuzzy:fuzzyopen first to open the fuzzy function,and start searching!"));
    msgBox.exec();
    //QMessageBox::aboutQt(this, tr("About Qt"));
}

void MainWindow::on_action_Exit_triggered(){close();}


void MainWindow::on_searchButton_clicked()
{
    // 获取输入的搜索关键词
    QString key = ui->lineEdit->text();
    // 如果关键词为空，则显示消息框提示输入搜索关键词，并返回
    if(key.isEmpty()){
        QMessageBox::information(this, tr("information"), tr("please input key."));
        return ;
    }
    // 读取设置中的最后打开的文件名
    QSettings settings;
    Q_ASSERT(settings.contains("lastOpenFileName"));
    QString fileName = settings.value("lastOpenFileName").toString();

    // 根据选中的搜索方式执行相应操作

    // 以作者姓名搜索，并在表格中显示搜索结果
    if(ui->AuthorButton->isChecked()){

        auto list = main_finder->indexOfAuthor(key);
        if(list.isEmpty()){
            QMessageBox::information(this, tr("information"), tr("author not found。"));
            return ;
        }

        // 清除表格内容并设置行数
        ui->Table_1->clearContents();
        ui->Table_1->setRowCount(list.size());

        // 填充表格数据
        for(int i = 0; i < list.size(); ++i){
            auto pos = list.at(i);
            Recorder record(Util::findRecord(fileName, pos));
            // 获取记录的作者列表
            QStringList authorList = record.authors();
            // 使用逗号将作者列表连接成一个字符串
            QString allAuthors = authorList.join(", ");

            // 将结果显示在界面上
            ui->Table_1->setItem(i, 0, new QTableWidgetItem(record.title()));
            ui->Table_1->setItem(i, 1, new QTableWidgetItem(allAuthors));
            ui->Table_1->setItem(i, 2, new QTableWidgetItem(record.mdate()));
            ui->Table_1->setItem(i, 3, new QTableWidgetItem(record.publisher()));
            ui->Table_1->setItem(i, 4, new QTableWidgetItem(record.key()));
        }
        ui->Table_1->setColumnWidth(0, 100);
        ui->Table_1->setColumnWidth(1, 100);  // 将第2列的宽度设置为300像素
        ui->Table_1->resizeRowsToContents(); // 调整行高
        ui->Table_1->setEditTriggers(QAbstractItemView::NoEditTriggers);//表格不可编辑
    }

    // 以文章标题搜索，并在标签中显示搜索结果的详细信息
    else if(ui->TitleButton->isChecked()){
        auto list = main_finder->indexOfTitle(key);
        if(list.isEmpty()){
            QMessageBox::information(this, tr("information"), tr("title not found."));
            return ;
        }

        // 清空标签内容
        ui->Label->clear();
        QString text;

        // 构建详细信息文本
        for(int i = 0; i < list.size(); ++i){
            auto pos = list.at(i);
            Recorder record(Util::findRecord(fileName, pos));
            QString authorText;

            // 构建作者信息文本
            foreach(QString author, record.authors()){
                authorText.append(tr("作者：%1 <br/>").arg(author));
            }

            // 构建完整记录文本
            text.append(tr(R"(<b>记录详情</b><br/>
                            标题：%1 <br/>
                            %2
                            修改日期：%3 <br/>
                            发布者: %4 <br/>
                            关键词：%5 <br/><br/>
                            )").arg(record.title()).arg(authorText).arg(record.mdate()).arg(record.publisher()).arg(record.key()));
        }

        // 在标签中显示详细信息
        ui->Label->setText(text);
    }

    // 以合著作者搜索，并在文本浏览器中显示搜索结果的合著作者
    else if(ui->CoauthorButton->isChecked()){
        auto list = main_finder->indexOfAuthor(key);
        if(list.isEmpty()){
            QMessageBox::information(this, tr("information"), tr("coauthor not found。"));
            return ;
        }

        // 构建合著作者文本
        QString text;
        QStringList coauthorlist;
        for(int i = 0; i < list.size(); ++i){
            quint32 pos = list.at(i);
            Recorder record(Util::findRecord(fileName, pos));
            QStringList tmplist=record.coauthors();
            for(int j = 0; j< tmplist.size();++j){
                coauthorlist.append(tmplist.at(j));
            }
            record.clearCoauthors();
        }

        // 去重并构建文本
        QSet<QString> coauthorSet;
        foreach (const QString &str, coauthorlist) {
            coauthorSet.insert(str);
        }
        coauthorSet.remove(key);
        foreach (const QString &value, coauthorSet)
            text.append(tr(R"(coauthor：%1 <br/>)").arg(value));

        // 在文本浏览器中显示合著作者
        ui->Browser->setText(text);
    }

    // 以关键字搜索，并在表格中显示搜索结果
    else if(ui->KeyButton->isChecked()){

        auto list = main_finder->indexOfKey(key);
        if(list.isEmpty()){
            QMessageBox::information(this, tr("information"), tr("keyword not found."));
            return ;
        }

        // 清除表格内容并设置行数
        ui->Table_1->clearContents();
        ui->Table_1->setRowCount(list.size());

        // 填充表格数据
        for(int i = 0; i < list.size(); ++i){
            auto pos = list.at(i);
            Recorder record(Util::findRecord(fileName, pos));
            // 获取记录的作者列表
            QStringList authorList = record.authors();
            // 使用逗号将作者列表连接成一个字符串
            QString allAuthors = authorList.join(", ");

            // 将结果显示在界面上
            ui->Table_1->setItem(i, 0, new QTableWidgetItem(record.title()));
            ui->Table_1->setItem(i, 1, new QTableWidgetItem(allAuthors));
            ui->Table_1->setItem(i, 2, new QTableWidgetItem(record.mdate()));
            ui->Table_1->setItem(i, 3, new QTableWidgetItem(record.publisher()));
            ui->Table_1->setItem(i, 4, new QTableWidgetItem(record.key()));
        }
        ui->Table_1->setColumnWidth(0, 200);
        ui->Table_1->setColumnWidth(1, 150);  // 将第2列的宽度设置为300像素
        // 调整行高
        ui->Table_1->resizeRowsToContents();
    }

    else if(ui->FuzzyButton->isChecked())
    {
        if(ui->action_FuzzyOpen->isChecked()){
            QList<quint32>list;
            QString key = ui->lineEdit->text();
            QVector<int> result;//=keypos[lemma(previousText)];
                QString filtered = filterNonPunctuation(key);
                QStringList words = filtered.split(" ", Qt::SkipEmptyParts);
                for (const QString& word : words)result.append(keypos[lemma(word)]);
                for (const auto& str : result) {
                    //qDebug() << alltitle[str] << "\n";
                    list.append(main_finder->indexOfTitle(alltitle[str]));
                    if(list.size()>1000)break;
                }

        // 清除表格内容并设置行数
        ui->Table_2->clearContents();
        ui->Table_2->setRowCount(list.size());
        // 填充表格数据
        for(int i = 0; i < list.size(); ++i){
            auto pos = list.at(i);
            Recorder record(Util::findRecord(fileName, pos));


            // 将结果显示在界面上
            ui->Table_2->setItem(i, 0, new QTableWidgetItem(record.title()));
            ui->Table_2->setItem(i, 1, new QTableWidgetItem(record.mdate()));
            ui->Table_2->setItem(i, 2, new QTableWidgetItem(record.key()));
        }
        ui->Table_2->setColumnWidth(0, 200);
        //ui->Table_1->setColumnWidth(1, 150);  // 将第2列的宽度设置为300像素
        // 调整行高
        ui->Table_2->resizeRowsToContents();

        }
    }
}

void MainWindow::on_action_Open_triggered()
{
    main_parser->condition=0;
    // 获取最后打开的文件名
    QString lastOpenFileName;
    QSettings settings;
    if(settings.contains("lastOpenFileName")){
        lastOpenFileName = settings.value("lastOpenFileName").toString();
    }else{
        // 使用文档位置作为默认路径
        lastOpenFileName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    }

    // 打开文件选择对话框，让用户选择要打开的XML文件
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("choose XML file"),
                                                    lastOpenFileName,
                                                    tr("XML file (*.xml)"));
    // 如果未选择文件，则返回
    if(fileName.isEmpty()) return ;

    // 将选择的文件名保存到设置中的最后打开文件名
    settings.setValue("lastOpenFileName", fileName);

    // 当文件大小超过64MiB时询问用户
    if(QFile(fileName).size() > (1 << 26)){
        QMessageBox box(this);
        box.setText(tr("XML parse will take lots memory"));
        box.setInformativeText(tr("still parse?"));
        box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        box.setDefaultButton(QMessageBox::No);
        int ret = box.exec();
        if(ret == QMessageBox::No) return ;
    }

    // 创建ParseDialog对话框，连接解析器的信号和ParseDialog的槽，显示对话框并启动解析器
    ParserDialog *dialog = new ParserDialog(this);
    connect(main_parser, &Parser::stateChanged,
            dialog, &ParserDialog::showStatus);
    connect(main_parser, &Parser::done,
            dialog, &ParserDialog::activeButton);
    dialog->open();
    main_parser->setFileName(fileName);
    main_parser->start();
}


void MainWindow::on_action_Status_triggered()
{
    // 创建消息框对象，将窗口作为消息框的父对象
    QMessageBox msgBox(this);

    // 如果XML文件已解析，则设置消息文本和按钮
    if(main_finder->parsed()){
        msgBox.setText(tr("XML文件已解析。"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
    }else{
        // 如果XML文件未解析，则设置消息文本和按钮，并修改按钮文本
        msgBox.setText(tr("没有解析任何XML文件。"));
        msgBox.setStandardButtons(QMessageBox::Open|QMessageBox::Cancel);
        msgBox.button(QMessageBox::Open)->setText(tr("open XML file"));
        msgBox.button(QMessageBox::Cancel)->setText(tr("cancel"));
        msgBox.setDefaultButton(QMessageBox::Cancel);
    }

    // 显示消息框并等待用户操作
    int ret = msgBox.exec();

    // 如果用户点击打开按钮，则触发打开文件操作
    if(ret == QMessageBox::Open){
        on_action_Open_triggered();
    }
}

void MainWindow::on_action_AuthorCount_triggered(){
    main_parser->condition=1;
    // 获取最后打开的文件名
    QString lastOpenFileName;
    QSettings settings;
    if(settings.contains("lastOpenFileName")){
        lastOpenFileName = settings.value("lastOpenFileName").toString();
    }
    // 打开文件选择对话框，让用户选择要打开的XML文件
    QString fileName = lastOpenFileName;
    // 如果未选择文件，则返回
    if(fileName.isEmpty()) return ;
    // 当文件大小超过64MiB时询问用户
    if(QFile(fileName).size() > (1 << 26)){
        QMessageBox box(this);
        if(main_parser->authorflag==1){
            box.setText(tr("author frequency have been counted,restart will take some time. "));
            box.setInformativeText(tr("still restart?"));
            box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            box.setDefaultButton(QMessageBox::No);
            int ret = box.exec();
            if(ret == QMessageBox::No) main_parser->authorflag=1 ;
            if(ret == QMessageBox::Yes) main_parser->authorflag=0 ;
        }
        else if(main_parser->authorflag==0){
            box.setText(tr("author frequency counting start will take some time. "));
            box.setInformativeText(tr("start?"));
            box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            box.setDefaultButton(QMessageBox::No);
            int ret = box.exec();

            if(ret == QMessageBox::No) {ui->action_AuthorCount->setChecked(false);return ;}

            if(ret == QMessageBox::Yes) {ui->action_AuthorCount->setChecked(true);}
        }
    }
    // 创建ParseDialog对话框，连接解析器的信号和ParseDialog的槽，显示对话框并启动解析器
    ParserDialog *dialog = new ParserDialog(this);
    connect(main_parser, &Parser::stateChanged,
            dialog, &ParserDialog::showStatus);
    connect(main_parser, &Parser::done,
            dialog, &ParserDialog::activeButton);
    dialog->open();
    main_parser->setFileName(fileName);
    main_parser->start();
}

void MainWindow::on_action_KeyWordCount_triggered(){
    main_parser->condition=4;
    // 获取最后打开的文件名
    QString lastOpenFileName;
    QSettings settings;
    if(settings.contains("lastOpenFileName")){
        lastOpenFileName = settings.value("lastOpenFileName").toString();
    }
    // 打开文件选择对话框，让用户选择要打开的XML文件
    QString fileName = lastOpenFileName;
    // 如果未选择文件，则返回
    if(fileName.isEmpty()) return ;
    // 当文件大小超过64MiB时询问用户
    if(QFile(fileName).size() > (1 << 26)){
        QMessageBox box(this);
        if(main_parser->keyflag==1){
            box.setText(tr("keyword frequency have been counted,restart will take some time. "));
            box.setInformativeText(tr("still restart?"));
            box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            box.setDefaultButton(QMessageBox::No);
            int ret = box.exec();
            if(ret == QMessageBox::No) main_parser->keyflag=1 ;
            if(ret == QMessageBox::Yes) main_parser->keyflag=0 ;
        }
        else if(main_parser->keyflag==0){
            box.setText(tr("keyword frequency count will take some time. "));
            box.setInformativeText(tr("start?"));
            box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            box.setDefaultButton(QMessageBox::No);
            int ret = box.exec();
            if(ret == QMessageBox::No) {ui->action_KeyWordCount->setChecked(false);return ;}
            if(ret == QMessageBox::Yes) {ui->action_KeyWordCount->setChecked(true);}
        }
    }
    // 创建ParseDialog对话框，连接解析器的信号和ParseDialog的槽，显示对话框并启动解析器
    ParserDialog *dialog = new ParserDialog(this);
    connect(main_parser, &Parser::stateChanged,
            dialog, &ParserDialog::showStatus);
    connect(main_parser, &Parser::done,
            dialog, &ParserDialog::activeButton);
    dialog->open();
    main_parser->setFileName(fileName);
    main_parser->start();
    if(main_parser->keyflag==1){ui->action_KeyWordCount->setChecked(true);}
}


void MainWindow::on_action_GroupAnalyze_triggered(){
    main_parser->condition=3;
    // 获取最后打开的文件名
    QString lastOpenFileName;
    QSettings settings;
    if(settings.contains("lastOpenFileName")){
        lastOpenFileName = settings.value("lastOpenFileName").toString();
    }
    // 打开文件选择对话框，让用户选择要打开的XML文件
    QString fileName = lastOpenFileName;
    // 如果未选择文件，则返回
    if(fileName.isEmpty()) return ;
    // 当文件大小超过64MiB时询问用户
    if(QFile(fileName).size() > (1 << 26)){
        QMessageBox box(this);
        if(main_parser->groupflag==1){
            box.setText(tr("author group have been analyzed,restart will take some time. "));
            box.setInformativeText(tr("still restart?"));
            box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            box.setDefaultButton(QMessageBox::No);
            int ret = box.exec();
            if(ret == QMessageBox::No) main_parser->groupflag=1 ;
            if(ret == QMessageBox::Yes) main_parser->groupflag=0 ;
        }
        else if(main_parser->groupflag==0){
            box.setText(tr("author frequency analyze will take some time. "));
            box.setInformativeText(tr("still start?"));
            box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            box.setDefaultButton(QMessageBox::No);
            int ret = box.exec();
            if(ret == QMessageBox::No) {ui->action_GroupAnalyze->setChecked(false);return ;}
            if(ret == QMessageBox::Yes) {ui->action_GroupAnalyze->setChecked(true);}
        }
    }
    // 创建ParseDialog对话框，连接解析器的信号和ParseDialog的槽，显示对话框并启动解析器
    ParserDialog *dialog = new ParserDialog(this);
    connect(main_parser, &Parser::stateChanged,
            dialog, &ParserDialog::showStatus);
    connect(main_parser, &Parser::done,
            dialog, &ParserDialog::activeButton);
    dialog->open();
    main_parser->setFileName(fileName);
    main_parser->start();
}

void MainWindow::on_action_FuzzyOpen_triggered(){
    main_parser->condition=2;
    // 获取最后打开的文件名
    QString lastOpenFileName;
    QSettings settings;
    if(settings.contains("lastOpenFileName")){
        lastOpenFileName = settings.value("lastOpenFileName").toString();
    }
    // 打开文件选择对话框，让用户选择要打开的XML文件
    QString fileName = lastOpenFileName;int flag=0;
    // 如果未选择文件，则返回
    if(fileName.isEmpty()) return ;
    // 当文件大小超过64MiB时询问用户
    if(QFile(fileName).size() > (1 << 26)){
        QMessageBox box(this);
        if(main_parser->titleflag==1){
            box.setText(tr("fuzzy have been opened,restart will take some time. "));
            box.setInformativeText(tr("still restart?"));
            box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            box.setDefaultButton(QMessageBox::No);
            int ret = box.exec();
            if(ret == QMessageBox::No) main_parser->titleflag=1 ;
            if(ret == QMessageBox::Yes) {main_parser->titleflag=0 ;flag=1;}
        }
        else if(main_parser->titleflag==0){
            box.setText(tr("fuzzy will take some time. "));
            box.setInformativeText(tr("still start?"));
            box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            box.setDefaultButton(QMessageBox::No);
            int ret = box.exec();
            if(ret == QMessageBox::No) {ui->action_FuzzyOpen->setChecked(false);main_parser->titleflag=0 ;return ;}
            if(ret == QMessageBox::Yes) {ui->action_FuzzyOpen->setChecked(true);flag=1;}
        }
    }
    // 创建ParseDialog对话框，连接解析器的信号和ParseDialog的槽，显示对话框并启动解析器
    if(flag==1){
        ParserDialog *dialog = new ParserDialog(this);
        connect(main_parser, &Parser::stateChanged,
                dialog, &ParserDialog::showStatus);
        connect(main_parser, &Parser::done,
                dialog, &ParserDialog::activeButton);
        dialog->open();
        main_parser->setFileName(fileName);
        main_parser->start();

    }
}

void MainWindow::on_action_Clear_triggered()
{
    main_parser->clearIndex();
    main_finder->clearIndex();
    statusBar()->showMessage(tr("Clear index file successful!"));
}

void MainWindow::on_action_Language_triggered()
{
    SettingDialog *dialog = new SettingDialog(this);
    dialog->open();ui->retranslateUi(this);
}

void MainWindow::Resize(QResizeEvent *event)
{
    Q_UNUSED(event)
    ui->Table_1->setColumnWidth(0, static_cast<int>(width() * 0.5));
}

void MainWindow::on_AuthorButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->lineEdit->setFocus();
}

void MainWindow::on_TitleButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->lineEdit->setFocus();
}

void MainWindow::on_CoauthorButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->lineEdit->setFocus();
}

void MainWindow::on_KeyButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->lineEdit->setFocus();
}

void MainWindow::on_FuzzyButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    ui->lineEdit->setFocus();
}

void MainWindow::onFuzzyChanged(){
    if(ui->action_FuzzyOpen->isChecked()){
        QString key = ui->lineEdit->text();
        wordList.clear();
        if(key.length()>2&&previousText!=key){
            previousText=key; QVector<int> result;//=keypos[lemma(previousText)];
             QString filtered = filterNonPunctuation(previousText);
             QStringList words = filtered.split(" ", Qt::SkipEmptyParts);
            for (const QString& word : words)result.append(keypos[lemma(word)]);
            for (const auto& str : result) {
                wordList.append(alltitle[str]);
                if(wordList.size()>500)break;
            }
        }
        //输入触发自动补全
        QCompleter *pCompleter = new QCompleter(wordList, this);
        pCompleter->setFilterMode(Qt::MatchContains);
        pCompleter->setCompletionMode(QCompleter::PopupCompletion);
        pCompleter->setCaseSensitivity(Qt::CaseInsensitive);
        ui->lineEdit->setCompleter(pCompleter);
    }

}
