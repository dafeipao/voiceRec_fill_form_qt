// webiatws.cpp
#include "webiatws.h"
#include <QDateTime>
#include <QTimeZone>
#include <QJsonDocument>
#include <QMessageAuthenticationCode>
#include <QBuffer>
#include <QUrlQuery>
#include <QtEndian>
#include<QJsonArray>
#include "mainwindow.h"

extern MainWindow *globalMainWindow;

WebIATWS::WebIATWS(QString m_hostUrl,QString m_apiKey,QString m_apiSecret,QString m_appId,QObject *parent) : QObject(parent)
{
    this->m_hostUrl=m_hostUrl;
    this->m_apiKey=m_apiKey;
    this->m_apiSecret=m_apiSecret;
    this->m_appId=m_appId;

    connect(&m_webSocket, &QWebSocket::connected, this, &WebIATWS::onConnected);
    connect(&m_webSocket, &QWebSocket::textMessageReceived,
            this, &WebIATWS::onTextMessageReceived);
    connect(&m_webSocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),
            this, &WebIATWS::onError);
}

void WebIATWS::startRecognition(const QByteArray& audioData)
{
    m_audioData = audioData;
    m_currentPos = 0;
    m_sending = false;

    QString url = generateAuthUrl();
    url.replace("https://", "wss://");
    m_webSocket.open(QUrl(url));
}

void WebIATWS::onConnected()
{
    m_sending = true;
    sendAudioFrame(StatusFirstFrame);
}

void WebIATWS::onTextMessageReceived(const QString& message)
{
    globalMainWindow->debug_rec("Recive Json"+message.toUtf8());

    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject json = doc.object();

    int code = json["code"].toInt();
    if(code != 0) {
        emit errorOccurred(QString("Code: %1, Message: %2")
                               .arg(code).arg(json["message"].toString()));
        return;
    }

    QJsonObject data = json["data"].toObject();
    QJsonObject result = data["result"].toObject();

    // qDebug()<<"JASON: "<<result;

    // 解析新的数据结构
    if(result.contains("ws")) {
        QString currentText;
        QJsonArray wsArray = result["ws"].toArray();
        foreach(const QJsonValue& wsValue, wsArray) {
            QJsonArray cwArray = wsValue.toObject()["cw"].toArray();
            foreach(const QJsonValue& cwValue, cwArray) {
                currentText += cwValue.toObject()["w"].toString();
            }
        }

        m_interimResult +=  currentText;

        if(data["status"].toInt() == 2) {
            emit finalResultReceived(m_interimResult);
            globalMainWindow->debug_rec("Recive text"+m_interimResult);
            m_interimResult.clear();
            m_webSocket.close();
        }
    }


}

void WebIATWS::onError(QAbstractSocket::SocketError error)
{
    globalMainWindow->debug_rec("ERROR:"+m_webSocket.errorString());
    emit errorOccurred(m_webSocket.errorString());
}

void WebIATWS::sendAudioFrame(FrameStatus status, const QByteArray& frame)
{
    globalMainWindow->debug_rec("sendAudioFrame");
    QJsonObject root;
    QJsonObject data;

    data["status"] = status;
    data["format"] = "audio/L16;rate=16000";
    data["encoding"] = "raw";
    data["audio"] = QString(frame.toBase64());

    if(status == StatusFirstFrame) {
        root["common"] = createCommonJson();
        root["business"] = createBusinessJson();
    }
    root["data"] = data;

    m_webSocket.sendTextMessage(QJsonDocument(root).toJson());

    if(status == StatusLastFrame) return;

    // 继续发送后续帧
    m_currentPos += m_frameSize;
    if(m_currentPos < m_audioData.size()) {
        QTimer::singleShot(40, this, [this]{
            QByteArray frame = m_audioData.mid(m_currentPos, m_frameSize);
            sendAudioFrame(StatusContinueFrame, frame);
        });
    } else {
        sendAudioFrame(StatusLastFrame);
    }
}

QString WebIATWS::generateAuthUrl()
{
    QDateTime now = QDateTime::currentDateTime().toUTC();
    QString date = now.toString("ddd, dd MMM yyyy HH:mm:ss 'GMT'");

    QString signatureOrigin = QString("host: %1\ndate: %2\nGET /v2/iat HTTP/1.1")
                                  .arg("iat-api.xfyun.cn").arg(date);

    QByteArray signature = hmacSha256(m_apiSecret.toUtf8(), signatureOrigin.toUtf8());
    QString authorization = QString("api_key=\"%1\", algorithm=\"%2\", headers=\"%3\", signature=\"%4\"")
                                .arg(m_apiKey, "hmac-sha256", "host date request-line", QString(signature));

    QUrl url(m_hostUrl);
    QUrlQuery query;
    query.addQueryItem("authorization", QByteArray(authorization.toUtf8()).toBase64());
    query.addQueryItem("date", date);
    query.addQueryItem("host", "iat-api.xfyun.cn");
    url.setQuery(query);

    return url.toString();
}

QJsonObject WebIATWS::createCommonJson()
{
    QJsonObject common;
    common["app_id"] = m_appId;
    return common;
}

QJsonObject WebIATWS::createBusinessJson()
{
    QJsonObject business;
    business["language"] = "zh_cn";
    business["domain"] = "iat";
    business["accent"] = "mandarin";
    // business["dwa"] = "wpgs";
    return business;
}

QByteArray WebIATWS::hmacSha256(const QByteArray &key, const QByteArray &data)
{
    QMessageAuthenticationCode hmac(QCryptographicHash::Sha256);
    hmac.setKey(key);
    hmac.addData(data);
    return hmac.result().toBase64();
}
