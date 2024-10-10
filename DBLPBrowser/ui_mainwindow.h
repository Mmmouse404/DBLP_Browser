/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_HowToUse;
    QAction *action_Open;
    QAction *action_Exit;
    QAction *action_Clear;
    QAction *actionAuthor;
    QAction *action_AuthorCount;
    QAction *action_FuzzyOpen;
    QAction *action_Language;
    QAction *action_GroupAnalyze;
    QAction *action_KeyWordCount;
    QAction *action_Status;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QStackedWidget *stackedWidget;
    QWidget *authorPage;
    QTableWidget *Table_1;
    QWidget *titlePage;
    QLabel *Label;
    QWidget *coauthorPage;
    QTextBrowser *Browser;
    QWidget *fuzzyPage;
    QTableWidget *Table_2;
    QGroupBox *LabelBox;
    QHBoxLayout *horizontalLayout_5;
    QRadioButton *AuthorButton;
    QRadioButton *TitleButton;
    QRadioButton *CoauthorButton;
    QRadioButton *KeyButton;
    QRadioButton *FuzzyButton;
    QHBoxLayout *TextLine;
    QLineEdit *lineEdit;
    QCheckBox *fuzzy;
    QPushButton *searchButton;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuSetting;
    QMenu *menuFunction;
    QMenu *menuHelp;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->setEnabled(true);
        MainWindow->resize(850, 700);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/mouse.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setAutoFillBackground(false);
        MainWindow->setStyleSheet(QString::fromUtf8("#MainWindow{background-image: url(:/background.jpg);}\n"
"#centralwidget{background-image: url(:/background.jpg);}"));
        MainWindow->setTabShape(QTabWidget::Triangular);
        action_HowToUse = new QAction(MainWindow);
        action_HowToUse->setObjectName("action_HowToUse");
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/IconParkHelp.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_HowToUse->setIcon(icon1);
        action_Open = new QAction(MainWindow);
        action_Open->setObjectName("action_Open");
        action_Open->setCheckable(false);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Open->setIcon(icon2);
        action_Exit = new QAction(MainWindow);
        action_Exit->setObjectName("action_Exit");
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Exit->setIcon(icon3);
        action_Clear = new QAction(MainWindow);
        action_Clear->setObjectName("action_Clear");
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/clear.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Clear->setIcon(icon4);
        actionAuthor = new QAction(MainWindow);
        actionAuthor->setObjectName("actionAuthor");
        QIcon icon5;
        icon5.addFile(QString::fromUtf8("icons/author.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAuthor->setIcon(icon5);
        action_AuthorCount = new QAction(MainWindow);
        action_AuthorCount->setObjectName("action_AuthorCount");
        action_AuthorCount->setCheckable(true);
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/author.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_AuthorCount->setIcon(icon6);
        action_FuzzyOpen = new QAction(MainWindow);
        action_FuzzyOpen->setObjectName("action_FuzzyOpen");
        action_FuzzyOpen->setCheckable(true);
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/fuzzyopen.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_FuzzyOpen->setIcon(icon7);
        action_Language = new QAction(MainWindow);
        action_Language->setObjectName("action_Language");
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/language.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Language->setIcon(icon8);
        action_GroupAnalyze = new QAction(MainWindow);
        action_GroupAnalyze->setObjectName("action_GroupAnalyze");
        action_GroupAnalyze->setCheckable(true);
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/group.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_GroupAnalyze->setIcon(icon9);
        action_KeyWordCount = new QAction(MainWindow);
        action_KeyWordCount->setObjectName("action_KeyWordCount");
        action_KeyWordCount->setCheckable(true);
        action_KeyWordCount->setChecked(false);
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/keyword.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_KeyWordCount->setIcon(icon10);
        action_Status = new QAction(MainWindow);
        action_Status->setObjectName("action_Status");
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/fuzzy.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Status->setIcon(icon11);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        centralwidget->setAutoFillBackground(false);
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(-1, -1, -1, 5);
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(stackedWidget->sizePolicy().hasHeightForWidth());
        stackedWidget->setSizePolicy(sizePolicy);
        stackedWidget->setAutoFillBackground(false);
        stackedWidget->setStyleSheet(QString::fromUtf8(""));
        stackedWidget->setFrameShape(QFrame::NoFrame);
        authorPage = new QWidget();
        authorPage->setObjectName("authorPage");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::MinimumExpanding);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(authorPage->sizePolicy().hasHeightForWidth());
        authorPage->setSizePolicy(sizePolicy1);
        Table_1 = new QTableWidget(authorPage);
        if (Table_1->columnCount() < 5)
            Table_1->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        Table_1->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        Table_1->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        Table_1->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        Table_1->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        Table_1->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        Table_1->setObjectName("Table_1");
        Table_1->setGeometry(QRect(5, 1, 750, 500));
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(Table_1->sizePolicy().hasHeightForWidth());
        Table_1->setSizePolicy(sizePolicy2);
        Table_1->setAutoFillBackground(true);
        Table_1->horizontalHeader()->setCascadingSectionResizes(true);
        Table_1->horizontalHeader()->setStretchLastSection(true);
        Table_1->verticalHeader()->setProperty("showSortIndicator", QVariant(false));
        Table_1->verticalHeader()->setStretchLastSection(true);
        stackedWidget->addWidget(authorPage);
        titlePage = new QWidget();
        titlePage->setObjectName("titlePage");
        Label = new QLabel(titlePage);
        Label->setObjectName("Label");
        Label->setGeometry(QRect(9, 1, 641, 271));
        sizePolicy.setHeightForWidth(Label->sizePolicy().hasHeightForWidth());
        Label->setSizePolicy(sizePolicy);
        stackedWidget->addWidget(titlePage);
        coauthorPage = new QWidget();
        coauthorPage->setObjectName("coauthorPage");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::MinimumExpanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(coauthorPage->sizePolicy().hasHeightForWidth());
        coauthorPage->setSizePolicy(sizePolicy3);
        Browser = new QTextBrowser(coauthorPage);
        Browser->setObjectName("Browser");
        Browser->setGeometry(QRect(5, 1, 671, 271));
        sizePolicy1.setHeightForWidth(Browser->sizePolicy().hasHeightForWidth());
        Browser->setSizePolicy(sizePolicy1);
        stackedWidget->addWidget(coauthorPage);
        fuzzyPage = new QWidget();
        fuzzyPage->setObjectName("fuzzyPage");
        sizePolicy3.setHeightForWidth(fuzzyPage->sizePolicy().hasHeightForWidth());
        fuzzyPage->setSizePolicy(sizePolicy3);
        Table_2 = new QTableWidget(fuzzyPage);
        if (Table_2->columnCount() < 3)
            Table_2->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        Table_2->setHorizontalHeaderItem(0, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        Table_2->setHorizontalHeaderItem(1, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        Table_2->setHorizontalHeaderItem(2, __qtablewidgetitem7);
        Table_2->setObjectName("Table_2");
        Table_2->setGeometry(QRect(0, 0, 711, 431));
        sizePolicy1.setHeightForWidth(Table_2->sizePolicy().hasHeightForWidth());
        Table_2->setSizePolicy(sizePolicy1);
        Table_2->setAutoFillBackground(true);
        Table_2->horizontalHeader()->setCascadingSectionResizes(true);
        Table_2->horizontalHeader()->setStretchLastSection(true);
        Table_2->verticalHeader()->setProperty("showSortIndicator", QVariant(false));
        Table_2->verticalHeader()->setStretchLastSection(true);
        stackedWidget->addWidget(fuzzyPage);

        gridLayout->addWidget(stackedWidget, 4, 0, 1, 1);

        LabelBox = new QGroupBox(centralwidget);
        LabelBox->setObjectName("LabelBox");
        LabelBox->setEnabled(true);
        LabelBox->setMaximumSize(QSize(16777215, 50));
        horizontalLayout_5 = new QHBoxLayout(LabelBox);
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        AuthorButton = new QRadioButton(LabelBox);
        AuthorButton->setObjectName("AuthorButton");

        horizontalLayout_5->addWidget(AuthorButton, 0, Qt::AlignLeft);

        TitleButton = new QRadioButton(LabelBox);
        TitleButton->setObjectName("TitleButton");

        horizontalLayout_5->addWidget(TitleButton);

        CoauthorButton = new QRadioButton(LabelBox);
        CoauthorButton->setObjectName("CoauthorButton");

        horizontalLayout_5->addWidget(CoauthorButton);

        KeyButton = new QRadioButton(LabelBox);
        KeyButton->setObjectName("KeyButton");

        horizontalLayout_5->addWidget(KeyButton);

        FuzzyButton = new QRadioButton(LabelBox);
        FuzzyButton->setObjectName("FuzzyButton");

        horizontalLayout_5->addWidget(FuzzyButton);


        gridLayout->addWidget(LabelBox, 3, 0, 1, 1);

        TextLine = new QHBoxLayout();
        TextLine->setObjectName("TextLine");
        lineEdit = new QLineEdit(centralwidget);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setMaximumSize(QSize(16777215, 25));
        lineEdit->setStyleSheet(QString::fromUtf8(""));
        lineEdit->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        TextLine->addWidget(lineEdit);

        fuzzy = new QCheckBox(centralwidget);
        fuzzy->setObjectName("fuzzy");

        TextLine->addWidget(fuzzy);

        searchButton = new QPushButton(centralwidget);
        searchButton->setObjectName("searchButton");
        searchButton->setMinimumSize(QSize(100, 25));
        searchButton->setMaximumSize(QSize(16777215, 25));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/search.png"), QSize(), QIcon::Normal, QIcon::Off);
        searchButton->setIcon(icon12);

        TextLine->addWidget(searchButton);


        gridLayout->addLayout(TextLine, 1, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 850, 17));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        menuSetting = new QMenu(menubar);
        menuSetting->setObjectName("menuSetting");
        menuFunction = new QMenu(menubar);
        menuFunction->setObjectName("menuFunction");
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName("menuHelp");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName("toolBar");
        toolBar->setContextMenuPolicy(Qt::CustomContextMenu);
        toolBar->setStyleSheet(QString::fromUtf8("color: rgb(100,100, 200);"));
        toolBar->setIconSize(QSize(30, 30));
        toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        MainWindow->addToolBar(Qt::LeftToolBarArea, toolBar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuSetting->menuAction());
        menubar->addAction(menuFunction->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuFile->addAction(action_Open);
        menuFile->addAction(action_Exit);
        menuSetting->addAction(action_Language);
        menuSetting->addAction(action_Status);
        menuFunction->addAction(action_Clear);
        menuFunction->addAction(action_AuthorCount);
        menuFunction->addAction(action_FuzzyOpen);
        menuFunction->addAction(action_GroupAnalyze);
        menuFunction->addAction(action_KeyWordCount);
        menuHelp->addAction(action_HowToUse);
        toolBar->addAction(action_Open);
        toolBar->addAction(action_Status);
        toolBar->addAction(action_Clear);
        toolBar->addAction(action_FuzzyOpen);
        toolBar->addAction(action_AuthorCount);
        toolBar->addAction(action_GroupAnalyze);
        toolBar->addAction(action_KeyWordCount);
        toolBar->addAction(action_HowToUse);
        toolBar->addAction(action_Language);
        toolBar->addAction(action_Exit);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "DBLP Little Browser", nullptr));
        action_HowToUse->setText(QCoreApplication::translate("MainWindow", "How To Use?", nullptr));
        action_Open->setText(QCoreApplication::translate("MainWindow", "Open", nullptr));
        action_Exit->setText(QCoreApplication::translate("MainWindow", "Exit", nullptr));
        action_Clear->setText(QCoreApplication::translate("MainWindow", "Clear", nullptr));
        actionAuthor->setText(QCoreApplication::translate("MainWindow", "Author of the Most Articles", nullptr));
        action_AuthorCount->setText(QCoreApplication::translate("MainWindow", "AuthorCount", nullptr));
        action_FuzzyOpen->setText(QCoreApplication::translate("MainWindow", "FuzzyOpen", nullptr));
        action_Language->setText(QCoreApplication::translate("MainWindow", "Language", nullptr));
        action_GroupAnalyze->setText(QCoreApplication::translate("MainWindow", "GroupAnalyze", nullptr));
        action_KeyWordCount->setText(QCoreApplication::translate("MainWindow", "KeyWordCount", nullptr));
        action_Status->setText(QCoreApplication::translate("MainWindow", "Status", nullptr));
        QTableWidgetItem *___qtablewidgetitem = Table_1->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "Title", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = Table_1->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Author", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = Table_1->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Modify Date", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = Table_1->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Publisher", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = Table_1->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "Key", nullptr));
        Label->setText(QString());
        QTableWidgetItem *___qtablewidgetitem5 = Table_2->horizontalHeaderItem(0);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "Title", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = Table_2->horizontalHeaderItem(1);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "Modify Date", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = Table_2->horizontalHeaderItem(2);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("MainWindow", "Key", nullptr));
        LabelBox->setTitle(QCoreApplication::translate("MainWindow", "Search By\357\274\232", nullptr));
        AuthorButton->setText(QCoreApplication::translate("MainWindow", "Author", nullptr));
        TitleButton->setText(QCoreApplication::translate("MainWindow", "Title", nullptr));
        CoauthorButton->setText(QCoreApplication::translate("MainWindow", "Coauthor", nullptr));
        KeyButton->setText(QCoreApplication::translate("MainWindow", "Key", nullptr));
        FuzzyButton->setText(QCoreApplication::translate("MainWindow", "Fuzzy", nullptr));
        fuzzy->setText(QCoreApplication::translate("MainWindow", "Fuzzy", nullptr));
        searchButton->setText(QCoreApplication::translate("MainWindow", "Search", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        menuSetting->setTitle(QCoreApplication::translate("MainWindow", "Setting", nullptr));
        menuFunction->setTitle(QCoreApplication::translate("MainWindow", "Function", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "Help", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
