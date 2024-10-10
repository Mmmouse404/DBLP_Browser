#include "finder.h"
#include <QFile>
#include <QDataStream>
#include <QSettings>
std::unordered_map<QString, int> authorFreqMap;
Parser::StringRef *Finder::s_authorIndex = nullptr;
Parser::StringRef *Finder::s_titleIndex = nullptr;
Parser::StringRef *Finder::s_keyIndex = nullptr;
quint32 Finder::s_authorIndexs = 0;
quint32 Finder::s_titleIndexs = 0;
quint32 Finder::s_keyIndexs = 0;
QFile *Finder::s_file = nullptr;

Finder::Finder(QObject *parent)
    : QObject{parent}
{}

bool Finder::parsed()
{
    // 检查是否存在作者和标题的索引文件
    return QFile("author.dat").exists() && QFile("title.dat").exists()
           && QFile("key.dat").exists();
}

void Finder::clearIndex()
{
    // 清除作者和标题的索引
    delete s_authorIndex;
    delete s_titleIndex;
    delete s_keyIndex;
    s_authorIndex = nullptr;
    s_titleIndex = nullptr;
    s_keyIndex = nullptr;
}

QList<quint32> Finder::indexOfAuthor(const QString &author) const
{
    QList<quint32> list;
    if(s_authorIndex == nullptr) return list;
    // 查找特定作者名的索引位置并返回
    auto range = equalRange(s_authorIndex, s_authorIndex + s_authorIndexs, author);
    for(auto i = range.first; i != range.second; ++i){
        list.append(i->l);
    }
    return list;
}

QList<quint32> Finder::indexOfTitle(const QString &title) const
{
    QList<quint32> list;
    if(s_titleIndex == nullptr) return list;
    // 查找特定标题的索引位置并返回
    auto range = equalRange(s_titleIndex, s_titleIndex + s_titleIndexs, title);
    for(auto i = range.first; i != range.second; ++i){
        list.append(i->l);
    }
    return list;
}
QList<quint32> Finder::indexOfKey(const QString &kkey) const
{
    QList<quint32> list;
    if(s_keyIndex == nullptr) return list;
    // 查找特定标题的索引位置并返回
    auto range = equalRange(s_keyIndex, s_keyIndex + s_keyIndexs, kkey);

    for(auto i = range.first; i != range.second; ++i){
        list.append(i->l);
    }
    qDebug()<<"空："<<list.size();
    return list;
}

QString Finder::readText(const Parser::StringRef &ref)
{
    Q_ASSERT(s_file != nullptr);
    Q_ASSERT(s_file->isOpen());
    // 读取特定位置的文本内容并返回
    s_file->seek(ref.l);
    return s_file->read(ref.r - ref.l);
}

void Finder::init(){
    // 如果已经解析过索引则直接返回
    if(!parsed()) return ;
    // 读取并初始化作者索引
    QFile file("author.dat");
    file.open(QFile::ReadOnly);
    Q_ASSERT(file.isOpen());
    QDataStream stream(&file);
    s_authorIndexs = static_cast<quint32>(file.size() >> 3);
    s_authorIndex = new Parser::StringRef[s_authorIndexs];
    QSettings settings;
    Q_ASSERT(settings.contains("lastOpenFileName"));
    QString fileName = settings.value("lastOpenFileName").toString();
    for(quint32 i = 0; i < s_authorIndexs; ++i){
        stream >> s_authorIndex[i].l >> s_authorIndex[i].r;
    }

    file.close();
    // 读取并初始化标题索引
    file.setFileName("title.dat");
    file.open(QFile::ReadOnly);
    Q_ASSERT(file.isOpen());
    stream.setDevice(&file);
    s_titleIndexs = static_cast<quint32>(file.size() >> 3);
    s_titleIndex = new Parser::StringRef[s_titleIndexs];
    for(quint32 i = 0; i < s_titleIndexs; ++i){
        stream >> s_titleIndex[i].l >> s_titleIndex[i].r;
    }
    file.close();
    // 读取并初始化关键词索引
    file.setFileName("key.dat");
    file.open(QFile::ReadOnly);
    Q_ASSERT(file.isOpen());
    stream.setDevice(&file);
    s_keyIndexs = static_cast<quint32>(file.size() >> 3);
    s_keyIndex = new Parser::StringRef[s_keyIndexs];
    for(quint32 i = 0; i < s_keyIndexs; ++i){
        stream >> s_keyIndex[i].l >> s_keyIndex[i].r;
    }
    file.close();
    // 关闭已打开的文本文件并释放资源
    if(s_file != nullptr){
        if(s_file->isOpen()) s_file->close();
        delete s_file;
        s_file = nullptr;
    }

    s_file = new QFile(fileName);
    s_file->open(QFile::ReadOnly);
};

QPair<const Parser::StringRef *, const Parser::StringRef *>
Finder::equalRange(const Parser::StringRef *first,
                   const Parser::StringRef *last,
                   const QString &val)
{
    // 计算范围的长度
    quint32 len = static_cast<quint32>(last - first);
    while(len > 0){
        // 从范围的中间开始查找
        quint32 half = len >> 1;
        const Parser::StringRef *mid = first + half;
        // 读取中间值对应的文本
        QString text = readText(*mid);
        // 比较文本与目标值
        int cmp = QString::compare(text, val);
        if(cmp < 0){
            // 范围缩小为后半部分
            first = mid;
            ++first;
            len = len - half - 1;
        }else if(cmp > 0){
            // 范围缩小为前半部分
            len = half;
        }else{
            // 找到相等值的范围
            const Parser::StringRef *left = lowerBound(first, mid, val);
            first += len;
            const Parser::StringRef *right = upperBound(++mid, first, val);
            return qMakePair(left, right);
        }
    }
    // 未找到相等值，返回空范围
    return qMakePair(first, first);
}

const Parser::StringRef *
Finder::lowerBound(const Parser::StringRef *first,
                   const Parser::StringRef *last,
                   const QString &val)
{
    // 从范围的下界开始查找
    quint32 len = static_cast<quint32>(last - first);
    while(len > 0){
        // 从范围的中间开始查找
        quint32 half = len >> 1;
        const Parser::StringRef *mid = first + half;
        // 读取中间值对应的文本
        QString text = readText(*mid);
        int ret = QString::compare(text, val);
        if(ret < 0){
            // 范围缩小为后半部分
            first = mid;
            ++first;
            len = len - half - 1;
        }else{
            // 范围缩小为前半部分
            len = half;
        }
    }
    return first;
}

const Parser::StringRef *
Finder::upperBound(const Parser::StringRef *first,
                   const Parser::StringRef *last,
                   const QString &val)
{
    // 从范围的上界开始查找
    quint32 len = static_cast<quint32>(last - first);
    while(len > 0){
        // 从范围的中间开始查找
        quint32 half = len >> 1;
        const Parser::StringRef *mid = first + half;
        // 读取中间值对应的文本
        QString text = readText(*mid);
        int ret = QString::compare(val, text);
        if(ret < 0){
            // 范围缩小为前半部分
            len = half;
        }else{
            // 范围缩小为后半部分
            first = mid;
            ++first;
            len = len - half - 1;
        }
    }
    return first;
}


