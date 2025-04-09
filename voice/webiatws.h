// webiatws.h
#ifndef WEBIATWS_H
#define WEBIATWS_H

#include <QObject>
#include <QWebSocket>
#include <QByteArray>
#include <QJsonObject>
#include <QTimer>

class WebIATWS : public QObject
{
    Q_OBJECT
public:
    enum FrameStatus {
        StatusFirstFrame = 0,
        StatusContinueFrame = 1,
        StatusLastFrame = 2
    };

    explicit WebIATWS(QString m_hostUrl,QString m_apiKey,QString m_apiSecret,QString m_appId,QObject *parent = nullptr);
    void startRecognition(const QByteArray& audioData);

signals:
    void finalResultReceived(const QString& result);
    void errorOccurred(const QString& error);

private slots:
    void onConnected();
    void onTextMessageReceived(const QString& message);
    void onError(QAbstractSocket::SocketError error);

private:
    void sendAudioFrame(FrameStatus status, const QByteArray& frame = QByteArray());
    QString generateAuthUrl();
    QJsonObject createCommonJson();
    QJsonObject createBusinessJson();
    QByteArray hmacSha256(const QByteArray& key, const QByteArray& data);
    QString m_interimResult;

    QWebSocket m_webSocket;
    QByteArray m_audioData;
    int m_currentPos = 0;
    const int m_frameSize = 1280; // 每帧大小
    bool m_sending = false;

    // 配置参数（需要用户自行设置）
    QString m_hostUrl ;
    QString m_apiKey ;
    QString m_apiSecret ;
    QString m_appId ;
};

#endif // WEBIATWS_H
