
#ifndef FINDER_H
#define FINDER_H
#include <QObject>
#include "parser.h"
#include "recorder.h"
#include "util.h"
class Finder: public QObject
{
    Q_OBJECT
public:
    explicit Finder(QObject *parent);
    static void init();
    static bool parsed();
    static void clearIndex();
    QList<quint32> indexOfAuthor(const QString &author) const;
    QList<quint32> indexOfTitle(const QString &title) const;
    QList<quint32> indexOfKey(const QString &key) const;
private:
    static QFile *s_file;
    static Parser::StringRef *s_authorIndex;
    static Parser::StringRef *s_titleIndex;
    static Parser::StringRef *s_keyIndex;
    static quint32 s_authorIndexs;
    static quint32 s_titleIndexs;
    static quint32 s_keyIndexs;

    static QString readText(const Parser::StringRef &ref);

    static QPair<const Parser::StringRef*, const Parser::StringRef*>
    equalRange(const Parser::StringRef *first,
               const Parser::StringRef *last,
               const QString &val);
    static const Parser::StringRef*
    lowerBound(const Parser::StringRef *first,
               const Parser::StringRef *last,
               const QString &val);
    static const Parser::StringRef*
    upperBound(const Parser::StringRef *first,
               const Parser::StringRef *last,
               const QString &val);
};

#endif // FINDER_H
