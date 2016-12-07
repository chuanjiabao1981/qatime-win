#include "UIHelpword.h"
#include <windows.h>
#include <windowsx.h>
#include <QPainter>
#include <QScrollBar>
#include <QScrollArea>

#define MAINWINDOW_X_MARGIN 6
#define MAINWINDOW_Y_MARGIN 6
#define MAINWINDOW_TITLE_HEIGHT 49

UIHelpWord::UIHelpWord(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setFixedSize(400, 640);
	InitUI();
	connect(ui.close_pushButton, SIGNAL(clicked()), this, SLOT(closeDialog()));
}

UIHelpWord::~UIHelpWord()
{

}

bool UIHelpWord::nativeEvent(const QByteArray &eventType, void *message, long *result)
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
				if (ui.close_pushButton)
				{
					QRect Rect = ui.close_pushButton->geometry();
					if (x > Rect.left() && x < Rect.right() && y > Rect.top() && y < Rect.bottom())
					{
						return false;
					}
				}
				*result = HTCAPTION;
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

void UIHelpWord::paintEvent(QPaintEvent *event)
{
	QPainterPath path;
	path.setFillRule(Qt::WindingFill);
	path.addRect(2, 2, this->width() - 4, this->height() - 4);
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.fillPath(path, QBrush(Qt::white));

	QColor color(222, 222, 222);
	for (int i = 0; i < 4; i++)
	{
		QPainterPath path;
		path.setFillRule(Qt::WindingFill);

		for (int j = 0; j < 30; j++)
		{
			path.addRect(2 - i, j - i, this->width() - (2 - i) * 2, this->height() - (1 - i) * 2);
		}

		painter.setPen(color);
		painter.drawPath(path);
	}
}

