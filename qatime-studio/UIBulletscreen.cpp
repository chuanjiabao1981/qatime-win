#include "UIBulletscreen.h"
#include <windowsx.h>
#include <wtypes.h>
#include <QPainter>
#include <QMouseEvent>

#define MAINWINDOW_X_MARGIN 6
#define MAINWINDOW_Y_MARGIN 6
#define MAINWINDOW_TITLE_HEIGHT 24
#pragma execution_character_set("utf-8")
UIBulletScreen::UIBulletScreen(QWidget *parent)
	: QWidget(parent)
	, m_BulletSet(NULL)
	, m_parent(NULL)
{
	ui.setupUi(this);
	setFocusPolicy(Qt::ClickFocus);
	setAttribute(Qt::WA_TranslucentBackground);
	this->setStyleSheet(QStringLiteral("border-image: url(:/LoginWindow/images/Bullet.png);"));
	ui.talk_pushButton->setStyleSheet("QPushButton{border-image:url(:/LoginWindow/images/bulletTalk_b.png);}");
	ui.set_pushButton->setStyleSheet("QPushButton{border-image:url(:/LoginWindow/images/bulletSet_b.png);}");
	ui.close_pushButton->setStyleSheet("QPushButton{border-image:url(:/LoginWindow/images/bulletClose_b.png);}");

	connect(ui.close_pushButton, SIGNAL(clicked()), this, SLOT(CloseDialog()));
	connect(ui.set_pushButton, SIGNAL(clicked()), this, SLOT(SetDialog()));

	m_BulletSet = new UIBulletSet();
	m_BulletSet->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
	m_BulletSet->setMainWindow(this);
	m_BulletSet->hide();

	m_TeacherColor = QColor(190, 11, 11);
	m_StudentColor = QColor(153, 153, 153);
	m_ContentColor = QColor(0,0,0);
}

UIBulletScreen::~UIBulletScreen()
{

}

void UIBulletScreen::setMainWindow(UIMainWindow* parent)
{
	m_parent = parent;
}

void UIBulletScreen::paintEvent(QPaintEvent *event)
{
	int iWidth = this->size().width();
	int iHeight = this->size().height();

	QPainterPath path;
	path.setFillRule(Qt::WindingFill);
	path.addRect(iWidth / 2, iHeight / 2, this->width() - iWidth, this->height() - iHeight);

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.fillPath(path, QBrush(Qt::white));

	QColor color(6, 157, 213, 255);
	for (int i = 0; i < iWidth / 2; i++)
	{
		QPainterPath path;
		path.setFillRule(Qt::WindingFill);
		path.addRect(iWidth / 2 - i, iHeight / 2 - i, this->width() - (iWidth / 2 - i) * 2, this->height() - (iHeight / 2 - i) * 2);
		color.setAlpha(1);
		painter.setPen(color);
		painter.drawPath(path);
	}
}

void UIBulletScreen::focusInEvent(QFocusEvent *)
{

}

