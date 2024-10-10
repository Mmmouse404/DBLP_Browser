#include "parser.h"
#include "port.h"
#include <QFile>
#include <QXmlStreamReader>
#include <QDataStream>
#include <QTime>
#include <QDebug>
char *Parser::s_data;
QVector<QString>alltitle;
unordered_map<QString,QVector<int>>keypos;
Graph* g;

QString filterNonPunctuation(QString str) {
    QString result;
    for (QChar c : str) {
        if (!c.isPunct()) {
            result += c;//把中间标点空格忽略
        }
    }
    return result;
}

Parser::Parser(QObject *parent)
    : QThread{parent}
{
    condition=authorflag=titleflag=groupflag=keyflag=0;
    authorcount.resize(100);
}


void Parser::run()
{
    if(condition==0)
        parse();  // 调用parse函数进行解析
    else if(condition==1)
        parseAuthor();
    else if(condition==2)
        parseTitle();
    else if(condition==3)
        parseAuthorGroup();
    else if(condition==4)
        parseKeyWord();
}

void Parser::parseAuthorGroup(){
    if(groupflag==0){
        clique.clear();m_timing.start();  
        emit stateChanged(tr("Author Parsing start."));  // 发射状态变化信号，用于通知解析开始
        Q_ASSERT(!m_fileName.isEmpty());// 检查文件名是否为空
        QFile file(m_fileName);  // 根据文件名创建文件对象
        file.open(QFile::ReadOnly);  // 以只读模式打开文件
        Q_ASSERT(file.isOpen());  // 检查文件是否成功打开

        s_data = new char[static_cast<quint64>(file.size())];// 从文件中读取数据到字符数组s_data中
        quint32 len = static_cast<quint32>(file.read(s_data, file.size()));
        StringRef ref(0, len);  // 根据读取的数据创建一个StringRef对象
        file.close();  // 关闭文件
        emit stateChanged(tr("XML file read successful."));  // 发射状态变化信号，用于通知XML文件读取成功

        QVector<int> oneAuthorIndex;  // 用于存储一篇文章里作者索引的容器
        quint32 x = 0;int authorcount=0,verticlenum=1,edgenum=0;QSet<QPair<int, int>> edgeSet;

        g=init_graph(3600000,49826330<<1);qDebug()<<"g->"<<g->e_num<<" verticlenum:"<<g->v_num;
        if(!authorList.contains(" ")) authorList.insert(" ",0);

        while(x < len){
            if(ref[x] == '<'){
                if(ref.startsWith("title",x+1)){
                    authorcount=0;
                    oneAuthorIndex.clear();
                }
                else if(ref.startsWith("author", x + 1)){  // 如果遇到了<author>标签
                    authorcount++;
                    StringRef author = readElementText(ref, x);  // 从标签中读取文本索引
                    QString str = getInformation(author);
                    if(!authorList.contains(str)){
                        authorList.insert(str,verticlenum);
                        verticlenum++;
                    }
                    if(authorcount>1){
                        for(int i=authorcount-2;i>=0;i--){
                            if(!edgeSet.contains(QPair<int, int>(oneAuthorIndex[i], authorList[str]))){
                                /*
                                对于输入的一条边（u，v）
                                ++u; ++v;*/
                                int s=oneAuthorIndex[i];
                                int t=authorList[str];
                                ++s,++t;
                                add_edge_no_weight(g, s, t);
                                add_edge_no_weight(g, t, s);
                                edgeSet.insert(QPair<int, int>(oneAuthorIndex[i], authorList[str]));
                                edgeSet.insert(QPair<int, int>(authorList[str], oneAuthorIndex[i]));
                                edgenum+=2;
                            }
                        }
                    }
                    oneAuthorIndex.append(authorList[str]);
                }
            }
            ++x;
        }
        g->e_num=edgenum; g->v_num=verticlenum;
        double *C = (double *)calloc(verticlenum + 1, sizeof(double));
        Pivoter ::pivoter(g,C);
        qDebug()<<"edgenum:"<<edgenum<<" verticlenum:"<<verticlenum;
        int m1 = m_timing.elapsed();  // 计算解析所花费的时间
        emit stateChanged(tr("Graph Generate done. Cost time: %1").arg(Util::formatTime(m1)));

        delete[] s_data;  // 释放字符数组s_data的内存
        for (int i = 1; i <= 500; i++) {
            if(C[i]>0){
                QString::number(C[i], 'f', 1);
                emit stateChanged(tr("%1 clique number：%2 ").arg(i).arg(C[i]));  // 输出k阶子图数量
                clique.append(C[i]);
            }
        }
        m_costMsecs = m_timing.elapsed();  // 计算解析所花费的时间
        emit stateChanged(tr("AuthorGroup Parse done. Total Cost time: %1").arg(Util::formatTime(m_costMsecs)));
        groupflag=1;
        emit done();  // 发射解析完成信号
        }
    else if(groupflag==1){
        for(int i=0;i<clique.size();i++){
            emit stateChanged(tr("%1 clique number：%2 ").arg(i+1).arg(clique[i]));
        }
        emit done();
    }


}

