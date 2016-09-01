#include "uimainwindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include "course.h"
#include <QJsonArray>
#include <QRect>
#include "windows.h"

UIMainWindow::UIMainWindow(QWidget *parent)
	: QWidget(parent)
	, m_AuxiliaryInfo(NULL)
	, m_LessonInfo(NULL)
	, m_VideoInfo(NULL)
{
	ui.setupUi(this);

	connect(ui.mainmin_pushBtn, SIGNAL(clicked()), this, SLOT(MinDialog()));
	connect(ui.mainclose_pushBtn, SIGNAL(clicked()), this, SLOT(CloseDialog()));
	connect(ui.Auxiliary_pushBtn, SIGNAL(clicked()), this, SLOT(ShowAuxiliary()));
	connect(ui.Lesson_pushBtn, SIGNAL(clicked()), this, SLOT(ShowLesson()));
	connect(ui.expansion_pushBtn, SIGNAL(clicked()), this, SLOT(Expansion()));
	connect(ui.Live_pushBtn, SIGNAL(clicked()), this, SLOT(slot_startOrStopLiveStream()));
	connect(ui.videosource_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(VideoSourceChange(int)));
	connect(ui.app_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(VideoAppChange(int)));

	m_AuxiliaryInfo = new UIAuxiliary(this);
	m_AuxiliaryInfo->setWindowFlags(Qt::FramelessWindowHint);
	m_AuxiliaryInfo->hide();

	m_LessonInfo = new UILesson(this);
	m_LessonInfo->setWindowFlags(Qt::FramelessWindowHint);
	m_LessonInfo->hide();

	m_VideoInfo = new UIViedeo(this);
	m_VideoInfo->setWindowFlags(Qt::FramelessWindowHint);
	m_VideoInfo->move(165, 60);
	m_VideoInfo->hide();

	// ������ƵԴ
	QFile file("styles/ComboBox.qss");
	file.open(QFile::ReadOnly);
	QString styleSheet = file.readAll();
	ui.videosource_comboBox->setStyleSheet(styleSheet);
	ui.videosource_comboBox->addItem(QString("����ͷ"));
	ui.videosource_comboBox->addItem(QString("ȫ������"));
	ui.videosource_comboBox->addItem(QString("����APP"));
	ui.videosource_comboBox->setCurrentIndex(0);

	// ��������Ӧ��
	ui.app_comboBox->setStyleSheet(styleSheet);

	ui.Live_pushBtn->setText("����");
}

UIMainWindow::~UIMainWindow()
{
	if (m_AuxiliaryInfo)
	{
		delete m_AuxiliaryInfo;
		m_AuxiliaryInfo = NULL;
	}

	if (m_LessonInfo)
	{
		delete m_LessonInfo;
		m_LessonInfo = NULL;
	}
}

void UIMainWindow::MinDialog()
{
	showMinimized();
}

void UIMainWindow::CloseDialog()
{
	exit(0);
}

void UIMainWindow::setTeacherInfo(QJsonObject &data)
{
	int iID = data["id"].toInt();
	m_teacherID = QString::number(iID);

	// ������ʦ����
	QString teacherName = data["name"].toString();
	ui.teacherName_label->setText(teacherName);

	// ������ʦͷ��
	QString teacherPhoto_url = data["avatar_url"].toString();
	setNetworkPic(teacherPhoto_url);
}

void UIMainWindow::setRemeberToken(const QString &token)
{
	mRemeberToken = token;
}

void UIMainWindow::ShowAuxiliary()
{
	// �γ̴���
	if (m_LessonInfo->isVisible())
	{
		m_LessonInfo->hide();
	}

	// �����ര��
	if (m_AuxiliaryInfo->isVisible())
	{
		m_AuxiliaryInfo->hide();
		return;
	}

	if (m_AuxiliaryInfo->IsInit())
	{
		m_AuxiliaryInfo->show();
		return;
	}

	QString strUrl;
#ifdef _DEBUG
	strUrl = "http://testing.qatime.cn/api/v1/live_studio/teachers/id/courses";
	strUrl.replace(53, 2, m_teacherID);
#else
	strUrl = "http://qatime.cn/api/v1/live_studio/teachers/id/courses";
	strUrl.replace(45, 2, m_teacherID);
#endif

	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);
	QString str = this->mRemeberToken;

	request.setRawHeader("Remember-Token", this->mRemeberToken.toUtf8());
	reply = manager.get(request);
	connect(reply, &QNetworkReply::finished, this, &UIMainWindow::AuxiliaryRequestFinished);
}

void UIMainWindow::AuxiliaryRequestFinished()
{
	QByteArray result = reply->readAll();
	QJsonDocument document(QJsonDocument::fromJson(result));
	
	m_AuxiliaryInfo->setWindowFlags(Qt::FramelessWindowHint);
	m_AuxiliaryInfo->initCourses();
	m_AuxiliaryInfo->setAuxiliaryInfo(document.object());
	m_AuxiliaryInfo->move(QPoint(100,110));
	m_AuxiliaryInfo->show();
}

