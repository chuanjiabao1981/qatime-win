#include "UIAudioBar.h"
#include <QToolTip>

UIAudioBar::UIAudioBar(QWidget *parent)
	: QWidget(parent)
	, m_parent(NULL)
	, m_iSec(0)
	, m_timer(NULL)
	, m_bSend(false)
	, m_FailTimer(false)
{
	ui.setupUi(this);
	
	connect(ui.send_pushButton, SIGNAL(clicked()), this, SLOT(clickSend()));
	connect(ui.cancel_pushButton, SIGNAL(clicked()), this, SLOT(clickCancel()));

	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(slot_onTimeout()));

	m_FailTimer = new QTimer(this);
	connect(m_FailTimer, SIGNAL(timeout()), this, SLOT(slot_onFailTimeout()));
}

UIAudioBar::~UIAudioBar()
{

}

void UIAudioBar::setMainWindow(UIChatRoom* parent)
{
	m_parent = parent;
}

void UIAudioBar::clickSend()
{
	ui.time_label->setText("");
	ui.progressBar->setValue(0);
	m_bSend = true;
	hide();
	m_timer->stop();
	if (m_parent)
		m_parent->finishAudio();
}

void UIAudioBar::clickCancel()
{
	ui.time_label->setText("");
	ui.progressBar->setValue(0);
	m_bSend = false;
	hide();
	m_timer->stop();
	if (m_parent)
		m_parent->finishAudio();
}

void UIAudioBar::slot_onTimeout()
{
	m_iSec++;
	ui.progressBar->setValue(m_iSec);

	QString sTime = QString::number(m_iSec);
	sTime += "\"";
	ui.time_label->setText(sTime);

	// 60秒直接发送
	if (m_iSec == 60)
	{
		emit ui.send_pushButton->clicked();
		return;
	}
}

// 语音条开始执行动作
void UIAudioBar::CaptureAudio()
{
	m_FailTimer->stop();
	m_iSec = 0;
	ui.time_label->setText("");
	ui.progressBar->setValue(0);
	m_timer->start(1000);
}

bool UIAudioBar::IsSend()
{
	return m_bSend;
}

void UIAudioBar::slot_onFailTimeout()
{
	m_FailTimer->stop();
	QString time = ui.time_label->text();
	hide();
	if (time == "")
		QToolTip::showText(QCursor::pos(), "录制语音失败！");
}

void UIAudioBar::MonitorFail()
{
	m_FailTimer->stop();
	m_FailTimer->start(5000);
}