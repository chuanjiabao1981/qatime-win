#include "UIHomework.h"
#include "QDebug"

UIHomework::UIHomework(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
	ui.lbl_Content->setStyleSheet("background-color:rgb(241,241,241);color:rgb(102, 102, 102)");
	ui.lbl_Sign->setStyleSheet("background-color:rgb(241,241,241);color:rgb(153, 153, 153)");
	ui.lbl_TopSpace->setText("");
	ui.lbl_BottomSpace->setText("");
	ui.lbl_LeftSpace->setText("");
	ui.lbl_RightSpace->setText("");
	ui.lbl_Picture->setText("");
	ui.VL_Content->setSpacing(0);

	m_URL = "";
}

UIHomework::~UIHomework()
{

}

void UIHomework::mousePressEvent(QMouseEvent *event)
{
	setFocus();
	bool bOpen = false;
	if (m_URL == "")
	{
		CMessageBox::showMessage(
			QString("答疑时间"),
			QString("URL网址有误，请尝试关闭软件重新打开！"),
			QString("确定"),
			QString());
		return;
	}
	try
	{
		bOpen = QDesktopServices::openUrl(QUrl(m_URL));
	}
	catch (...)
	{
		qDebug() << __FILE__<< __LINE__ << m_URL;
		bOpen = false;
	}
	if (bOpen == false)
	{
		CMessageBox::showMessage(
			QString("答疑时间"),
			QString("打开链接失败，请联系管理员！"),
			QString("确定"),
			QString());
	}
}

void UIHomework::ShowPage(QString mContent, QString mType, QString mURL)
{
	QString mSignText = "发布作业";
	if (mType == "LiveStudio::Homework")
	{
		mSignText = "发布作业";
		ui.lbl_Picture->setStyleSheet("image: url(:/LoginWindow/images/Homework.png);");
	}
	else
	if (mType == "LiveStudio::Question")
	{
		mSignText = "创建提问";
		ui.lbl_Picture->setStyleSheet("image: url(:/LoginWindow/images/Question.png);");
	}
	else
	if (mType == "LiveStudio::Answer")
	{
		mSignText = "回复提问";
		ui.lbl_Picture->setStyleSheet("image: url(:/LoginWindow/images/Question.png);");
	}
	else
	if (mType == "Resource::File")
	{
		mSignText = "上传课件";
		ui.lbl_Picture->setStyleSheet("image: url(:/LoginWindow/images/Courseware.png);");
	}
	ui.lbl_Content->setText(mContent);
	ui.lbl_Sign->setText(mSignText);
	m_URL = mURL;
}