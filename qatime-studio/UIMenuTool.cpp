#include "UIMenuTool.h"

UIMenuTool::UIMenuTool(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.Live_pushBtn->setStyleSheet("QPushButton{background-color:white;color: red;border-radius: 10px; }"
		"QPushButton{border: 2px groove gray; border - style: outset;}"
		"QPushButton:pressed{border-style: inset; }");
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

	connect(ui.Live_pushBtn, SIGNAL(clicked()), this, SLOT(slot_startOrStopLiveStream()));
	connect(ui.Audio_checkBox, SIGNAL(stateChanged(int)), this, SLOT(AudioStatus(int)));
	connect(ui.video_checkBox, SIGNAL(stateChanged(int)), this, SLOT(VideoStatus(int)));
	connect(ui.fullscreen_checkBox, SIGNAL(stateChanged(int)), this, SLOT(FullScreenStatus(int)));
	connect(ui.app_pushBtn, SIGNAL(stateChanged(int)), this, SLOT(OtherApp(int)));
	connect(ui.AudioCorner_pushBtn, SIGNAL(stateChanged(int)), this, SLOT(clickChangeAudio(int)));
	connect(ui.videoCorner_pushBtn, SIGNAL(stateChanged(int)), this, SLOT(clickChangeVideo(int)));
	connect(ui.ratio_pushBtn, SIGNAL(clicked()), this, SLOT(clickChangeRatio()));
	connect(ui.lesson_pushButton, SIGNAL(clicked()), this, SLOT(clickLessonList()));
}

UIMenuTool::~UIMenuTool()
{

}
void UIMenuTool::setRatioText(QString text)
{
	ui.ratio_pushBtn->setText(text);
}
void UIMenuTool::setTimeLabelVisible(bool b)
{
	ui.time_label->setVisible(b);
}
void UIMenuTool::setTimeLabelText(QString text)
{
	ui.time_label->setText(text);
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
void UIMenuTool::setFullScreenCheck(bool bEnable)
{
	ui.fullscreen_checkBox->setCheckState(Qt::CheckState::Unchecked);
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
void UIMenuTool::setLivePushBtnText(QString text)
{
	ui.Live_pushBtn->setText(text);
}

QPoint UIMenuTool::getGeometry()
{
//	return ui.Audio_checkBox->geometry();
//	return ui.Audio_checkBox->mapTo(this->parent, ui.Audio_checkBox->pos());
//	return ui.Audio_checkBox->mapToParent(ui.Audio_checkBox->pos());
	QPoint point(this->size().width() / 800 * 180, 0);
	 return point;
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
