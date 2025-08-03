#include "NetworkManager.h"
#include <QtCore/QDebug>
#include <QtCore/QTimer>
#include <QtCore/QUrl>
#include <QtCore/QUrlQuery>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

NetworkManager::NetworkManager(QObject *parent)
    : QObject(parent)
    , networkManager(new QNetworkAccessManager(this))
    , connectionTimer(new QTimer(this))
    , connected(false)
    , maxRetries(3)
    , currentRetries(0)
{
    setupNetworkManager();
    
    // Set default API endpoints
    searchApiUrl = "https://api.duckduckgo.com/";
    aiApiUrl = "https://api.openai.com/v1/chat/completions";
    codeApiUrl = "https://api.github.com/";
    
    userAgent = "AI-Assistant/1.0 (Qt6; C++)";
    
    // Setup connection checking
    connect(connectionTimer, &QTimer::timeout, this, &NetworkManager::checkConnectionStatus);
    connectionTimer->start(10000); // Check every 10 seconds
    
    // Initial connection check
    checkInternetConnection();
}

NetworkManager::~NetworkManager()
{
}

void NetworkManager::setupNetworkManager()
{
    // Connect network manager signals
    connect(networkManager, &QNetworkAccessManager::finished,
            this, &NetworkManager::onNetworkReplyFinished);
}

void NetworkManager::checkInternetConnection()
{
    // Try to connect to a reliable server
    QNetworkRequest request(QUrl("https://www.google.com"));
    request.setHeader(QNetworkRequest::UserAgentHeader, userAgent);
    request.setRawHeader("Connection", "close");
    
    QNetworkReply *reply = networkManager->head(request);
    pendingRequests[reply] = "connection_check";
    
    // Set timeout
    QTimer::singleShot(5000, this, [this, reply]() {
        if (reply && reply->isRunning()) {
            reply->abort();
        }
    });
}

bool NetworkManager::isConnected() const
{
    return connected;
}

void NetworkManager::setApiKey(const QString &key)
{
    apiKey = key;
}

void NetworkManager::searchWeb(const QString &query)
{
    if (!connected) {
        emit errorOccurred("Nie je pripojenie na internet");
        return;
    }
    
    // Use DuckDuckGo Instant Answer API
    QUrl url(searchApiUrl);
    QUrlQuery urlQuery;
    urlQuery.addQueryItem("q", query);
    urlQuery.addQueryItem("format", "json");
    urlQuery.addQueryItem("no_html", "1");
    urlQuery.addQueryItem("skip_disambig", "1");
    url.setQuery(urlQuery);
    
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, userAgent);
    
    QNetworkReply *reply = networkManager->get(request);
    pendingRequests[reply] = "web_search";
}

void NetworkManager::queryAI(const QString &prompt, const QString &context)
{
    if (!connected) {
        emit errorOccurred("Nie je pripojenie na internet");
        return;
    }
    
    if (apiKey.isEmpty()) {
        // Simulate AI response for demo purposes
        QTimer::singleShot(1000, this, [this, prompt]() {
            QString response = QString("Simulovaná AI odpoveď na: %1").arg(prompt);
            emit aiResponseReady(response);
        });
        return;
    }
    
    // Prepare OpenAI API request
    QJsonObject json;
    json["model"] = "gpt-3.5-turbo";
    
    QJsonArray messages;
    if (!context.isEmpty()) {
        QJsonObject contextMsg;
        contextMsg["role"] = "system";
        contextMsg["content"] = context;
        messages.append(contextMsg);
    }
    
    QJsonObject userMsg;
    userMsg["role"] = "user";
    userMsg["content"] = prompt;
    messages.append(userMsg);
    
    json["messages"] = messages;
    json["max_tokens"] = 1000;
    json["temperature"] = 0.7;
    
    QNetworkRequest request(QUrl(aiApiUrl));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::UserAgentHeader, userAgent);
    request.setRawHeader("Authorization", QString("Bearer %1").arg(apiKey).toUtf8());
    
    QJsonDocument doc(json);
    QNetworkReply *reply = networkManager->post(request, doc.toJson());
    pendingRequests[reply] = "ai_query";
}

void NetworkManager::downloadCode(const QString &repository)
{
    if (!connected) {
        emit errorOccurred("Nie je pripojenie na internet");
        return;
    }
    
    // GitHub API to get repository content
    QString apiUrl = QString("%1repos/%2/contents").arg(codeApiUrl, repository);
    
    QNetworkRequest request(QUrl(apiUrl));
    request.setHeader(QNetworkRequest::UserAgentHeader, userAgent);
    
    QNetworkReply *reply = networkManager->get(request);
    pendingRequests[reply] = "download_code";
}

void NetworkManager::uploadLearningData(const QJsonObject &data)
{
    if (!connected) {
        emit errorOccurred("Nie je pripojenie na internet");
        return;
    }
    
    // This would typically upload to your own server
    // For demo purposes, we'll just simulate success
    QTimer::singleShot(2000, this, [this]() {
        emit uploadComplete(true);
    });
}

void NetworkManager::makeRequest(const QUrl &url, const QString &method, const QJsonObject &data)
{
    if (!connected) {
        emit errorOccurred("Nie je pripojenie na internet");
        return;
    }
    
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, userAgent);
    
    QNetworkReply *reply = nullptr;
    
    if (method.toUpper() == "GET") {
        reply = networkManager->get(request);
    } else if (method.toUpper() == "POST") {
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        QJsonDocument doc(data);
        reply = networkManager->post(request, doc.toJson());
    } else if (method.toUpper() == "PUT") {
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        QJsonDocument doc(data);
        reply = networkManager->put(request, doc.toJson());
    } else if (method.toUpper() == "DELETE") {
        reply = networkManager->deleteResource(request);
    }
    
    if (reply) {
        pendingRequests[reply] = "custom_request";
    }
}

