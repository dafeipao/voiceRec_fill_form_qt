#include "TextProcessor.h"
#include <algorithm>
#include "Python.h"
#include <QFile>
#include<QCoreApplication>
TextProcessor::TextProcessor(const QString &filePath,QObject *parent)
    : QObject(parent)
{
    this->m_nameList=loadNamesFromExcel(filePath);
}

QString TextProcessor::getPinyinFromPython(const QString &text) {
    // 初始化 Python 解释器
    QString path=QCoreApplication::applicationDirPath()+"/pinyin/";
    Py_SetPythonHome((wchar_t*)(reinterpret_cast<const wchar_t *>(path.utf16())));
    Py_Initialize();

    if (!Py_IsInitialized()) {
        qDebug() << "[db:] Py_Initialize fail";
        return "";
    }

    // qDebug() << "[db:] Py_Initialize success";

    // 导入 Python 模块
    PyObject *pName = PyUnicode_DecodeFSDefault("pinyin_script");  // Python 文件名 (pinyin_script.py)
    PyObject *pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (!pModule) {
        qDebug() << "[db:] Failed to load Python module";
        Py_Finalize();
        return "";
    }

    // 获取 convert_to_pinyin 函数
    PyObject *pFunc = PyObject_GetAttrString(pModule, "convert_to_pinyin");
    if (!pFunc || !PyCallable_Check(pFunc)) {
        qDebug() << "[db:] Function not found or not callable";
        Py_DECREF(pModule);
        Py_Finalize();
        return "";
    }

    // 转换 C++ QString 到 Python 字符串
    PyObject *pArg = PyUnicode_FromString(text.toUtf8().constData());

    // 调用 Python 函数
    PyObject *pValue = PyObject_CallFunctionObjArgs(pFunc, pArg, NULL);
    Py_DECREF(pArg);

    // 解析 Python 返回值
    QString result;
    if (pValue) {
        result = QString::fromUtf8(PyUnicode_AsUTF8(pValue));
        Py_DECREF(pValue);
    } else {
        qDebug() << "[db:] Function call failed";
    }

    // 释放 Python 资源
    Py_DECREF(pFunc);
    Py_DECREF(pModule);
    Py_Finalize();

    return result;
}


QString TextProcessor::processText(const QString &mode, const QString &text)
{

    if(m_currentPos>text.length()-2){
        return "END_FLAG";
    }
    // qDebug()<<"text.mid(m_currentPos)  "<<text.mid(m_currentPos);
    auto [cleanedText, consumed]  = autoCorrectByPinyin(text.mid(m_currentPos), mode,true);

    // cleanedText.remove(QRegularExpression("[\\p{P}\\p{S}]"));

    // qDebug()<<"cleanedText"<<cleanedText;


    if (mode == "姓名") {
        return processName(cleanedText);
    } else if (mode == "成绩") {
        return processGrade(cleanedText);
    } else if (mode == "1位数字") {
        return processSingleDigit(cleanedText);
    }

    return "";
}

bool isPunctuation(QChar c)
{
    static QRegularExpression re("\\p{P}");
    return re.match(c).hasMatch();
}

QList<NameCandidate> TextProcessor::evaluateNameCandidates(const QStringList &inputPinyin)
{
    QList<NameCandidate> candidates;
    const int MIN_SCORE = 5;
    const int INPUT_LENGTH = inputPinyin.size();

    foreach (const QString &name, m_nameList) {
        QStringList namePinyin = convertToPinyinList(name);
        const int nameLen = namePinyin.size();

        // 滑动窗口匹配
        for (int start = 0; start <= INPUT_LENGTH - nameLen; ++start) {
            int score = 0;
            bool fullMatch = true;

            // 逐字评分
            for (int i = 0; i < nameLen; ++i) {
                const QString &input = inputPinyin[start+i];
                const QString &target = namePinyin[i];

                // 权重系数：前两个字符（姓和名首字）加倍
                const int positionWeight = (i < 2) ? 2 : 1;

                // 首字母匹配得3分
                if (!input.isEmpty() && !target.isEmpty() &&
                    input[0].toLower() == target[0].toLower()) {
                    score += 3 * positionWeight;
                }
                // 全拼匹配额外得2分
                if (input.compare(target, Qt::CaseInsensitive) == 0) {
                    score += 2 * positionWeight;
                }
                // 长度差异超过1视为不匹配
                if (abs(input.length() - target.length()) > 1) {
                    fullMatch = false;
                    break;
                }
            }

            // 起始位置奖励：越靠前奖励越多（每提前一位加2分）
            int positionBonus = (INPUT_LENGTH - start) * 2;
            score += positionBonus;

            if (fullMatch && score >= MIN_SCORE) {
                candidates.append({name, score, start});
            }
        }
    }

    // 排序规则：
    // 1. 分数降序
    // 2. 同分数时起始位置升序
    // 3. 同位置时姓名长度降序
    std::sort(candidates.begin(), candidates.end(),
              [](const NameCandidate &a, const NameCandidate &b) {
                  if (a.score != b.score) return a.score > b.score;
                  if (a.startPos != b.startPos) return a.startPos < b.startPos;
                  return a.name.length() > b.name.length();
              });

    return candidates;
}