void Parser::parseAuthor(){
    if(authorflag==0){
        m_timing.start();  // 开始计时
        emit stateChanged(tr("Author Group Parsing start.<br/>"));
        // 检查文件名是否为空
        Q_ASSERT(!m_fileName.isEmpty());
        QFile file(m_fileName);
        file.open(QFile::ReadOnly);
        Q_ASSERT(file.isOpen());
        s_data = new char[static_cast<quint64>(file.size())];
        quint32 len = static_cast<quint32>(file.read(s_data, file.size()));
        StringRef ref(0, len);
        file.close();
        emit stateChanged(tr("XML file read successful."));

        QVector<StringRef> authorcountIndex;  // 用于存储作者索引的容器
        quint32 x = 0;
        while(x < len){
            if(ref[x] == '<'){
                if(ref.startsWith("author", x + 1)){  // 如果遇到了<author>标签
                    StringRef author = readElementText(ref, x);  // 从标签中读取文本内容
                    authorcountIndex.append(author);  // 将读取的作者信息添加到作者索引容器中
                }
            }
            ++x;
        }
        emit stateChanged(tr("Index file sorting……"));
        // 使用 QHash 统计每个字符串的出现次数
        QHash<QString, int> countMap;
        for (const auto& ref : authorcountIndex) {
            QString str = QString::fromUtf8(s_data + ref.l, ref.r - ref.l);
            countMap[str]++;
        }
        // 使用 QMap 对计数结果进行排序
        QMap<int, QString> sortedMap;
        for (auto it = countMap.begin(); it != countMap.end(); it++) {
            sortedMap.insert(it.value(), it.key());
        }
        // 输出出现次数最多的前一百个字符串
        QMap<int, QString>::iterator it = sortedMap.end();
        int count=0;
        while (it != sortedMap.begin()&& count<100) {
            --it;
            authorcount[count].first=it.key();        authorcount[count].second=it.value();
            emit stateChanged(tr("No.%1 author: %2 count: %3 <br/>").arg(count+1).arg(it.value()).arg(it.key()));
            count++;
        }
        authorflag=1;

        delete[] s_data;  // 释放字符数组s_data的内存

        m_costMsecs = m_timing.elapsed();  // 计算解析所花费的时间
        emit stateChanged(tr("AuthorCount Parse done. Total Cost time: %1").arg(Util::formatTime(m_costMsecs))); 
        emit done();
    }
    else if(authorflag==1){
        for(int i=0;i<100;i++){
            emit stateChanged(tr("No.%1 author: %2 count: %3 <br/>").arg(i+1).arg(authorcount[i].second).arg(authorcount[i].first));
        }
        emit done();
    }

}