bool UIBulletScreen::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
	if ("windows_generic_MSG" == eventType)
	{
		PMSG pMsg = static_cast<PMSG>(message);
		switch (pMsg->message)
		{
		case WM_NCHITTEST:
		{
			int x = GET_X_LPARAM(pMsg->lParam) - this->frameGeometry().x();
			int y = GET_Y_LPARAM(pMsg->lParam) - this->frameGeometry().y();

			int xflag = (x <= MAINWINDOW_X_MARGIN) ? -1 : ((x < this->width() - MAINWINDOW_X_MARGIN) ? 0 : 1);
			int yflag = (y <= MAINWINDOW_Y_MARGIN) ? -1 : ((y < this->height() - MAINWINDOW_Y_MARGIN) ? 0 : 1);

			if (-1 == xflag && -1 == yflag)
			{
				*result = HTTOPLEFT;
			}
			else if (-1 == xflag && 0 == yflag)
			{
				*result = HTLEFT;
			}
			else if (-1 == xflag && 1 == yflag)
			{
				*result = HTBOTTOMLEFT;
			}
			else if (0 == xflag && -1 == yflag)
			{
				*result = HTTOP;
			}
			else if (0 == xflag && 0 == yflag)
			{
				*result = HTCLIENT;
			}
			else if (0 == xflag && 1 == yflag)
			{
				*result = HTBOTTOM;
			}
			else if (1 == xflag && -1 == yflag)
			{
				*result = HTTOPRIGHT;
			}
			else if (1 == xflag && 0 == yflag)
			{
				*result = HTRIGHT;
			}
			else if (1 == xflag && 1 == yflag)
			{
				*result = HTBOTTOMRIGHT;
			}
			if (0 == xflag && y > MAINWINDOW_Y_MARGIN && y <= MAINWINDOW_TITLE_HEIGHT)
			{
				if (ui.set_pushButton && ui.close_pushButton)
				{
					QRect Rect = ui.set_pushButton->geometry();
					if (x > Rect.left() && x < Rect.right() && y > Rect.top() && y < Rect.bottom())
					{
						return false;
					}

					Rect = ui.close_pushButton->geometry();
					if (x > Rect.left() && x < Rect.right() && y > Rect.top() && y < Rect.bottom())
					{
						return false;
					}
				}
				*result = HTCAPTION;
//				setCursor(Qt::OpenHandCursor);
			}
			return true;
		}
		break;
		default:
			return false;
			break;
		}
	}
	return false;
}

void UIBulletScreen::CloseDialog()
{
	DeleteTalkData();

	hide();
	if (m_BulletSet)
		m_BulletSet->hide();
}

void UIBulletScreen::DeleteTalkData()
{
	// 删除聊天数据
	ui.textBrowser->clear();

	// ToMainWindow发送弹幕关闭消息
//	m_parent->closeBulletMsg();
}

void UIBulletScreen::SetDialog()
{
	if (m_BulletSet)
	{
		QPoint pt;
		pt = mapToGlobal(ui.set_pushButton->pos());
		m_BulletSet->move(pt.x()-150,pt.y()+24);
		m_BulletSet->show();
		m_BulletSet->setFocus();
		SetWindowPos((HWND)m_BulletSet->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	}
}

void UIBulletScreen::stringToHtmlFilter(QString &str)
{
	//注意这几行代码的顺序不能乱，否则会造成多次替换
	str.replace("&", "&amp;");
	str.replace(">", "&gt;");
	str.replace("<", "&lt;");
	str.replace("\"", "&quot;");
	str.replace("\'", "&#39;");
	str.replace(" ", "&nbsp;");
	str.replace("\n", "<br>");
	str.replace("\r", "<br>");
}

void UIBulletScreen::stringToHtml(QString &str, QColor crl)
{
	QByteArray array;
	array.append(crl.red());
	array.append(crl.green());
	array.append(crl.blue());
	QString strC(array.toHex());
	str = QString("<span style=\" color:#%1;\">%2</span>").arg(strC).arg(str);
}

void UIBulletScreen::SetContentColor(QColor color)
{
	m_ContentColor = color;
}

void UIBulletScreen::SetFontSize(int iSize)
{
	QFont font;
	font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
	font.setPointSize(iSize);
	ui.textBrowser->setFont(font);
}
void UIBulletScreen::ReciverStudent(QString name, QString content)
{
	stringToHtml(name, m_StudentColor);
	stringToHtml(content, m_ContentColor);
	ui.textBrowser->insertHtml(name + ": " + content);
	ui.textBrowser->append("");
}

void UIBulletScreen::ReciverTeacher(QString name, QString content)
{
	stringToHtml(name, m_TeacherColor);
	stringToHtml(content, m_ContentColor);
	ui.textBrowser->insertHtml(name + ": "+content);
	ui.textBrowser->append("");
}