#pragma once

#include <QObject>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QMap>

class QNetworkReply;
class QNetworkRequest;

typedef QMap< QByteArray, QByteArray > RawHeaderMap;

class HttpRequest :	public QObject
{
	Q_OBJECT

public:
	HttpRequest(QObject *parent = NULL);
	~HttpRequest();

	void setRawHeader(const QByteArray& headerName, const QByteArray& value);
	void clearRawHeader();

	QByteArray httpGet(const QString& url);

private:
	QNetworkAccessManager			m_NetworkManager;
	RawHeaderMap					m_HeaderMap;

	QNetworkReply*					m_pReply;
};