void NetworkManager::makePostRequest(const QUrl &url, const QJsonObject &data)
{
    makeRequest(url, "POST", data);
}

void NetworkManager::makeGetRequest(const QUrl &url)
{
    makeRequest(url, "GET");
}

void NetworkManager::onNetworkReplyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;
    
    QString requestType = pendingRequests.take(reply);
    
    if (reply->error() != QNetworkReply::NoError) {
        handleNetworkError(reply->error());
        reply->deleteLater();
        return;
    }
    
    QByteArray data = reply->readAll();
    
    if (requestType == "connection_check") {
        bool wasConnected = connected;
        connected = (reply->error() == QNetworkReply::NoError);
        
        if (wasConnected != connected) {
            emit connectionStatusChanged(connected);
        }
    }
    else if (requestType == "web_search") {
        QJsonObject results = parseResponse(data);
        emit searchResultsReady(results);
        
        // Also emit as general response
        QString resultText = formatSearchResults(results);
        emit responseReceived(resultText);
    }
    else if (requestType == "ai_query") {
        QJsonObject response = parseResponse(data);
        
        // Extract AI response from OpenAI format
        QString aiResponse = "Chyba pri spracovaní odpovede";
        if (response.contains("choices")) {
            QJsonArray choices = response["choices"].toArray();
            if (!choices.isEmpty()) {
                QJsonObject firstChoice = choices[0].toObject();
                if (firstChoice.contains("message")) {
                    QJsonObject message = firstChoice["message"].toObject();
                    aiResponse = message["content"].toString();
                }
            }
        }
        
        emit aiResponseReady(aiResponse);
    }
    else if (requestType == "download_code") {
        QString content = QString::fromUtf8(data);
        emit downloadComplete(content);
    }
    else if (requestType == "custom_request") {
        QString response = QString::fromUtf8(data);
        emit responseReceived(response);
    }
    
    reply->deleteLater();
}

void NetworkManager::onConnectionTimeout()
{
    connected = false;
    emit connectionStatusChanged(false);
}

void NetworkManager::checkConnectionStatus()
{
    checkInternetConnection();
}

void NetworkManager::handleNetworkError(QNetworkReply::NetworkError error)
{
    QString errorString;
    
    switch (error) {
        case QNetworkReply::ConnectionRefusedError:
            errorString = "Pripojenie odmietnuté";
            connected = false;
            break;
        case QNetworkReply::RemoteHostClosedError:
            errorString = "Vzdialený server ukončil pripojenie";
            break;
        case QNetworkReply::HostNotFoundError:
            errorString = "Server nenájdený";
            connected = false;
            break;
        case QNetworkReply::TimeoutError:
            errorString = "Vypršal časový limit";
            break;
        case QNetworkReply::OperationCanceledError:
            errorString = "Operácia zrušená";
            break;
        case QNetworkReply::SslHandshakeFailedError:
            errorString = "SSL chyba";
            break;
        case QNetworkReply::UnknownNetworkError:
            errorString = "Neznáma sieťová chyba";
            connected = false;
            break;
        default:
            errorString = QString("Sieťová chyba: %1").arg(static_cast<int>(error));
            break;
    }
    
    emit errorOccurred(errorString);
    emit connectionStatusChanged(connected);
}

QString NetworkManager::formatApiRequest(const QString &prompt, const QString &context)
{
    QJsonObject request;
    request["prompt"] = prompt;
    request["context"] = context;
    request["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    
    QJsonDocument doc(request);
    return doc.toJson(QJsonDocument::Compact);
}

QJsonObject NetworkManager::parseResponse(const QByteArray &data)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    
    if (error.error != QJsonParseError::NoError) {
        QJsonObject errorObj;
        errorObj["error"] = "JSON parse error";
        errorObj["details"] = error.errorString();
        return errorObj;
    }
    
    return doc.object();
}

QString NetworkManager::formatSearchResults(const QJsonObject &results)
{
    QString formatted = "Výsledky vyhľadávania:\n\n";
    
    // DuckDuckGo Instant Answer format
    if (results.contains("Abstract")) {
        QString abstract = results["Abstract"].toString();
        if (!abstract.isEmpty()) {
            formatted += "Súhrn: " + abstract + "\n\n";
        }
    }
    
    if (results.contains("AbstractText")) {
        QString abstractText = results["AbstractText"].toString();
        if (!abstractText.isEmpty()) {
            formatted += "Detaily: " + abstractText + "\n\n";
        }
    }
    
    if (results.contains("AbstractURL")) {
        QString url = results["AbstractURL"].toString();
        if (!url.isEmpty()) {
            formatted += "Zdroj: " + url + "\n\n";
        }
    }
    
    // Related topics
    if (results.contains("RelatedTopics")) {
        QJsonArray topics = results["RelatedTopics"].toArray();
        if (!topics.isEmpty()) {
            formatted += "Súvisiace témy:\n";
            for (int i = 0; i < qMin(3, topics.size()); ++i) {
                QJsonObject topic = topics[i].toObject();
                QString text = topic["Text"].toString();
                if (!text.isEmpty()) {
                    formatted += "- " + text + "\n";
                }
            }
        }
    }
    
    if (formatted == "Výsledky vyhľadávania:\n\n") {
        formatted = "Žiadne relevantné výsledky nenájdené.";
    }
    
    return formatted;
}

#include "NetworkManager.moc"