void Parser::parseTitle(){
    if(titleflag==0){
        m_timing.start();
        emit stateChanged(tr("Title Parsing start."));
        Q_ASSERT(!m_fileName.isEmpty());
        QFile file(m_fileName);
        file.open(QFile::ReadOnly);
        Q_ASSERT(file.isOpen());

        s_data = new char[static_cast<quint64>(file.size())];
        quint32 len = static_cast<quint32>(file.read(s_data, file.size()));
        StringRef ref(0, len);
        file.close();
        emit stateChanged(tr("XML file read successful."));
        quint32 x = 0;
        while(x < len){
            if(ref[x] == '<'){
                if(ref.startsWith("title", x + 1)){  // 如果遇到了<title>标签
                    static int titlepos=0;
                    StringRef title = readElementText(ref, x);  // 从标签中读取文本内容
                    QString tmp=getInformation(title);
                    alltitle.append(tmp);
                    QString filtered = filterNonPunctuation(tmp);
                    QStringList words = filtered.split(" ", Qt::SkipEmptyParts);
                    for (const QString& word : words) {keypos[lemma(word)].append(titlepos); }
                    titlepos++;
                }
            }
            ++x;
        }
        titleflag=1;
        delete[] s_data;  // 释放字符数组s_data的内存
        m_costMsecs = m_timing.elapsed();
        emit stateChanged(tr("Title Parse done. Total Cost time: %1").arg(Util::formatTime(m_costMsecs)));  // 发射状态变化信号，用于通知解析完成，并附带所花费的时间信息
        emit done();  // 发射解析完成信号
    }

}

void Parser::parseKeyWord(){
    QSet<QString> stopWords = { "for","in","of","on","to","an","us","with","de","and","the","by",
                               "from","as","without","towards","using","or","but","a","based","w"};
    if(keyflag==0){
        m_timing.start();
        emit stateChanged(tr("KeyWord Parsing start."));
        Q_ASSERT(!m_fileName.isEmpty());
        QFile file(m_fileName);
        file.open(QFile::ReadOnly);
        Q_ASSERT(file.isOpen());

        s_data = new char[static_cast<quint64>(file.size())];// 从文件中读取数据到字符数组s_data中
        quint32 len = static_cast<quint32>(file.read(s_data, file.size()));
        StringRef ref(0, len);  // 根据读取的数据创建一个StringRef对象
        file.close();  // 关闭文件
        unordered_map<int,QVector<QString>>keyWordIndex;
        emit stateChanged(tr("XML file read successful."));  // 发射状态变化信号，用于通知XML文件读取成功
        quint32 x = 0;

        while(x < len){
            if(ref[x] == '<'){
                if(ref.startsWith("title",x+1)){
                    StringRef title = readElementText(ref, x);  // 读取文本内容
                    QString tmptitle=getInformation(title);
                    QString filtered = filterNonPunctuation(tmptitle);
                    QStringList words = filtered.split(" ", Qt::SkipEmptyParts);
                    x++;
                    while(ref[x]!='<'|| !ref.startsWith("year",x+1))x++;
                    StringRef tmpyear = readElementText(ref, x);
                    int year=getInformation(tmpyear).toInt();
                    for (const QString& word : words) {
                        QString lowercaseWord = word.toLower(); // 将单词转换为小写
                        if(stopWords.find(lowercaseWord) == stopWords.end())
                            keyWordIndex[year].push_back((lowercaseWord));
                        if(keyWordIndex[year].size()%1000000==1)qDebug()<<lowercaseWord;
                    }
                }
            }
            ++x;
        }
        map<int, QVector<QString>> sortedKeyWordIndex(keyWordIndex.begin(), keyWordIndex.end());
        for (const auto& pair : sortedKeyWordIndex) {
            int year = pair.first;  // 获取年份
            const QVector<QString>& keywords = pair.second;  // 获取该年份对应的所有关键词
            // 使用 unordered_map 统计每个关键词的出现次数
            std::unordered_map<QString, int> keywordCount;
            for (const QString& keyword : keywords) {
                keywordCount[keyword]++;
                yearKeywordCountMap[year][keyword]++;
            }

            QVector<QPair<int, QString>> keywordcount_vector;
            // 将统计结果转换为 Qvector 以便排序
            for (const auto& countPair : keywordCount) {
                keywordcount_vector.push_back(std::make_pair(countPair.second, countPair.first));
            }

            // 对关键词按频率进行排序
            std::sort(keywordcount_vector.rbegin(), keywordcount_vector.rend());

            // 输出每年的前十名关键词
            int count = 0;
            for (const auto& countPair : keywordcount_vector) {
                if (count < 10) {
                    emit stateChanged(tr("Year:%1 No.%2 keyword: %3 count: %4 <br/>")
                                          .arg(year).arg(count + 1).arg(countPair.second).arg(countPair.first));
                    count++;
                } else {
                    break;
                }
            }
        }
        keyflag=1;
        delete[] s_data;  // 释放字符数组s_data的内存

        m_costMsecs = m_timing.elapsed();  // 计算解析所花费的时间
        emit stateChanged(tr("Title Parse done. Total Cost time: %1").arg(Util::formatTime(m_costMsecs)));  // 发射状态变化信号，用于通知解析完成，并附带所花费的时间信息
        emit done();  // 发射解析完成信号
    }
    else if (keyflag == 1) {
        // 在需要输出时遍历 yearKeywordCountMap 输出每年的前十个关键词
        for (auto& yearKeywords : yearKeywordCountMap) {
            int year = yearKeywords.first;
            std::unordered_map<QString, int>& keywords = yearKeywords.second;

            // 将 keywords 转换为 vector 以便排序
            std::vector<std::pair<int, QString>> countVector;
            for (const auto& countPair : keywords) {
                countVector.push_back(std::make_pair(countPair.second, countPair.first));
            }

            // 对关键词按频率进行排序
            std::sort(countVector.rbegin(), countVector.rend());

            // 输出每年的前十名关键词
            int count = 0;
            for (const auto& countPair : countVector) {
                if (count < 10) {
                    emit stateChanged(tr("Year: %1 No.%2 keyword: %3 count: %4 <br/>")
                                          .arg(year).arg(count + 1).arg(countPair.second).arg(countPair.first));
                    count++;
                } else {
                    break;
                }
            }
        }
        emit done();
    }

}


