/*
*	Author       hzzhuling15
*	Date         2015/09/11
*	Copyright    Hangzhou, Netease Inc.
*	Brief        
*   Notice       
*/
#ifndef _LS_MEDIACAPTURE_UI_MESSAGEBOX_H_
#define _LS_MEDIACAPTURE_UI_MESSAGEBOX_H_
#include <QtWidgets/QDialog>
#include <windowsx.h>
#include <QTimer>

class QLabel;
class CButton;
#pragma execution_character_set("utf-8")
class CMessageBox : public QDialog
{
	Q_OBJECT

public:
	~CMessageBox();

public:
	static int CMessageBox::showMessage(
		const QString & title, const QString & text,
		const QString& btn0,
		const QString& btn1 = QString(),	
		QWidget *parent = NULL,
		bool bTip=false);

protected:
	CMessageBox(const QString & title, const QString & text, const QString & btn0, const QString & btn1, QWidget *parent = NULL, bool bTip=false);

	void initUI();

	protected Q_SLOTS:
	void slot_titleClose(bool);
	void slot_btnclicked(int, bool);
	void slot_onCountTimeout();

protected:
	virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result);
	virtual void paintEvent(QPaintEvent *);
	virtual void closeEvent(QCloseEvent *);

private:
	QLabel * m_pImage;
	QLabel*  m_pLabelIcon;
	QLabel * m_pInnerText;
	CButton* m_pBtnClose;

	QList<CButton*> m_lBtns;

	QString m_qsImage;
	QString m_qsText;

	QPushButton* m_pButtonConfirm;
	QPushButton* m_pButtonCancel;

	QTimer*     m_hideTimers;
};

#endif //_LS_MEDIACAPTURE_UI_MESSAGEBOX_H_