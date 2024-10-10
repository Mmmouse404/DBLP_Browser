#ifndef PARSERDIALOG_H
#define PARSERDIALOG_H

#include <QObject>
#include <QDialog>
QT_BEGIN_NAMESPACE
namespace Ui {
class parserDialog;
}
QT_END_NAMESPACE
class ParserDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ParserDialog(QWidget *parent = nullptr);
    ~ParserDialog();
    void showStatus(const QString &msg);
    void clear();
    void activeButton();

private slots:
    void on_closeButton_clicked();

private:
    Ui::parserDialog *ui;
signals:
};

#endif // PARSERDIALOG_H