void Parser::parse()
{
    m_timing.start();
    emit stateChanged(tr("Parsing start."));

    // 检查文件名是否为空
    Q_ASSERT(!m_fileName.isEmpty());
    QFile file(m_fileName);
    file.open(QFile::ReadOnly);
    Q_ASSERT(file.isOpen());

    // 从文件中读取数据到字符数组s_data中
    s_data = new char[static_cast<quint64>(file.size())];
    quint32 len = static_cast<quint32>(file.read(s_data, file.size()));
    StringRef ref(0, len);  // 根据读取的数据创建一个StringRef对象
    file.close();  // 关闭文件
    emit stateChanged(tr("XML file read successful."));  // 发射状态变化信号，用于通知XML文件读取成功

    QVector<StringRef> authorIndex;  // 用于存储作者索引的容器
    QVector<StringRef> titleIndex;   // 用于存储标题索引的容器
    QVector<StringRef> keyIndex;   // 用于存储key索引的容器
    quint32 x = 0;
    while(x < len){
        if(ref[x] == '<'){
            if(ref.startsWith("author", x + 1)){  // 如果遇到了<author>标签
                StringRef author = readElementText(ref, x);  // 从标签中读取文本内容
                authorIndex.append(author);
            }else if(ref.startsWith("title", x + 1)){
                StringRef title = readElementText(ref, x);
                titleIndex.append(title);
            }
        }
        else if(ref[x]=='k'){
            if(ref.startsWith("key=",x)){
                StringRef key = readKeyElementText(ref, x);  // 读取文本内容
                keyIndex.append(key);  // 将读取的关键词信息添加到关键词索引容器中;
            }
        }
        ++x;
    }

    emit stateChanged(tr("XML file parse successful."));
    int m1 = m_timing.elapsed();
    emit stateChanged(tr("XML Parse Cost time: %1").arg(Util::formatTime(m1)));

    emit stateChanged(tr("Index file sorting……"));
    std::sort(authorIndex.begin(), authorIndex.end());
    std::sort(titleIndex.begin(), titleIndex.end());
    std::sort(keyIndex.begin(), keyIndex.end());
    int m2 = m_timing.elapsed();
    emit stateChanged(tr("Index sort Cost time: %1").arg(Util::formatTime(m2-m1)));

    emit stateChanged(tr("Index generated."));
    emit stateChanged(tr("Index file saving……"));
    delete[] s_data;

    // 写入作者索引到文件D:/bigwork/work/memory/author.dat中
    file.setFileName("author.dat");
    QDataStream stream(&file);
    file.open(QFile::WriteOnly);
    Q_ASSERT(file.isOpen());
    foreach(auto i, authorIndex){
        stream << i.l << i.r;
    }
    file.close();

    // 写入标题索引到文件D:/bigwork/work/memory/title.dat中
    file.setFileName("title.dat");
    stream.setDevice(&file);
    file.open(QFile::WriteOnly);
    Q_ASSERT(file.isOpen());
    foreach(auto i, titleIndex){
        stream << i.l << i.r;
    }
    file.close();

    // 写入标题索引到文件D:/bigwork/work/memory/key.dat中
    file.setFileName("key.dat");
    stream.setDevice(&file);
    file.open(QFile::WriteOnly);
    Q_ASSERT(file.isOpen());
    foreach(auto i, keyIndex){
        stream << i.l << i.r;
    }
    file.close();

    emit stateChanged(tr("Index file saved."));  // 发射状态变化信号，用于通知索引文件已保存

    m_costMsecs = m_timing.elapsed();  // 计算解析所花费的时间
    emit stateChanged(tr("Parse done. Total Cost time: %1").arg(Util::formatTime(m_costMsecs)));  // 发射状态变化信号，用于通知解析完成，并附带所花费的时间信息
    emit done();  // 发射解析完成信号
}

