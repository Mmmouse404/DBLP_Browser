#ifndef PARSER_H
#define PARSER_H


#include <QThread>
#include <QMap>
#include <QTime>
#include <QXmlStreamReader>
#include <QDebug>
#include <QElapsedTimer>
#include "util.h"
#include "pivoter.h"


class QFile;
    QString filterNonPunctuation(QString str);
class Parser : public QThread
{
    Q_OBJECT
public:
    Parser(QObject *parent = nullptr);
    QString fileName() const{return m_fileName;};
    int costMsecs(){return m_costMsecs;};
    void run() override;
    void parse();
    void parseAuthor();
    void parseTitle();
    void parseAuthorGroup();
    void parseKeyWord();
    void setFileName(const QString &fileName){m_fileName = fileName;}  // 设置文件名
    int condition;
    int authorflag,titleflag,groupflag,keyflag;
    QVector<QPair<int, QString>> authorcount;
    std::unordered_map<int, std::unordered_map<QString, int>> yearKeywordCountMap;
    static void clearIndex();


signals:
    void stateChanged(QString state);
    void done();

private:
    static char *s_data;

public:
    struct StringRef{
            quint32 l, r; // 指向 s_data[l..r) 的字符串范围

            StringRef(){}
            StringRef(quint32 l_, quint32 r_):l(l_), r(r_){}

            // 获取字符串引用中位置 x 处的字符
            char &operator[](quint32 x) const;

            // 比较当前 StringRef 是否小于另一个 StringRef
            bool operator<(const StringRef &s) const;

            // 获取字符串引用从位置 'pos' 开始到末尾的子串
            StringRef mid(quint32 pos) const;

            // 获取字符串引用从位置 'pos' 开始指定长度为 'len' 的子串
            StringRef mid(quint32 pos, quint32 len) const;

            // 检查字符串引用是否以给定子串从指定位置 'from' 开始
            bool startsWith(const char *str, quint32 from = 0) const;

            // 在字符串引用中从指定位置 'from' 开始查找给定子串的索引
            qint32 indexOf(const char *str, quint32 from = 0) const;

    };
    friend QDebug operator<<(QDebug debug, const StringRef &s){
        debug << QByteArray::fromRawData(s_data + s.l, static_cast<int>(s.r - s.l));
        return debug;
    }

    friend QString getInformation(StringRef s){
        QString str = QString::fromUtf8(s_data + s.l, static_cast<int>(s.r - s.l));
        return str;
    }
   // int groupcount;
   //QHash<int, QSet<int>> adjacencyList;
   QHash <QString,int> authorList;
    QVector<double> clique;
    // 自定义优先队列类
    class MyPriorityQueue {
    public:
        MyPriorityQueue() {}

        // 插入元素
        void push(const QPair<QString, int>& value) {
            data.push_back(value);
            std::push_heap(data.begin(), data.end(), compare);
        }

        // 弹出优先级最高的元素
        void pop() {
            std::pop_heap(data.begin(), data.end(), compare);
            data.pop_back();
        }

        // 获取优先级最高的元素
        std::pair<QString, int> top() const {
            return data.front();
        }

        // 获取队列大小
        size_t size() const {
            return data.size();
        }

    private:
        // 自定义比较函数对象
        struct Compare {
            bool operator()(const QPair<QString, int>& a, const QPair<QString, int>& b) const {
                return a.second > b.second; // 小顶堆
            }
        } compare;

        QVector<QPair<QString, int>> data; //存储数据的容器
    };

    // Trie 节点定义
    struct TrieNode {
        std::unordered_map<QChar, TrieNode*> children;
        bool isEnd;
        int count;  //用于统计词频

        TrieNode() : isEnd(false), count(0) {}
    };

    // Trie 类定义
    class Trie {
    public:
        Trie() {
            root = new TrieNode();
        }



        // 插入单词到 Trie 中，并统计词频
        void insert(const QString& word) {

            QString lowercaseWord = word.toLower(); // 将单词转换为小写
            TrieNode* current = root;
            for (const QChar& ch : lowercaseWord) {
                if (!current->children[ch]) {
                    current->children[ch] = new TrieNode();
                }
                current = current->children[ch];
            }
            current->isEnd = true;
            current->count++;  // 更新词频
        }

        // 检查单词是否在 Trie 中，并返回其出现频率
        int getWordFrequency(const QString& word) {
            TrieNode* current = root;
            for (const QChar& ch : word) {
                if (!current->children[ch]) {
                    return 0;  // 单词不存在
                }
                current = current->children[ch];
            }
            return current->isEnd ? current->count : 0;
        }

        // 找到出现频率前十的词
        QVector<QPair<QString, int>> topTenWords() {
            MyPriorityQueue maxHeap;
            topTenWordsHelper(root, "", maxHeap);

            QVector<QPair<QString, int>> result;
            while (maxHeap.size() > 0) {
                result.push_back(maxHeap.top());
                maxHeap.pop();
            }
            return result;
        }

        void traverseTrie() {
            traverseHelper(root, "");
        }

    private:
        TrieNode* root;

        // 辅助函数，用于递归删除所有节点
        void clear(TrieNode* node) {
            if (!node) return;
            for (auto& child : node->children) {
                clear(child.second);
            }
            delete node;
        }

        // 辅助函数，递归查找出现频率前十的词
        void topTenWordsHelper(TrieNode* node, QString word, MyPriorityQueue& maxHeap) {
            if (!node) return;

            if (node->isEnd) {
                maxHeap.push({word, node->count});
                if (maxHeap.size() > 10) {
                    maxHeap.pop();
                }
            }

            for (auto& child : node->children) {
                topTenWordsHelper(child.second, word + child.first, maxHeap);
            }
        }

        // 辅助函数，递归遍历字典树
        void traverseHelper(TrieNode* node, QString word) {
            if (!node) return;

            if (node->isEnd) {
                qDebug() << word.toStdString() << ": " << node->count;
            }

            for (auto& child : node->children) {
                traverseHelper(child.second, word + child.first);
            }
        }
    };

    QMap<int,Trie> Tries; //维护一个map存储不同年份对应的字典树
    Trie trie;

private:
    QString m_fileName;
    int m_costMsecs;
    QElapsedTimer m_timing;

    // treat child element as text, after reading, from equal to the
    // last position of the end element
    static StringRef readElementText(const StringRef &r, quint32 &from);
    static StringRef readKeyElementText(const StringRef &r, quint32 &from);
    static int readYearElementText(const StringRef &r, quint32 &from);
};
extern QVector<QString>alltitle;
extern std::unordered_map<QString,QVector<int>>keypos;
extern Graph* g;
#endif // PARSER_H
