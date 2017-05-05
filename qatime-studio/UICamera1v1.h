#ifndef UICAMERA1V1_H
#define UICAMERA1V1_H

#include <QWidget>
#include "ui_UICamera1v1.h"
#include "nlss_type.h"

class UICamera1v1 : public QWidget
{
	Q_OBJECT

public:
	UICamera1v1(QWidget *parent = 0);
	~UICamera1v1();

private:
	Ui::UICamera1v1 ui;

	QTimer*							m_refreshTimer;
	static ST_NLSS_VIDEO_SAMPLER	m_SvideoSampler;
	static QMutex					m_mutex;
	QPixmap*						m_pBkImage;			// ±³¾°
	bool							m_bEndVideo;
protected:
	virtual void paintEvent(QPaintEvent *);
	virtual void mouseDoubleClickEvent(QMouseEvent* e);
private slots:
	void slot_onRefreshTimeout();						// Ë¢ÐÂ½çÃæ
	void VideoCapture(const char* data, unsigned int iwidth, unsigned int iheight, unsigned int iSize);
	void setBkImage(QString qImg); 

public:
	void StartEndVideo(bool);
};

#endif // UICAMERA1V1_H
