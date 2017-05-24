#include "HttpRequest.h"

#include <QEventLoop>
#include <QNetworkReply>

HttpRequest::HttpRequest(QObject *parent) 
	: QObject(parent)
	, m_pReply(NULL)
{
}


HttpRequest::~HttpRequest()
{
}

void HttpRequest::setRawHeader(const QByteArray& headerName, const QByteArray& value)
{
	m_HeaderMap.insert(headerName, value);
}

void HttpRequest::clearRawHeader()
{
	m_HeaderMap.clear();
}

QByteArray HttpRequest::httpGet(const QString& url)
{
	QNetworkRequest	request(url);

	RawHeaderMap::const_iterator itr = m_HeaderMap.begin();
	for (; itr != m_HeaderMap.end(); itr++)
	{
		request.setRawHeader(itr.key(), itr.value());
	}

	m_pReply = m_NetworkManager.get( request );

	QEventLoop eventLoop;
	connect(m_pReply, SIGNAL(finished()), &eventLoop, SLOT(quit())); 
	eventLoop.exec();

	return m_pReply->readAll();
}