QPair<QString, int> TextProcessor::autoCorrectByPinyin(const QString &text, const QString &mode,bool flag)
{
    QString cleanedText;
    QVector<int> posMapping;

    for (int i = 0; i < text.length(); ++i) {
        if (!isPunctuation(text[i])) {
            cleanedText += text[i];
            posMapping.append(i);
        }
    }

    if (text.isEmpty()) return { "", 0 };

    if (mode == "姓名") {
        QStringList inputPinyin = convertToPinyinList(cleanedText);
        QList<NameCandidate> candidates = evaluateNameCandidates(inputPinyin);

        if (!candidates.isEmpty()) {
            const NameCandidate &best = candidates.first();

            // 计算实际消耗的原始字符数
            int consumed = 0;
            for (int i = 0; i < best.name.length(); ++i) {
                consumed += (cleanedText[i] == text[posMapping[i]] ? 1 : 0);
            }

            if(flag==true){
                m_currentPos+=posMapping[best.startPos + best.name.length() - 1] + 1;
            }
            return { best.name, posMapping[best.startPos + best.name.length() - 1] + 1 };

        }

        // 智能回退策略
        QString defaultName = m_nameList.isEmpty() ? cleanedText.left(2) : m_nameList.first();
        if(flag==true){
            m_currentPos+=defaultName.length();
        }


        return { defaultName, defaultName.length() };
    }
    else {
        QHash<QString, QString> pinyinNumMap = {
            {"ling", "0"}, {"yi", "1"}, {"er", "2"}, {"san", "3"},
            {"si", "4"}, {"wu", "5"}, {"liu", "6"}, {"qi", "7"},
            {"ba", "8"}, {"jiu", "9"}, {"shi", "10"}, {"yao", "1"},
            {"dian", "."}
        };


        QStringList tokens = convertToPinyinList(cleanedText);
        QString buffer;
        QString prevNum;
        int consumedChars = 0;  // 已处理的过滤后字符数
        int globalConsumed = 0; // 新增：原始文本中已处理的全局位置

        for (int tokenIndex = 0; tokenIndex < tokens.size(); ++tokenIndex) {
            const QString &pinyin = tokens[tokenIndex];
            const int originalCharPos = posMapping.value(tokenIndex, 0); // 获取该token对应的原始位置

            // 提前终止条件（保持原有逻辑）
            if ((mode == "成绩" && buffer.length() >= 2) ||
                (mode == "1位数字" && buffer.length() >= 1)) {
                globalConsumed = originalCharPos + 1; // +1 因为位置从0开始
                break;
            }

            if (pinyinNumMap.contains(pinyin)) {
                QString num = pinyinNumMap[pinyin];

                // 处理"十"的特殊情况
                if (num == "10") {
                    if (!prevNum.isEmpty()) {
                        buffer.chop(prevNum.length());
                        buffer += prevNum + "0";
                    }
                }else if (num == ".") {
                    buffer += ".";  // 处理小数点
                }else {
                    buffer += num;
                }

                // 每个token对应1个原始字符（汉字）
                globalConsumed = originalCharPos + 1; // 更新到当前token的结束位置
            }
            else if (pinyin.startsWith("num_")) {
                buffer += pinyin.mid(4);
                globalConsumed = originalCharPos + 1; // 数字字符对应1个原始位置
            }

            // 处理完当前token后更新consumedChars
            consumedChars = tokenIndex + 1;
        }

        // 最终consumed值取最大有效位置
        int consumed = globalConsumed;

        // 处理未进入循环的情况（空输入）
        if (consumedChars == 0 && !text.isEmpty()) {
            consumed = posMapping.value(0, 0) + 1; // 至少消耗第一个字符的位置
        }

        QString result = mode == "成绩" ? buffer.left(2) : buffer.left(1);
        if(flag==true){
            m_currentPos+=consumed-1;
        }
        return { result, consumed };
    }
}

