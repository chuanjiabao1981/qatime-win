#ifndef UICHATROOM_H
#define UICHATROOM_H

#include <QWidget>
#include "ui_UIChatRoom.h"
#include "myemotionwidget.h"
#include <QLabel>
class UIChatRoom : public QWidget
{
	Q_OBJECT

public:
	UIChatRoom(QWidget *parent = 0);
	~UIChatRoom();
	void stringToHtmlFilter(QString &str);					//QStringתhtlm
	void stringToHtml(QString &str, QColor crl);			//QStringתhtlm����ɫ
	void imgPathToHtml(QString &path);						//edit ����ͼƬ
	public slots:
	void setBrow(QString path);
private:
	Ui::UIChatRoom ui;
	
	//�����	
	MyEmotionWidget*				m_smallEmotionWidget;
	MyEmotionWidget*				m_normalEmotionWidget;
	QLabel*						    m_lableTitle;
	QString							m_borw;//��ǰѡ�еı���
	bool							m_isBorw;//�Ƿ��б���
	void initEmotion();
	
private slots:
	void clickTalk();									// ���������
	void clickStudentList();							// ����ѧ���б�
	void clickProclamation();							// �������ۿ�
	void clickCleanText();								// ����
	void clickBrow();									// ���鰴ť
	void clickNotes();									// ��Ϣ��¼
	void clickSendMseeage();							// ������Ϣ
};

#endif // UICHATROOM_H
