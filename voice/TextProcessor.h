#ifndef TEXTPROCESSOR_H
#define TEXTPROCESSOR_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QHash>
#include <QVector>
#include <QRegularExpression>

struct NameCandidate {
    QString name;
    int score;
    int startPos; // 新增：记录在输入文本中的起始位置
};

class TextProcessor : public QObject
{
    Q_OBJECT
public:
    explicit TextProcessor(const QString &filePath,QObject *parent);
    QString processText(const QString &mode, const QString &text);
    int m_currentPos = 0;

private:
    QStringList m_nameList;
    QHash<QString, QString> m_pinyinCache;
    QString getPinyinFromPython(const QString &text);
    QPair<QString, int> autoCorrectByPinyin(const QString &text, const QString &mode,bool flag);
    QStringList convertToPinyinList(const QString &text);
    QString processName(const QString &text);
    QString processGrade(const QString &text);
    QString processSingleDigit(const QString &text);
    QString findBestName(const QString &pinyin);
    int calculateSimilarity(const QString &s1, const QString &s2);
    QString getCachedPinyin(const QString &text);
    QStringList loadNamesFromExcel(const QString &filePath);
    QList<NameCandidate> evaluateNameCandidates(const QStringList &inputPinyin);
};

#endif // TEXTPROCESSOR_H