void Parser::clearIndex()
{
    QFile("author.dat").remove();  // 删除D:/bigwork/work/memory/author.dat文件
    QFile("title.dat").remove();  // 删除D:/bigwork/work/memory/title.dat文件
    QFile("key.dat").remove();  // 删除D:/bigwork/work/memory/title.dat文件
}



char &Parser::StringRef::operator[](quint32 x) const
{
    // 断言x的值在[l, r)的范围内
    Q_ASSERT(0 <= x && x < r - l);
    // 返回第x个元素的引用
    return s_data[l + x];
}

bool Parser::StringRef::operator<(const Parser::StringRef &s) const
{
    quint32 len = r - l;
    // 比较当前StringRef对象和s的长度，取较短的那个值
    if(s.r - s.l < len) len = s.r - s.l;
    // 逐个比较当前对象和s的字符，如果遇到不同的字符则返回对应位置字符的大小比较结果
    for(quint32 i = 0; i < len; i++){
        if(s_data[l + i] != s_data[s.l + i]){
            return s_data[l + i] < s_data[s.l + i];
        }
    }
    // 如果前len个字符相等，则返回长度较短的字符串作为小于
    if(len == s.r - s.l) return false;
    else return true;
}

Parser::StringRef Parser::StringRef::mid(quint32 pos) const
{
    // 断言pos在[l, r)的范围内
    Q_ASSERT(0 <= pos && pos < r - l);
    // 返回从pos开始直到末尾的字符串
    return StringRef(l + pos, r);
}

Parser::StringRef Parser::StringRef::mid(quint32 pos, quint32 len) const
{
    // 断言pos和pos+len在[l, r)的范围内
    Q_ASSERT(0 <= pos && pos < r - l);
    Q_ASSERT(pos + len <= r - l);
    // 返回从pos开始长度为len的子字符串
    return StringRef(l + pos, l + pos + len);
}