void UIMainWindow::ShowLesson()
{
	if (m_AuxiliaryInfo->isVisible())
	{
		m_AuxiliaryInfo->hide();
	}
	if (m_LessonInfo->isVisible())
	{
		m_LessonInfo->hide();
		return;
	}

	QString strUrl;
#ifdef _DEBUG
	strUrl = "http://testing.qatime.cn/api/v1/live_studio/teachers/id/courses/lessonid";
	strUrl.replace(64, 8, m_AuxiliaryInfo->getLessonID());
	strUrl.replace(53, 2, m_teacherID);
#else
	strUrl = "http://qatime.cn/api/v1/live_studio/teachers/id/courses/lessonid";
	strUrl.replace(56, 8, m_AuxiliaryInfo->getLessonID());
		strUrl.replace(45, 2, m_teacherID);
#endif

	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);
	QString str = this->mRemeberToken;

	request.setRawHeader("Remember-Token", this->mRemeberToken.toUtf8());
	reply = manager.get(request);
	connect(reply, &QNetworkReply::finished, this, &UIMainWindow::LessonRequestFinished);
}

void UIMainWindow::LessonRequestFinished()
{
	QByteArray result = reply->readAll();
	QJsonDocument document(QJsonDocument::fromJson(result));

	m_LessonInfo->setWindowFlags(Qt::FramelessWindowHint);
	m_LessonInfo->initLesson();
	m_LessonInfo->setLessonInfo(document.object());
	m_LessonInfo->move(QPoint(100, 240));
	m_LessonInfo->show();
}

void UIMainWindow::setNetworkPic(const QString &szUrl)
{
	QUrl url(szUrl);
	QNetworkAccessManager manager;
	QEventLoop loop;

	QNetworkReply *reply = manager.get(QNetworkRequest(url));
	//���������������ɺ��˳����¼�ѭ�� 
	QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	//�������¼�ѭ�� 
	loop.exec();

	QByteArray jpegData = reply->readAll();
	QPixmap pixmap;
	pixmap.loadFromData(jpegData);
	ui.teacherPhoto_Label->setPixmap(pixmap);
}

void UIMainWindow::Expansion()
{
	if (ui.teacherInfo_widget->isVisible())
	{
		ui.teacherInfo_widget->setVisible(false);
		ui.expansion_pushBtn->move(QPoint(10, 290));

		m_VideoInfo->move(20, 60);
		m_VideoInfo->resize(725, 560);
	}
	else
	{
		ui.teacherInfo_widget->setVisible(true);
		ui.expansion_pushBtn->move(QPoint(145, 290));

		m_VideoInfo->move(165, 60);
		m_VideoInfo->resize(580, 560);
	}	
}

void UIMainWindow::mousePressEvent(QMouseEvent *e)
{
	last = e->globalPos();
}

void UIMainWindow::mouseMoveEvent(QMouseEvent *e)
{
	int dx = e->globalX() - last.x();
	int dy = e->globalY() - last.y();

	last = e->globalPos();
	move(x() + dx, y() + dy);
}

void UIMainWindow::mouseReleaseEvent(QMouseEvent *e)
{
	int dx = e->globalX() - last.x();
	int dy = e->globalY() - last.y();

	move(x() + dx, y() + dy);
}

void UIMainWindow::slot_startOrStopLiveStream()
{
	if (m_VideoInfo)
	{
		if (m_VideoInfo->IsCurrentPreview())
		{
			ui.Live_pushBtn->setText("����");
			ui.videosource_comboBox->setEnabled(true);
//			ui.app_comboBox->setEnabled(true);
			m_VideoInfo->StopLiveVideo();
		}
		else
		{
			ui.Live_pushBtn->setText("ֹͣ");
			ui.videosource_comboBox->setEnabled(false);
//			ui.app_comboBox->setEnabled(false);
			m_VideoInfo->StartLiveVideo();
		}

		ui.video_widget->hide();
		m_VideoInfo->show();
	}
}

void UIMainWindow::VideoSourceChange(int index)
{
	switch (index)
	{
	case 0:
		m_VideoInfo->m_videoSourceType = EN_NLSS_VIDEOIN_CAMERA;
		break;
	case 1:
		m_VideoInfo->m_videoSourceType = EN_NLSS_VIDEOIN_FULLSCREEN;
		break;
	case 2:
		{
			m_VideoInfo->m_videoSourceType = EN_NLSS_VIDEOIN_APP;
			SetSourceAppPath();
			break;
		}
	default:
		break;
	}
}

void UIMainWindow::VideoAppChange(int index)
{
	if (m_VideoInfo)
	{
		m_VideoInfo->ChangeAppPath(index);
	}
}

void UIMainWindow::SetSourceAppPath()
{
	for (int i = 0; i < m_VideoInfo->m_iAppWindNum; i++)
	{
		QString qStrPath = m_VideoInfo->m_pAppWinds[i].paFriendlyName;
		ui.app_comboBox->addItem(qStrPath);
	}
	
	if (m_VideoInfo->m_iAppWindNum != 0)
	{
		ui.app_comboBox->setCurrentIndex(0);
	}
}