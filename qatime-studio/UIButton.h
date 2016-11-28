/*
*	Author       hzzhuling15
*	Date         2015/09/11
*	Copyright    Hangzhou, Netease Inc.
*	Brief        
*   Notice       
*/
#ifndef _LS_MEDIACAPTURE_UI_BUTTON_H_
#define _LS_MEDIACAPTURE_UI_BUTTON_H_
#include <QtWidgets/QPushButton>
#pragma execution_character_set("utf-8")

class CButton : public QPushButton
{
	Q_OBJECT

public:
	CButton(QWidget *parent = 0);
	CButton(int nId, QWidget *parent = 0);
	~CButton();

	void setImage(QString qsImage, int nState = 4);

	enum EButtonStatus
	{
		BS_NORMAL = 0,
		BS_HOVER,
		BS_DOWN,
		BS_DISABLED,
		_C_BS_STATUS_COUNT
	};

	enum EButtonImageIndex
	{
		BI_NORMAL,
		BI_HOVER,
		BI_DOWN,
		BI_DISABLED,
		_C_BI_COUNT,
	};

Q_SIGNALS:
	void sig_idclicked(int, bool);

	public Q_SLOTS:
	void onClicked(bool);

public:
	bool HintBtn(QPoint& pos);

protected:
	virtual void mousePressEvent(QMouseEvent *);
	virtual void mouseReleaseEvent(QMouseEvent *);
	virtual void mouseMoveEvent(QMouseEvent *);
	//     virtual void focusInEvent(QFocusEvent *);
	//     virtual void focusOutEvent(QFocusEvent *);
	virtual void enterEvent(QEvent *);
	virtual void leaveEvent(QEvent *);
	virtual void paintEvent(QPaintEvent *);
	void changeEvent(QEvent *e);

	EButtonStatus getButtonStatus() const;
	EButtonImageIndex getImageIndex() const;
protected:
	EButtonStatus m_eStatus;
	bool     m_bMouseDown;

private:
	QPixmap* m_pImage;
	QString  m_qsImage;
	int      m_nImageWidth;
	int      m_nImageHeight;
	int      m_nId;
	int      m_nState;

};

#endif // _LS_MEDIACAPTURE_UI_BUTTON_H_