QStringList TextProcessor::convertToPinyinList(const QString &text)
{
    QString fullPinyin = getCachedPinyin(text);
    fullPinyin.replace(QRegularExpression("(\\d)"), " num_\\1 ");
    return fullPinyin.split(" ", Qt::SkipEmptyParts);
}

QString TextProcessor::processName(const QString &text)
{
    QString pinyin = getCachedPinyin(text);
    QString result = findBestName(pinyin);
    // m_currentPos += result.length();
    return result;
}

QString TextProcessor::findBestName(const QString &pinyin)
{
    QString bestMatch;
    int maxScore = INT_MIN;

    for (const QString &name : m_nameList) {
        QString namePinyin = getCachedPinyin(name);
        int score = calculateSimilarity(pinyin, namePinyin);
        if (score > maxScore) {
            maxScore = score;
            bestMatch = name;
        }
    }
    return bestMatch;
}

int TextProcessor::calculateSimilarity(const QString &s1, const QString &s2)
{
    QStringList list1 = s1.split(" ");
    QStringList list2 = s2.split(" ");
    int score = 0;
    const int INITIAL_WEIGHT = 3;

    for (int i = 0; i < qMin(list1.size(), list2.size()); ++i) {
        QString p1 = list1[i];
        QString p2 = list2[i];

        // 首字母匹配权重
        if (p1.left(1) == p2.left(1)) {
            score += INITIAL_WEIGHT - i; // 位置越前权重越高
        }

        // 全拼音匹配
        if (p1 == p2) {
            score += 5;
        }
    }

    // 长度差异惩罚
    score -= qAbs(list1.size() - list2.size()) * 2;
    return score;
}

QString TextProcessor::processGrade(const QString &text)
{
    auto [numbers, consumed] = autoCorrectByPinyin(text, "成绩",false);
    QRegularExpression regex("^\\d+(\\.\\d+)?$");  // 匹配整数或带小数的数字
    if (regex.match(numbers).hasMatch()) {
        return numbers.left(4); // 保留最多4位数字（如98.5）
    }

    return numbers.left(2); // 默认处理整数（如 98）
}

QString TextProcessor::processSingleDigit(const QString &text)
{
    auto [numbers, consumed] = autoCorrectByPinyin(text, "1位数字",false);
    QString result = numbers.left(1);
    // m_currentPos += (result.isEmpty() ? 0 : 1);
    return result;
}

QString TextProcessor::getCachedPinyin(const QString &text)
{
    if (!m_pinyinCache.contains(text)) {
        m_pinyinCache[text] = getPinyinFromPython(text);
    }
    return m_pinyinCache[text];
}

QStringList TextProcessor::loadNamesFromExcel(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open text file!";
        return  {};
    }

    QByteArray fileData = file.readAll();  // 读取整个文件
    file.close();
    QStringList nameList;

    // 检测是否 UTF-8（有 BOM 或全 UTF-8 兼容格式）
    bool isUtf8 = fileData.startsWith("\xEF\xBB\xBF") || QString::fromUtf8(fileData).toUtf8() == fileData;

    QString content;
    if (isUtf8) {
        content = QString::fromUtf8(fileData);  // 直接转换 UTF-8
    } else {
        content = QString::fromLocal8Bit(fileData);  // 处理 GBK 编码
    }

    // 处理 BOM（去掉前3个字节的 UTF-8 BOM）
    if (content.startsWith(QChar(0xFEFF))) {
        content.remove(0, 1);
    }

    // 按换行符拆分（支持 Windows `\r\n`、Linux `\n`、Mac `\r`）
    QStringList lines = content.split(QRegularExpression("[\r\n]+"), Qt::SkipEmptyParts);

    nameList.clear();
    for (const QString &line : lines) {
        nameList.append(line.trimmed());
    }

    // qDebug() << "Loaded" << nameList << " names from text file.";
    return nameList;
}