void UIHelpWord::InitUI()
{
	QScrollArea* scrollArea = new QScrollArea();
	scrollArea->setLayout(ui.verticalLayout_2);

// 	this->scroll
// 	this->scrollArea->setWidget(mainLabel);
// 	ui->scrollArea->setWidgetResizable(true);
// 	ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
// 	ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	ui.pushButton_01->setText("ֱ�����ֶ�����Щ���ܣ�");
	ui.pushButton_02->setText("ֱ������ȷ������ʲô��");
	ui.pushButton_03->setText("���ѡ��γ̣�");
	ui.pushButton_04->setText("��ο����͹ر�ֱ����");
	ui.pushButton_05->setText("����л�����ͬ�ĸ�������ֱ����");
	ui.pushButton_06->setText("ֱ�������������ж�����ô�죿");
	ui.pushButton_07->setText("Ϊʲô�еĿγ���ʾ�����л���");
	ui.pushButton_08->setText("���ֱ��ʱ����ô�죿");
	ui.pushButton_09->setText("Ϊʲô�������ұ��Ҵ�����ʱ���٣�");
	ui.pushButton_10->setText("��С��ʱ��β鿴�������ݣ�");
	ui.pushButton_11->setText("��η������棿");
	ui.pushButton_12->setText("������ʲô�ã���ν��Ժͽ��ѧ����");
	ui.pushButton_13->setText("�����¼�ܹ��������֮ǰ�����ݣ�");
	ui.pushButton_14->setText("�ɼ�ģʽ��ʲô�ã������ѡ��");
	ui.pushButton_15->setText("ֱ����Ϊʲô���ܵ���رղɼ���");
	ui.pushButton_16->setText("ֱ����������������ʲô�ã�");
	ui.pushButton_17->setText("ֱ������ʲôԭ��");

	ui.label_01->setText("ֱ�����ֵ���Ҫ������Ϊ�˷�����ʦ�ܹ���ݵĹ����Լ��ĸ����࣬���ҿ��ٸ�Ч�Ľ���ֱ���ڿΣ�ʵ����ѧ��������������Ҫ���ܰ������������ҹ����γ�ֱ�������컥�����������桢����ѧ�����ԡ������¼��ѯ�ȡ�");
	ui.label_02->setText("������Ӳ��  ��  ѡ��γ�  ��  ��ʼֱ��  ��  ����ֱ��\n"
		"1.���ȼ����˺�����ͷ�Ƿ��������������Ӵ����Ƿ�������ͼ��\n"		"2.����γ̱�򸨵���ʦ���пγ�ѡ�񣬳ɹ������ϵͳ����ʾ��\n"		"3.ѡ����Ҫʹ�õĲɼ���ʽ������ʼֱ����ť���ɿ���ֱ����ֱ�������пɲ鿴���촰��������ѧ�����л�����\n"		"4.�ڿ�������Ϻ�������ֱ����ť��ȷ����ر�ֱ����ֱ�����������ڿγ̽�����ֱ����������ٴο�������\n");
	ui.label_03->setText("��ʦ����ͨ�����ַ�ʽѡ��γ̣�\n"
		"1.������Ӵ����Ϸ��γ̱��������ѡ��γ̽�����տγ̱�ѡ��\n"
		"2.���������ʦ���ѡ����Ӧ�ĸ��������ѡ��γ�˫�����ɡ�");
	ui.label_04->setText("����ֱ����Ҫ����ֱ�����Һ�ѡ����Ӧ�Ĳɼ�ģʽ��Ȼ������ʼֱ��(�����ж�����ʱ��ʾΪ����ֱ��)"
		"���ɣ��ر�ֱ����������ȷ�Ϻ󼴿ɹرա�");
	ui.label_05->setText("�л�����ͬ�ĸ���������Ҫ��������������Ȼ��ѡ��ý����ڽ���Ŀγ̣���ÿγ̴�����ֱ������ϿΣ������׽ڣ�״̬��ֱ���л�������ѡ�γ�Ϊ���׽ڴ��Ͽ�����Ҫ������һ�ں��ٽ����л���");
	ui.label_06->setText("ֱ������������������ԭ��������������ն�ֱ����ϵͳ������ʦ��ǰ��ֱ���γ̽���15���ӵĶ��߱�������ʦ��¼��ֻ����������ֱ����ť���ɻָ����糬ʱδ���¿������Զ�����ֱ������ʦ�����¿���ֱ����");
	ui.label_07->setText("Ϊ������ʦ׼ʱ�ڿΣ�ϵͳĿǰ��֧�ֵ��տγ̿�ѡ����������ѡ���ѽ�����δ��ʼ�Ŀγ̣���");
	ui.label_08->setText("��������γ̵���δ��������ʦ�ɷ��͹���֪ͨѧ�����Ͽ�ʱ���ʼֱ���ڿΣ���������ǰ2Сʱ���ͣ�����ѧ������׼ʱ�μ�Ӱ�������ڿ����ۣ���\n"
		"��������γ̵����ѽ�������ʦ���¼����ʱ����վ�Ըÿγ̽��е��Σ����κ�ɰ����µ�ʱ���ϿΡ�");
	ui.label_09->setText("�򸨵����ҽ���ʾ����ֱ���ĸ����ࣨ�������еĸ�������");
	ui.label_10->setText("��С��ʱ�鿴�����ұ�Եλ�ã���ͼ����ʾ����Ϊ�������������鿴��ϸ���������л�������ģʽ�鿴��");
	ui.label_11->setText("ѡ�񹫸洰�ں����������¹��桱��ť������Ҫ���������ݺ�������������ɷ�����");
	ui.label_12->setText("�����ܹ���Ч��ֹѧ�������Ӱ�����˵����ۣ�ѡ��ѧԱ���ں�����ѧԱ��������Ӧ�Ľ���ѡ���ѡ�󼴿ɽ��ԣ�ȡ�������ˣ�ȫ����ȫ��ѧ�����ԡ�");
	ui.label_13->setText("�����¼�ܹ�����������ӿ��ε��������е������¼�������԰����ڲ�ѯ��");
	ui.label_14->setText("��Բ�ͬ�Ľ�ѧ��ʽ����ʦ�ṩ���ʺϵ�ֱ���ɼ���ʽ��\n"
		"���ڲɼ��ܹ��ֲ��ɼ��ص����ݣ�������ֱ����ͬʱ�ܹ�ʵʱ�鿴ѧ�����������ݣ���Ҫ�������ض����ݵ�����ɼ�ģʽ��\n"
		"ȫ���ɼ��ܹ�ץȡ�����������ݣ�����ʦ��ʹ��PPT��΢�α����װ��ȫ�������ǽ��������ݳ��ָ�ѧ������Ҫ������PPT���װࡢPDF��ȫ��ֱ�����ݡ�");
	ui.label_15->setText("ֱ���в��ܹرղɼ���Ϊ�˷�ֹ��ʦ�Ĵ����������ѧ���ۿ��ǻ���������ֱ�������Ĵ����Ӱ���ѧ��ѧϰ���顣");
	ui.label_16->setText("������Ҫ�����������ݣ�\n"
		"1.ѡ�����������ȣ�\n"
		"���� - �ʺ���������Ϻõ���ʦ\n"
		"���� - �ʺ�����״��һ����û�\n"
		"2.ѡ����˷磺��ʦ��������ѡ����Ƶ�����豸��\n"
		"3.ѡ������ͷ����ʦ��������ѡ����Ƶ�����豸��\n"
		"��ѡ����Ӧ�����ú����رռ�����ɣ�");
	ui.label_17->setText("ֱ��������Ҫԭ���У�\n"
		"1.�����ȹ��Ͳ��������ϴ���Ƶ��������������������������Ϳ��Ϊ4 - 5M������������������������Ϳ��Ϊ1.5M����\n"
		"2.�������ù��ͣ�����ʹ��I5���ϼ����CPU�����ö�����ʾ������\n"
		"3.ϵͳ�����Խϲ����ʹ��win7 - sp1����ϵͳ�汾��");

	connect(ui.pushButton_01, SIGNAL(clicked()), this, SLOT(Label_01Visible()));
	connect(ui.pushButton_02, SIGNAL(clicked()), this, SLOT(Label_02Visible()));
	connect(ui.pushButton_03, SIGNAL(clicked()), this, SLOT(Label_03Visible()));
	connect(ui.pushButton_04, SIGNAL(clicked()), this, SLOT(Label_04Visible()));
	connect(ui.pushButton_05, SIGNAL(clicked()), this, SLOT(Label_05Visible()));
	connect(ui.pushButton_06, SIGNAL(clicked()), this, SLOT(Label_06Visible()));
	connect(ui.pushButton_07, SIGNAL(clicked()), this, SLOT(Label_07Visible()));
	connect(ui.pushButton_08, SIGNAL(clicked()), this, SLOT(Label_08Visible()));
	connect(ui.pushButton_09, SIGNAL(clicked()), this, SLOT(Label_09Visible()));
	connect(ui.pushButton_10, SIGNAL(clicked()), this, SLOT(Label_10Visible()));
	connect(ui.pushButton_11, SIGNAL(clicked()), this, SLOT(Label_11Visible()));
	connect(ui.pushButton_12, SIGNAL(clicked()), this, SLOT(Label_12Visible()));
	connect(ui.pushButton_13, SIGNAL(clicked()), this, SLOT(Label_13Visible()));
	connect(ui.pushButton_14, SIGNAL(clicked()), this, SLOT(Label_14Visible()));
	connect(ui.pushButton_15, SIGNAL(clicked()), this, SLOT(Label_15Visible()));
	connect(ui.pushButton_16, SIGNAL(clicked()), this, SLOT(Label_16Visible()));
	connect(ui.pushButton_17, SIGNAL(clicked()), this, SLOT(Label_17Visible()));
	
	QString objectname;
	QPushButton *button = NULL;
	for (int i = 1; i < 18;i++)
	{
		objectname = QString().sprintf("pushButton_%02d", i);
		button = this->findChild<QPushButton*>(objectname);
		if (button)
		{
			button->setMinimumHeight(25);
			button->setCursor(Qt::PointingHandCursor);
			button->setFlat(true);
		}
	}

	QString objectLabelName;
	QLabel *label = NULL;
	for (int i = 1; i < 18; i++)
	{
		objectLabelName = QString().sprintf("label_%02d", i);
		label = this->findChild<QLabel*>(objectLabelName);
		if (label)
		{
			label->setWordWrap(true);
			label->setAlignment(Qt::AlignTop);
			label->setFixedWidth(380);
			label->setVisible(false);
		}
	}
	
}

