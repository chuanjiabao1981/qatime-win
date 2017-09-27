#ifndef ZWEB_H
#define ZWEB_H

#include <QWidget>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QEventLoop>


class ZWeb : public QWidget
{
	Q_OBJECT

public:
	ZWeb(QWidget *parent = 0);
	~ZWeb();

public:
	QJsonObject		m_JsonObj;

private:
	QNetworkReply*					z_reply;
	QNetworkAccessManager			z_manager;
public:
	bool	GetURLInfo(QString strUrl, QJsonObject &urlValue, QString mToken);
	bool	PostURLInfo(QString strUrl, QJsonObject &urlValue, QString mToken);

	bool	PostURLInfoBySignal(QString strUrl, QByteArray strAppend, QString mToken);
	
	void	RequestError(QJsonObject &error);

public slots:
	void	slot_FinishedGet();
	void	slot_FinishedPost();
};

#endif