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
			QString("����ʱ��"),
			QString("URL��ַ�����볢�Թر�������´򿪣�"),
			QString("ȷ��"),
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
			QString("����ʱ��"),
			QString("������ʧ�ܣ�����ϵ����Ա��"),
			QString("ȷ��"),
			QString());
	}
}

void UIHomework::ShowPage(QString mContent, QString mType, QString mURL)
{
	QString mSignText = "������ҵ";
	if (mType == "LiveStudio::Homework")
	{
		mSignText = "������ҵ";
		ui.lbl_Picture->setStyleSheet("image: url(:/LoginWindow/images/Homework.png);");
	}
	else
	if (mType == "LiveStudio::Question")
	{
		mSignText = "��������";
		ui.lbl_Picture->setStyleSheet("image: url(:/LoginWindow/images/Question.png);");
	}
	else
	if (mType == "LiveStudio::Answer")
	{
		mSignText = "�ظ�����";
		ui.lbl_Picture->setStyleSheet("image: url(:/LoginWindow/images/Question.png);");
	}
	else
	if (mType == "Resource::File")
	{
		mSignText = "�ϴ��μ�";
		ui.lbl_Picture->setStyleSheet("image: url(:/LoginWindow/images/Courseware.png);");
	}
	ui.lbl_Content->setText(mContent);
	ui.lbl_Sign->setText(mSignText);
	m_URL = mURL;
}