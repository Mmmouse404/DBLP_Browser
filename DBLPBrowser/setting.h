#ifndef SETTING_H
#define SETTING_H

#include <QObject>

class Setting : public QObject
{
    Q_OBJECT
public:
    explicit Setting(QObject *parent = nullptr);
    ~Setting();
private slots:
    //设置应用槽函数被触发
    void applySettings();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();
signals:
};

#endif // SETTING_H
