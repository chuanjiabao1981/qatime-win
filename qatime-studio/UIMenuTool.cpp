#include "UIMenuTool.h"

UIMenuTool::UIMenuTool(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);	
	ui.Audio_checkBox->setStyleSheet("QCheckBox{spacing: 2px;color: white;}"
		"QCheckBox::indicator{width: 32px;height: 32px;}"
		"QCheckBox::indicator:unchecked{image: url(./images/mic_open.png);}"
		"QCheckBox::indicator:checked{image: url(./images/mic_close.png);}");
	// ��������ͷ��ƵԴ��ʽ
	ui.video_checkBox->setStyleSheet("QCheckBox{spacing: 2px;color: white;}"
		"QCheckBox::indicator{width: 32px;height: 32px;}"
		"QCheckBox::indicator:unchecked{image: url(./images/camera_close.png);}"
		"QCheckBox::indicator:checked{image: url(./images/camera_open.png);}");
	// ����ץȡȫ����ʽ
	ui.fullscreen_checkBox->setStyleSheet("QCheckBox{spacing: 2px;color: white;}"
		"QCheckBox::indicator{width: 32px;height: 32px;}"
		"QCheckBox::indicator:unchecked{image: url(./images/screen_close.png);}"
		"QCheckBox::indicator:checked{image: url(./images/screen_open.png);}");
	// ����ץȡ����Ӧ����ʽ
	ui.app_pushBtn->setStyleSheet("QCheckBox{spacing: 2px;color: white;}"
		"QCheckBox::indicator{width: 32px;height: 32px;}"
		"QCheckBox::indicator:unchecked{image: url(./images/app.png);}"
		"QCheckBox::indicator:checked{image: url(./images/app.png);}");
	ui.AudioCorner_pushBtn->setStyleSheet("QCheckBox{spacing: 2px;color: white;}"
		"QCheckBox::indicator{width: 16px;height: 32px;}"
		"QCheckBox::indicator:unchecked{image: url(./images/corner.png);}"
		"QCheckBox::indicator:checked{image: url(./images/corner.png);}"
		"QCheckBox::indicator:unchecked:pressed{image: url(./images/corner_push.png);}"
		"QCheckBox::indicator:checked:pressed{image: url(./images/corner_push.png);}");

	ui.videoCorner_pushBtn->setStyleSheet("QCheckBox{spacing: 2px;color: white;}"
		"QCheckBox::indicator{width: 16px;height: 32px;}"
		"QCheckBox::indicator:unchecked{image: url(./images/corner.png);}"
		"QCheckBox::indicator:checked{image: url(./images/corner.png);}"
		"QCheckBox::indicator:unchecked:pressed{image: url(./images/corner_push.png);}"
		"QCheckBox::indicator:checked:pressed{image: url(./images/corner_push.png);}");
	ui.fulsereen_checkBox->setStyleSheet("QCheckBox{spacing: 2px;color: white;}"
		"QCheckBox::indicator{width: 28px;height: 28px;}"
		"QCheckBox::indicator:unchecked{image: url(./images/fullScreen.png);}"
		"QCheckBox::indicator:checked{image: url(./images/fullQuit.png);}"
		"QCheckBox::indicator:unchecked:pressed{image: url(./images/fullScreen.png);}"
		"QCheckBox::indicator:checked:pressed{image: url(./images/fullQuit.png);}");
	connect(ui.fulsereen_checkBox, SIGNAL(stateChanged(int)), this, SLOT(slot_fulSereen(int)));
//	connect(ui.Live_pushBtn, SIGNAL(clicked()), this, SLOT(slot_startOrStopLiveStream()));
	connect(ui.Audio_checkBox, SIGNAL(stateChanged(int)), this, SLOT(AudioStatus(int)));
	connect(ui.video_checkBox, SIGNAL(stateChanged(int)), this, SLOT(VideoStatus(int)));
	connect(ui.fullscreen_checkBox, SIGNAL(stateChanged(int)), this, SLOT(FullScreenStatus(int)));
	connect(ui.app_pushBtn, SIGNAL(stateChanged(int)), this, SLOT(OtherApp(int)));
	connect(ui.AudioCorner_pushBtn, SIGNAL(stateChanged(int)), this, SLOT(clickChangeAudio(int)));
	connect(ui.videoCorner_pushBtn, SIGNAL(stateChanged(int)), this, SLOT(clickChangeVideo(int)));
	connect(ui.ratio_pushBtn, SIGNAL(clicked()), this, SLOT(clickChangeRatio()));
	connect(ui.lesson_pushButton, SIGNAL(clicked()), this, SLOT(clickLessonList()));
