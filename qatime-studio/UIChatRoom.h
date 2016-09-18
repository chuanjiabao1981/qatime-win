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
	void stringToHtmlFilter(QString &str);					//QString转htlm
	void stringToHtml(QString &str, QColor crl);			//QString转htlm带颜色
	void imgPathToHtml(QString &path);						//edit 设置图片
	public slots:
	void setBrow(QString path);
private:
	Ui::UIChatRoom ui;
	
	//表情框	
	MyEmotionWidget*				m_smallEmotionWidget;
	MyEmotionWidget*				m_normalEmotionWidget;
	QLabel*						    m_lableTitle;
	QString							m_borw;//当前选中的表情
	bool							m_isBorw;//是否有表情
	void initEmotion();
	
private slots:
	void clickTalk();									// 弹出聊天框
	void clickStudentList();							// 弹出学生列表
	void clickProclamation();							// 弹出讨论框
	void clickCleanText();								// 清屏
	void clickBrow();									// 表情按钮
	void clickNotes();									// 消息记录
	void clickSendMseeage();							// 发送消息
};

#endif // UICHATROOM_H
