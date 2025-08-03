#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtCore/QTimer>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

class NetworkManager : public QObject
{
    Q_OBJECT

public:
    explicit NetworkManager(QObject *parent = nullptr);
    ~NetworkManager();
    
    // Connection management
    void checkInternetConnection();
    bool isConnected() const;
    void setApiKey(const QString &key);
    
    // API calls
    void searchWeb(const QString &query);
    void queryAI(const QString &prompt, const QString &context = "");
    void downloadCode(const QString &repository);
    void uploadLearningData(const QJsonObject &data);
    
    // Custom requests
    void makeRequest(const QUrl &url, const QString &method = "GET", 
                    const QJsonObject &data = QJsonObject());
    void makePostRequest(const QUrl &url, const QJsonObject &data);
    void makeGetRequest(const QUrl &url);

signals:
    void connectionStatusChanged(bool connected);
    void responseReceived(const QString &response);
    void searchResultsReady(const QJsonObject &results);
    void aiResponseReady(const QString &response);
    void downloadComplete(const QString &content);
    void uploadComplete(bool success);
    void errorOccurred(const QString &error);

private slots:
    void onNetworkReplyFinished();
    void onConnectionTimeout();
    void checkConnectionStatus();

private:
    void setupNetworkManager();
    void handleNetworkError(QNetworkReply::NetworkError error);
    QString formatApiRequest(const QString &prompt, const QString &context);
    QJsonObject parseResponse(const QByteArray &data);
    
    QNetworkAccessManager *networkManager;
    QTimer *connectionTimer;
    
    bool connected;
    QString apiKey;
    QString userAgent;
    
    // API endpoints
    QString searchApiUrl;
    QString aiApiUrl;
    QString codeApiUrl;
    
    // Request tracking
    QMap<QNetworkReply*, QString> pendingRequests;
    int maxRetries;
    int currentRetries;
};

#endif // NETWORKMANAGER_H