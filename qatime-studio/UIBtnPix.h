/*
*	Author       hzzhuling15
*	Date         2015/09/11
*	Copyright    Hangzhou, Netease Inc.
*	Brief        
*   Notice       
*/
#ifndef _LS_UI_BUTTON_H_
#define _LS_UI_BUTTON_H_
#include <QtWidgets/QPushButton>
#pragma execution_character_set("utf-8")

class CBtnPix : public QPushButton
{
	Q_OBJECT

public:
	CBtnPix(QWidget *parent = 0);
	CBtnPix(QString url, QPixmap pixmap,QWidget *parent = 0);
	CBtnPix(QPixmap pixmap, QWidget *parent = 0);
	~CBtnPix();

Q_SIGNALS:
	void sig_idclicked(QString, QPixmap, bool);
	void sig_faildclicked(CBtnPix*);

public Q_SLOTS:
	void onClicked(bool);
	void onClickedFail(bool);
	
public:
	QPixmap CenterRect(QPixmap pix);
protected:

private:
	QString  m_qsImage;
	int      m_nImageWidth;
	int      m_nImageHeight;
	QPixmap  m_pixmap;
	QPushButton* m_pImgProcess;
	QString	 m_MsgID;
	QPushButton* m_pFailBtn;

public:
	void SetImgProcess(QPushButton* btn);		// 设置进度按钮
	QPushButton* GetImgProcess();				// 获取进度按钮
	void SetMsgID(QString msgid);				// 设置消息ID
	QString GetMsgID();							// 获取消息
	QString GetUrl();							// 获取图片的URL

public:
	void AddFailBtn();							//添加失败按钮
	void DeleteFailBtn();						//删除失败按钮
};

#endif // _LS_MEDIACAPTURE_UI_BUTTON_H_