void UIHelpWord::closeDialog()
{
	setVisible(false);
}

void UIHelpWord::Label_01Visible()
{
	visibleAll();
	ui.label_01->setVisible(!ui.label_01->isVisible());
}
void UIHelpWord::Label_02Visible()
{
	visibleAll();
	ui.label_02->setVisible(!ui.label_02->isVisible());
}
void UIHelpWord::Label_03Visible()
{
	visibleAll();
	ui.label_03->setVisible(!ui.label_03->isVisible());
}
void UIHelpWord::Label_04Visible()
{
	visibleAll();
	ui.label_04->setVisible(!ui.label_04->isVisible());
}
void UIHelpWord::Label_05Visible()
{
	visibleAll();
	ui.label_05->setVisible(!ui.label_05->isVisible());
}
void UIHelpWord::Label_06Visible()
{
	visibleAll();
	ui.label_06->setVisible(!ui.label_06->isVisible());
}
void UIHelpWord::Label_07Visible()
{
	visibleAll();
	ui.label_07->setVisible(!ui.label_07->isVisible());
}
void UIHelpWord::Label_08Visible()
{
	visibleAll();
	ui.label_08->setVisible(!ui.label_08->isVisible());
}
void UIHelpWord::Label_09Visible()
{
	visibleAll();
	ui.label_09->setVisible(!ui.label_09->isVisible());
}
void UIHelpWord::Label_10Visible()
{
	visibleAll();
	ui.label_10->setVisible(!ui.label_10->isVisible());
}
void UIHelpWord::Label_11Visible()
{
	visibleAll();
	ui.label_11->setVisible(!ui.label_11->isVisible());
}
void UIHelpWord::Label_12Visible()
{
	visibleAll();
	ui.label_12->setVisible(!ui.label_12->isVisible());
}
void UIHelpWord::Label_13Visible()
{
	visibleAll();
	ui.label_13->setVisible(!ui.label_13->isVisible());
}
void UIHelpWord::Label_14Visible()
{
	visibleAll();
	ui.label_14->setVisible(!ui.label_14->isVisible());
}
void UIHelpWord::Label_15Visible()
{
	visibleAll();
	ui.label_15->setVisible(!ui.label_15->isVisible());
}
void UIHelpWord::Label_16Visible()
{
	visibleAll();
	ui.label_16->setVisible(!ui.label_16->isVisible());
}
void UIHelpWord::Label_17Visible()
{
	visibleAll();
	ui.label_17->setVisible(!ui.label_17->isVisible());
}

void UIHelpWord::visibleAll()
{
	QString objectLabelName;
	QLabel *label = NULL;
	for (int i = 1; i < 18; i++)
	{
		objectLabelName = QString().sprintf("label_%02d", i);
		label = this->findChild<QLabel*>(objectLabelName);
		if (label)
		{
			label->setVisible(false);
		}
	}
}