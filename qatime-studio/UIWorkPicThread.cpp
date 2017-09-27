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
#include <windows.h>
#include <QFile>

WorkerPic::WorkerPic() {
	workerThread.start();
	this->moveToThread(&workerThread);
	m_url = "";
	m_bClose = false;
}

WorkerPic::~WorkerPic()
{
	workerThread.requestInterruption();
	workerThread.quit();
	workerThread.wait();
}
void WorkerPic::SetUrl(QLabel* pic, QString url)
{
	m_pic = pic;
	m_url = url;
}


void WorkerPic::slot_StartLoading()
{
	bool mIsPicExist = false;
	for (int i = 0; i < m_mapUrl.size(); i++)
	{

		if (m_mapUrl.keys().at(i))
		{
			QLabel* pic = m_mapUrl.keys().at(i);
			QString url_ = m_mapUrl.values().at(i);

			if (!url_.isEmpty())
			{
				// ��ȡͼƬ����·��
				QStringList arr = url_.split("/");
				QString urlName = arr.last();

				QString picPath = "\\catch\\" + urlName;

				TCHAR szTempPath[MAX_PATH] = { 0 };
				GetCurrentDirectory(MAX_PATH, szTempPath);
				lstrcat(szTempPath, (LPCTSTR)picPath.utf16());

				QString path = QString::fromStdWString(szTempPath);
				QFile file(path);
				mIsPicExist = (file.exists() == true ? true : false);

				// ������ڣ���ֱ����ʾ
				if (mIsPicExist)
				{
					QPixmap pixmap;
					QPixmap scaledPixmap;
					pixmap = QPixmap(path);

					QSize pixSize(pic->width(), pic->height());
					scaledPixmap = pixmap.scaled(pixSize, Qt::IgnoreAspectRatio);
					pic->setPixmap(scaledPixmap);
				}
				else
				{
					QUrl url(url_);
					QNetworkAccessManager manager;
					QEventLoop loop;

					QNetworkReply *reply = manager.get(QNetworkRequest(url));
					//���������������ɺ��˳����¼�ѭ�� 
					QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
					//�������¼�ѭ�� 
					loop.exec();

					if (m_bClose)
						return;

					QByteArray jpegData = reply->readAll();
					QPixmap pixmap;
					QPixmap scaledPixmap;
					QSize pixSize(pic->width(), pic->height());
					// ���سɹ�����ʾ
					if (pixmap.loadFromData(jpegData))
					{
						scaledPixmap = pixmap.scaled(pixSize, Qt::IgnoreAspectRatio);
						scaledPixmap.save(path);
					}
					else // ������ʾ����ͼƬ
					{
						QString sUrl = "./images/teacherPhoto.png";
						pixmap = QPixmap(sUrl);
						scaledPixmap = pixmap.scaled(pixSize, Qt::IgnoreAspectRatio);
					}
					pic->setPixmap(scaledPixmap);
				}
			}
		}
	}

	m_mapUrl.clear();
}

void WorkerPic::slot_Close()
{
	m_bClose = true;
}