#ifndef UICameraS1v1_H
#define UICameraS1v1_H

#include <QWidget>
#include "ui_UICameraS1v1.h"
#include "nlss_type.h"

// ѧ������ͷ
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
	QPixmap*						m_pBkImage;			// ����
	bool							m_bEndVideo;

	QWidget*						m_parentWnd;		// ������
	
	int								m_iWidth;			// ���ڿ�
	int								m_iHeight;			// ���ڸ�
protected:
	virtual void paintEvent(QPaintEvent *);
private slots:
	void slot_onRefreshTimeout();						// ˢ�½���
	void VideoCapture(const char* data, unsigned int iwidth, unsigned int iheight, unsigned int iSize);
	void setBkImage(QString qImg); 
	void StartEndVideo(bool);							// ��Ա�仯 true:�˳� false:����

public:
	void setParentWnd(QWidget* parent);
	void ScaleZoom(int iWidth, int iHeight);			// ����������ͼ��
};

#endif // UICameraS1v1_H
