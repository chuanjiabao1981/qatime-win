#include "UICamera1v1.h"
#include <QTimer>
#include <QPainter>
#include <QMutex>

QMutex UICamera1v1::m_mutex;
ST_NLSS_VIDEO_SAMPLER UICamera1v1::m_SvideoSampler;
UICamera1v1::UICamera1v1(QWidget *parent)
	: QWidget(parent)
	, m_pBkImage(NULL)
	, m_bEndVideo(false)
{
	ui.setupUi(this);

	m_refreshTimer = new QTimer(this);
	m_refreshTimer->start(1000 / 25);
	connect(m_refreshTimer, SIGNAL(timeout()), this, SLOT(slot_onRefreshTimeout()));

	m_SvideoSampler.iWidth = 0;
	m_SvideoSampler.iHeight = 0;
	m_SvideoSampler.puaData = NULL;//一次性分配一个最大的控件，不断填充不同的数据

	setBkImage("./iamges/camera_1.png");
}

UICamera1v1::~UICamera1v1()
{
	if (m_refreshTimer)
	{
		m_refreshTimer->stop();
		delete m_refreshTimer;
		m_refreshTimer = NULL;
	}
}

void UICamera1v1::slot_onRefreshTimeout()
{
	update();
}

void UICamera1v1::paintEvent(QPaintEvent*)
{
	if (this->isVisible())
	{
		QPainter p(this);
		p.setPen(QColor(0x8099be));

		if (m_SvideoSampler.iDataSize > 0 && !m_bEndVideo)
		{
			QImage qimage;
			m_mutex.lock();
			qimage = QImage((uchar*)m_SvideoSampler.puaData, m_SvideoSampler.iWidth, m_SvideoSampler.iHeight, QImage::Format_RGB32);

			QImage q = qimage.mirrored(false, true);
			p.drawImage(rect(), q);
			m_mutex.unlock();
		}
		else
		{
			if (NULL != m_pBkImage)
				p.drawPixmap(QPoint(0, 0), *m_pBkImage);
		}
	}
}

void UICamera1v1::VideoCapture(const char* data, unsigned int iwidth, unsigned int iheight, unsigned int iSize)
{
	m_mutex.lock();

	if (m_SvideoSampler.puaData)
		delete[]m_SvideoSampler.puaData;
	m_SvideoSampler.puaData = new unsigned char[iSize];

	m_SvideoSampler.iWidth = iwidth;
	m_SvideoSampler.iHeight = iheight;
	m_SvideoSampler.iDataSize = iSize;
	memcpy(m_SvideoSampler.puaData, data, iSize);

	m_mutex.unlock();
	delete data;
}

void UICamera1v1::setBkImage(QString qsImage)
{
	if (!qsImage.isEmpty())
	{
		if (NULL == m_pBkImage)
		{
			m_pBkImage = new QPixmap(qsImage);
		}
		else
		{
			m_pBkImage->load(qsImage);
		}
	}
}

void UICamera1v1::StartEndVideo(bool bEndVideo)
{
	m_bEndVideo = bEndVideo;
}

void UICamera1v1::mouseDoubleClickEvent(QMouseEvent* e)
{
	return;
	if (this->isFullScreen())
	{
		setWindowFlags(Qt::SubWindow);
		showNormal();
	}
	else
	{
		setWindowFlags(Qt::Window);
		showFullScreen();
	}
}