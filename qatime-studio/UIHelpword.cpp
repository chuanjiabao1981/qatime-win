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

	ui.pushButton_01->setText("直播助手都有哪些功能？");
	ui.pushButton_02->setText("直播的正确流程是什么？");
	ui.pushButton_03->setText("如何选择课程？");
	ui.pushButton_04->setText("如何开启和关闭直播？");
	ui.pushButton_05->setText("如何切换到不同的辅导教室直播？");
	ui.pushButton_06->setText("直播过程中意外中断了怎么办？");
	ui.pushButton_07->setText("为什么有的课程提示不能切换？");
	ui.pushButton_08->setText("错过直播时间怎么办？");
	ui.pushButton_09->setText("为什么辅导教室比我创建的时候少？");
	ui.pushButton_10->setText("最小化时如何查看聊天内容？");
	ui.pushButton_11->setText("如何发布公告？");
	ui.pushButton_12->setText("禁言有什么用，如何禁言和解禁学生？");
	ui.pushButton_13->setText("聊天记录能够产看多久之前的内容？");
	ui.pushButton_14->setText("采集模式有什么用，该如何选择？");
	ui.pushButton_15->setText("直播中为什么不能点击关闭采集？");
	ui.pushButton_16->setText("直播控制面板的设置有什么用？");
	ui.pushButton_17->setText("直播卡是什么原因？");

	ui.label_01->setText("直播助手的主要功能是为了方便老师能够便捷的管理自己的辅导班，并且快速高效的进行直播授课，实现与学生互动交流；主要功能包含：辅导教室管理、课程直播、聊天互动、发布公告、管理学生发言、聊天记录查询等。");
	ui.label_02->setText("测试软硬件  →  选择课程  →  开始直播  →  结束直播\n"
		"1.首先检查麦克和摄像头是否正常工作，监视窗口是否有桌面图像；\n"		"2.点击课程表或辅导教师进行课程选择，成功进入后系统会提示；\n"		"3.选择想要使用的采集方式后点击开始直播按钮即可开启直播，直播过程中可查看聊天窗口内容与学生进行互动；\n"		"4.授课内容完毕后点击结束直播按钮并确定后关闭直播（直播结束不等于课程结束，直播结束后可再次开启）；\n");
	ui.label_03->setText("老师可以通过两种方式选择课程：\n"
		"1.点击监视窗口上方课程标题区域的选择课程进入今日课程表选择；\n"
		"2.点击辅导教师面板选择相应的辅导班后再选择课程双击即可。");
	ui.label_04->setText("开启直播需要进入直播教室后并选择相应的采集模式，然后点击开始直播(意外中断重连时显示为继续直播)"
		"即可；关闭直播则点击结束确认后即可关闭。");
	ui.label_05->setText("切换到不同的辅导教室需要点击辅导教室面板然后选择该教室内今天的课程，如该课程处于已直播或待上课（仅限首节）状态则直接切换，如所选课程为非首节待上课则需要结束上一节后再进行切换。");
	ui.label_06->setText("直播过程中如遇到意外原因（如电脑死机）终端直播，系统将对老师当前所直播课程进行15分钟的短线保护，老师登录后只需点击继续回直播按钮即可恢复。如超时未重新开启则自动结束直播，老师可重新开启直播。");
	ui.label_07->setText("为保护老师准时授课，系统目前仅支持当日课程可选（即：不能选择已结束和未开始的课程）。");
	ui.label_08->setText("如所错过课程当日未结束，老师可发送公告通知学生新上课时间后开始直播授课（公告需提前2小时发送，以免学生不能准时参加影响您的授课评价）；\n"
		"如所错过课程当日已结束，老师需登录答疑时间网站对该课程进行调课，调课后可按照新的时间上课。");
	ui.label_09->setText("因辅导教室仅显示可以直播的辅导班（即开课中的辅导）。");
	ui.label_10->setText("最小化时查看桌面右边缘位置，该图标所示内容为聊天计数；如需查看详细内容需点击切换至正常模式查看。");
	ui.label_11->setText("选择公告窗口后点击“发布新公告”按钮，输入要发布的内容后点击发布即可完成发布。");
	ui.label_12->setText("禁言能够有效防止学生过多或影响他人的言论；选择学员窗口后点击该学员名称所对应的禁言选项，勾选后即可禁言，取消后解禁了；全禁则将全体学生禁言。");
	ui.label_13->setText("聊天记录能够产看辅导班从开课到现在所有的聊天记录，并可以按日期查询。");
	ui.label_14->setText("针对不同的教学方式给老师提供更适合的直播采集方式：\n"
		"窗口采集能够局部采集重点内容，并且在直播的同时能够实时查看学生的聊天内容，主要适用于特定内容的区域采集模式；\n"
		"全屏采集能够抓取整个桌面内容，让老师在使用PPT或微课宝、白班等全屏功能是将所有内容呈现给学生，主要适用于PPT、白班、PDF等全屏直播内容。");
	ui.label_15->setText("直播中不能关闭采集是为了防止老师的错误操作导致学生观看是画面黑屏造成直播结束的错觉，影响教学和学习体验。");
	ui.label_16->setText("设置主要包含如下内容：\n"
		"1.选择推流清晰度：\n"
		"高清 - 适合网络情况较好的老师\n"
		"标清 - 适合网络状况一般的用户\n"
		"2.选择麦克风：老师可以自主选择音频输入设备。\n"
		"3.选择摄像头：老师可以自主选择视频输入设备。\n"
		"（选择相应的设置后点击关闭即可完成）");
	ui.label_17->setText("直播卡的主要原因有：\n"
		"1.网络宽度过低不能正常上传视频流（高清推流建议网络上行最低宽带为4 - 5M；标清推流建议网络上行最低宽带为1.5M）；\n"
		"2.电脑配置过低（建议使用I5以上级别的CPU并配置独立显示卡）；\n"
		"3.系统兼容性较差（建议使用win7 - sp1以上系统版本）");

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