//	ui.time_label->hide();

	m_timers = new UITimers(this);
	m_timers->setWindowFlags(Qt::FramelessWindowHint);
	connect(m_timers, SIGNAL(emit_startOrStopLiveStream()), parent, SLOT(slot_startOrStopLiveStream()));
	m_timers->setTimeLabelVisible(false);
	m_timers->setParent(this);
}

UIMenuTool::~UIMenuTool()
{

}
void UIMenuTool::setFullEnabled()
{
	ui.fulsereen_checkBox->show();
}

void UIMenuTool::setRatioText(QString text)
{
	ui.ratio_pushBtn->setText(text);
}
void UIMenuTool::setTimeLabelVisible(bool b)
{
//	ui.time_label->setVisible(b);
	m_timers->setTimeLabelVisible(b);
}
void UIMenuTool::setTimeLabelText(QString text)
{
//	ui.time_label->setText(text);
	m_timers->setTimeLabelText(text);
}
void UIMenuTool::setLessonEnabled(bool bEnable)
{
	ui.app_pushBtn->setEnabled(bEnable);
}
void UIMenuTool::setAppEnabled(bool bEnable)
{
	ui.app_pushBtn->setEnabled(bEnable);
}
void UIMenuTool::setFullScreenEnabled(bool bEnable)
{
	ui.fullscreen_checkBox->setEnabled(bEnable);
}
void UIMenuTool::setFullScreenCheck(Qt::CheckState status)
{
	ui.fullscreen_checkBox->setCheckState(status);
}
bool UIMenuTool::getFullScreenIsChecked()
{
	return ui.fullscreen_checkBox->isChecked();
}
bool UIMenuTool::getisChecked()
{
	return ui.video_checkBox->isChecked();
}
void UIMenuTool::setVideoEnabled(bool bEnable)
{
	ui.video_checkBox->setEnabled(bEnable);
}
void UIMenuTool::setMenuCheckState(Qt::CheckState state)
{
	ui.video_checkBox->setCheckState(state);
}
void UIMenuTool::setVideoCheckState(Qt::CheckState state)
{
	ui.video_checkBox->setCheckState(state);
}
void UIMenuTool::setLivePushBtnText(bool bStart)
{
	//ui.Live_pushBtn->setText(text);
	m_timers->setLivePushBtnText(bStart);
}

QPoint UIMenuTool::getGeometry()
{
//	return ui.Audio_checkBox->geometry();
//	return ui.Audio_checkBox->mapTo(this->parent, ui.Audio_checkBox->pos());
//	return ui.Audio_checkBox->mapToParent(ui.Audio_checkBox->pos());
	QPoint point(this->size().width() / 800 * 180, 0);
	 return point;
}
//�����Ҳ������
void UIMenuTool::slot_fulSereen(int b)
{
	emit emit_FulSereen(b);
}

//��ʼֱ��
void UIMenuTool::slot_startOrStopLiveStream()
{
	emit emit_startOrStopLiveStream();
}
// ��Ƶ״̬��ֱ������ͣ�������Ŀ��ƣ�
void UIMenuTool::VideoStatus(int iStatus)
{
	emit emit_VideoStatus(iStatus);
}
// ����״̬��ֱ������ͣ�������Ŀ��ƣ�
void UIMenuTool::AudioStatus(int iStatus)
{
	emit emit_AudioStatus(iStatus);
}
// �л�ȫ����ƵԴ
void UIMenuTool::FullScreenStatus(int iStatus)
{
	emit emit_FullScreenStatus(iStatus);
}
// ����APPӦ��
void UIMenuTool::OtherApp(int i)
{
	emit emit_OtherApp(i);
}

void UIMenuTool::clickChangeAudio(int i)
{
	emit emit_clickChangeAudio(i);
}
// ��������ͷѡ���
void UIMenuTool::clickChangeVideo(int i)
{
	emit emit_clickChangeVideo(i);
}
// �����ֱ��ʿ�
void UIMenuTool::clickChangeRatio()
{
	emit emit_clickChangeRatio();
}
// �����γ̱�
void UIMenuTool::clickLessonList()
{
	emit emit_clickLessonList();
}

void UIMenuTool::moveLiveBtn()
{
	QRect rect = ui.time_label->geometry();
	m_timers->move(rect.left(), rect.top());
}

void UIMenuTool::InitMoveLiveBtn()
{
	m_timers->move(this->size().width()/2 - 60 +20, 16);
}