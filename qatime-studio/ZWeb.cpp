#include "ZWeb.h"
#include <QUrl>
#include "UIMessageBox.h"


ZWeb::ZWeb(QWidget *parent)

{
	
}

//post 请求
bool ZWeb::PostURLInfo(QString strUrl, QJsonObject &urlValue, QString mToken)
{
	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);
	QString str = mToken;
	QByteArray append("");
	QEventLoop mloop;
	request.setRawHeader("Remember-Token", mToken.toUtf8());

	z_reply = z_manager.post(request, append);
	//请求结束完成后，退出子事件循环 
	QObject::connect(z_reply, SIGNAL(finished()), &mloop, SLOT(quit()));
	//开启子事件循环
	mloop.exec();
	//下面这种信号槽的方式也行
	//connect(z_reply, &QNetworkReply::finished, this, &ZWeb::FinishedPost11);
	QByteArray mResult = z_reply->readAll();
	//QJsonObject urlValue;
	int mStatus = QJsonDocument::fromJson(mResult).object()["status"].toInt();
	//添加网络错误判断
	if (mStatus == 0)
	{
		RequestError(QJsonDocument::fromJson(mResult).object()["error"].toObject());
		return false;
	}
	QJsonDocument document(QJsonDocument::fromJson(mResult));
	urlValue = document.object();
	return true;
}


//post 信号槽请求
bool ZWeb::PostURLInfoBySignal(QString strUrl, QByteArray strAppend, QString mToken)
{
	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);
	QString str = mToken;
	
	request.setRawHeader("Remember-Token", mToken.toUtf8());

	z_reply = z_manager.post(request, strAppend);
	//下面这种信号槽的方式也行
	connect(z_reply, &QNetworkReply::finished, this, &ZWeb::slot_FinishedPost);
	return true;
}

void ZWeb::slot_FinishedPost()
{
	QByteArray mResult = z_reply->readAll();
	//QJsonObject urlValue;
	int mStatus = QJsonDocument::fromJson(mResult).object()["status"].toInt();
	//添加网络错误判断
	if (mStatus == 0)
	{
		RequestError(QJsonDocument::fromJson(mResult).object()["error"].toObject());
		return ;
	}
}



//get请求
bool ZWeb::GetURLInfo(QString strUrl, QJsonObject &urlValue, QString mToken)
{
	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);
	QString str = mToken;
	QEventLoop mloop;
	request.setRawHeader("Remember-Token", mToken.toUtf8());
	z_reply = z_manager.get(request);

	
	//请求结束完成后，退出子事件循环 
	QObject::connect(z_reply, SIGNAL(finished()), &mloop, SLOT(quit()));
	//开启子事件循环
	mloop.exec();


	QByteArray mResult = z_reply->readAll();
	int mStatus = QJsonDocument::fromJson(mResult).object()["status"].toInt();
	//添加网络错误判断
	if (mStatus == 0)
	{
		RequestError(QJsonDocument::fromJson(mResult).object()["error"].toObject());
		return false;
	}
	QJsonDocument mDocument(QJsonDocument::fromJson(mResult));
	urlValue = mDocument.object();
	return true;
}

void ZWeb::slot_FinishedGet()
{
	QByteArray mResult = z_reply->readAll();
	int mStatus = QJsonDocument::fromJson(mResult).object()["status"].toInt();
	//添加网络错误判断
	if (mStatus == 0)
	{
		RequestError(QJsonDocument::fromJson(mResult).object()["error"].toObject());
		return;
	}
	QJsonDocument mDocument(QJsonDocument::fromJson(mResult));
	m_JsonObj = mDocument.object();
}

void ZWeb::RequestError(QJsonObject &error)
{
	QString strError;
	int mErrorCode = 0;
	mErrorCode = error["code"].toInt();
	if (mErrorCode == 1002)
	{
		strError = QString("授权过期,请重新登录！");
	}
	else if (mErrorCode == 1003)
		strError = QString("没有权限访问！");
	else if (mErrorCode == 1004)
		strError = QString("授权失败,请重新登录！");
	else if (mErrorCode == 3001)
		strError = QString("参数错误,请重新登录！");
	else if (mErrorCode == 3002)
		strError = QString("数据不合法,请重新登录！");
	else if (mErrorCode == 4001)
		strError = QString("找不到资源,请重新登录！");
	else if (mErrorCode == 9999)
		strError = QString("服务器错误,请重新登录！");
	else
		strError = QString("未知错误！");

	qDebug() << __FILE__ << __LINE__ << "错误代码："<<mErrorCode;
	int iStatus = CMessageBox::showMessage(
		QString("答疑时间"),
		QString(strError),
		QString("确定"),
		QString());
}

ZWeb::~ZWeb()
{

}