bool Parser::StringRef::startsWith(const char *str, quint32 from) const
{
    quint32 x = l + from;
    // 从位置x开始逐个比较str中的字符和当前字符串中的字符，如果有不同的则返回false，如果x溢出则返回false
    while(*str != 0){
        if(*str != s_data[x]) return false;
        ++str;
        ++x;
        if(x >= r) return false;
    }
    return true;
}

qint32 Parser::StringRef::indexOf(const char *str, quint32 from) const
{
    for(quint32 t = l + from; t < r; ++t){
        bool flag = true;
        // 从位置t开始逐个比较str中的字符和当前字符串中的字符
        for(const char *x = str; *x != 0; ++x){
            if(*x != s_data[t + x - str]){
                flag = false;
                break;
            }
        }
        // 如果匹配成功则返回匹配位置相对于l的偏移量
        if(flag){
            return static_cast<qint32>(t - l);
        }
    }
    // 如果没有找到匹配则返回-1
    return -1;
}


// 从输入字符串中提取 XML 元素的文本内容
Parser::StringRef Parser::readElementText(const Parser::StringRef &r, quint32 &from)
{
    // 从索引 'from' 开始提取子字符串 's'
    StringRef s = r.mid(from);

    // 确保子字符串的第一个字符是 '<'，否则触发断言失败
    Q_ASSERT(s[0] == '<');
    quint32 i = 1;
    char name[30];
    name[0] = '<';
    name[1] = '/';
    // 从第二个字符开始，找出元素的名称，并存储在 'name' 数组中，直到遇到空格或者 '>'
    while(s[i] != ' ' && s[i] != '>'){
        name[i + 1] = s[i];
        i++;
    }
    from += i;
    name[i + 1] = '>';
    name[i + 2] = 0;
    // 'name' 存储了结束标签的内容，如 "</ele>"

    // 继续遍历子字符串 's' 直到找到结束标签的 '>'，并在子字符串 's' 中查找结束标签 'name' 的位置
    while(s[i] != '>') i++;
    qint32 p = s.indexOf(name, i + 1);
    // 确保找到结束标签，否则触发断言失败
    Q_ASSERT(p != -1);
    // 计算结束标签在s字符串中的偏移，并更新 'from' 的值
    quint32 x = static_cast<quint32>(p);
    from += x + 1;
    // 返回开始标签和结束标签之间的文本内容
    return s.mid(i + 1, x - i - 1);
}

Parser::StringRef Parser::readKeyElementText(const Parser::StringRef &r, quint32 &from)
{
    // 从索引 'from' 开始提取子字符串 's'
    StringRef s = r.mid(from);
    // 确保子字符串的第一个字符是 'k'，否则触发断言失败
    Q_ASSERT(s[0] == 'k');
    quint32 i=0,x1,x2;
    // 从第二个字符开始，找出元素的名称，并存储在 'name' 数组中，直到遇到空格或者 '"'
    while(s[i] != '"'){
        i++;
    }
    i++;x1=i;
    while(s[i] != '"'){
            i++;
    }
    Q_ASSERT(s[i] == '"');
    x2=i;i++;
    from += i;
    return s.mid(x1, x2-x1);
}

int Parser::readYearElementText(const Parser::StringRef &r, quint32 &from)
{
    // 从索引 'from' 开始提取子字符串 's'
    StringRef s = r.mid(from);QString str="";
    // 确保子字符串的第一个字符是 'm'，否则触发断言失败
    Q_ASSERT(s[0] == 'm');
    quint32 i=0;
    // 从第二个字符开始，找出元素的名称，并存储在 'name' 数组中，直到遇到空格或者 '"'
    while(s[i] != '"'){
        i++;
    }
    i++;
    while(s[i] != '-'){
        str+=s[i];i++;
    }
    Q_ASSERT(s[i] == '-');
    i++;
    from += i;
    int year=str.toInt();
    // 计算结束标签在s字符串中的偏移，并更新 'from' 的值
    // 返回开始标签和结束标签之间的文本内容
    return year;
}
