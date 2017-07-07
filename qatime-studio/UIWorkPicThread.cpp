/*
*	Author       hzzhuling15
*	Date         2015/09/11
*	Copyright    Hangzhou, Netease Inc.
*	Brief        
*   Notice       
*/
#include "UIWorkPicThread.h"
#include <QUrl>
#include <QNetworkAccessManager>
#include <QObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QPixmap>

WorkerPic::WorkerPic() {
	workerThread.start();
	this->moveToThread(&workerThread);
	m_url = "";
	m_bClose = false;
}

WorkerPic::~WorkerPic()
{
	
}
void WorkerPic::SetUrl(QLabel* pic, QString url)
{
	m_pic = pic;
	m_url = url;
}

void WorkerPic::slot_StartLoading()
{
	for (int i = 0; i < m_mapUrl.size(); i++)
	{
		if (m_mapUrl.keys().at(i))
		{
			QLabel* pic = m_mapUrl.keys().at(i);
			QString url_ = m_mapUrl.values().at(i);

			QUrl url(url_);
			QNetworkAccessManager manager;
			QEventLoop loop;

			QNetworkReply *reply = manager.get(QNetworkRequest(url));
			//请求结束并下载完成后，退出子事件循环 
			QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
			//开启子事件循环 
			loop.exec();

			if (m_bClose)
				return;

			QByteArray jpegData = reply->readAll();
			QPixmap pixmap;
			QPixmap scaledPixmap;
			QSize pixSize(pic->width(), pic->height());
			// 加载成功则显示
			if (pixmap.loadFromData(jpegData))
			{
				scaledPixmap = pixmap.scaled(pixSize, Qt::IgnoreAspectRatio);
			}
			else // 否则显示备用图片
			{
				QString sUrl = "./images/teacherPhoto.png";
				pixmap = QPixmap(sUrl);
				scaledPixmap = pixmap.scaled(pixSize, Qt::IgnoreAspectRatio);
			}
			pic->setPixmap(scaledPixmap);
		}
	}

	m_mapUrl.clear();
}

void WorkerPic::slot_Close()
{
	m_bClose = true;
}