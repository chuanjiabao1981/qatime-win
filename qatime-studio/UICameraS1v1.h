#ifndef UICameraS1v1_H
#define UICameraS1v1_H

#include <QWidget>
#include "ui_UICameraS1v1.h"
#include "nlss_type.h"

// 学生摄像头
class UICameraS1v1 : public QWidget
{
	Q_OBJECT

public:
	UICameraS1v1(QWidget *parent = 0);
	~UICameraS1v1();

private:
	Ui::UICameraS1v1 ui;

	QTimer*							m_refreshTimer;
	static ST_NLSS_VIDEO_SAMPLER	m_SvideoSampler;
	static QMutex					m_mutex;
	QPixmap*						m_pBkImage;			// 背景
	bool							m_bEndVideo;
protected:
	virtual void paintEvent(QPaintEvent *);
private slots:
	void slot_onRefreshTimeout();						// 刷新界面
	void VideoCapture(const char* data, unsigned int iwidth, unsigned int iheight, unsigned int iSize);
	void setBkImage(QString qImg); 

public:
	void StartEndVideo(bool);
};

#endif // UICameraS1v1_H
