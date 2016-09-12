#include "UIOtherApp.h"
#include <QScrollBar>
#include <QBitmap>
#include <QPainter>

#define QT_USERDATA			100

UIOtherApp::UIOtherApp(QWidget *parent)
	: QDialog(parent)
	, m_CurrentIndex(0)
{
	ui.setupUi(this);

	ui.Auxiliary_tableWidget->setFocusPolicy(Qt::NoFocus);

	connect(ui.Auxiliary_tableWidget, SIGNAL(itemSelectionChanged()), this, SLOT(CurrentIndex()));
	connect(ui.OK_pushBtn, SIGNAL(clicked()), this, SLOT(OK()));
	connect(ui.Cancel_pushBtn, SIGNAL(clicked()), this, SLOT(CANCEL()));

	initAppPath();
}

UIOtherApp::~UIOtherApp()
{

}

void UIOtherApp::setAppInfo(ST_NLSS_INDEVICE_INF* m_pAppWinds, int iAppCount)
{
	ui.Auxiliary_tableWidget->setRowCount(1);
	ui.Auxiliary_tableWidget->clearContents();

	for (int i = 0; i < iAppCount;i++)
	{
		ui.Auxiliary_tableWidget->insertRow(i+1);

		QString strAppName = m_pAppWinds[i].paFriendlyName;
		QTableWidgetItem *pItemNum = new QTableWidgetItem(strAppName);
		ui.Auxiliary_tableWidget->setItem(i - 1, 0, pItemNum);
	}

	ui.Auxiliary_tableWidget->removeRow(iAppCount);
}

void UIOtherApp::initAppPath()
{
	ui.Auxiliary_tableWidget->setColumnCount(1); //��������
	ui.Auxiliary_tableWidget->setRowCount(1);
	ui.Auxiliary_tableWidget->horizontalHeader()->setSectionsMovable(false);
	ui.Auxiliary_tableWidget->horizontalHeader()->setSectionsClickable(false);

	ui.Auxiliary_tableWidget->verticalHeader()->setDefaultSectionSize(25); //�����и�
	ui.Auxiliary_tableWidget->setFrameShape(QFrame::NoFrame); //�����ޱ߿�
	ui.Auxiliary_tableWidget->setShowGrid(false); //���ò���ʾ������
	ui.Auxiliary_tableWidget->verticalHeader()->setVisible(false); //���ô�ֱͷ���ɼ�
	ui.Auxiliary_tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);  //�ɶ�ѡ��Ctrl��Shift��  Ctrl+A�����ԣ�
	ui.Auxiliary_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //����ѡ����Ϊʱÿ��ѡ��һ��
	ui.Auxiliary_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //���ò��ɱ༭
	ui.Auxiliary_tableWidget->horizontalHeader()->resizeSection(0, 370); //���ñ�ͷ��һ�еĿ��Ϊ370
	ui.Auxiliary_tableWidget->horizontalHeader()->setFixedHeight(30); //���ñ�ͷ�ĸ߶�
	ui.Auxiliary_tableWidget->setStyleSheet("selection-background-color:lightblue;");  //����ѡ�б���ɫ
	ui.Auxiliary_tableWidget->horizontalHeader()->setVisible(false);

	// ���ù�������ʽ
	ui.Auxiliary_tableWidget->verticalScrollBar()->setStyleSheet("QScrollBar:vertical"
		"{"
		"width:8px;"
		"background:rgba(0,0,0,0%);"
		"margin:0px,0px,0px,0px;"
		"padding-top:9px;"
		"padding-bottom:9px;"
		"}"
		"QScrollBar::handle:vertical"
		"{"
		"width:8px;"
		"background:rgba(0,0,0,25%);"
		" border-radius:4px;"
		"min-height:20;"
		"}"
		"QScrollBar::handle:vertical:hover"
		"{"
		"width:8px;"
		"background:rgba(0,0,0,50%);"
		" border-radius:4px;"
		"min-height:20;"
		"}"
		"QScrollBar::add-line:vertical"
		"{"
		"height:9px;width:8px;"
		"border-image:url(:/images/a/3.png);"
		"subcontrol-position:bottom;"
		"}"
		"QScrollBar::sub-line:vertical"
		"{"
		"height:9px;width:8px;"
		"border-image:url(:/images/a/1.png);"
		"subcontrol-position:top;"
		"}"
		"QScrollBar::add-line:vertical:hover"
		"{"
		"height:9px;width:8px;"
		"border-image:url(:/images/a/4.png);"
		"subcontrol-position:bottom;"
		"}"
		"QScrollBar::sub-line:vertical:hover"
		"{"
		"height:9px;width:8px;"
		"border-image:url(:/images/a/2.png);"
		"subcontrol-position:top;"
		"}"
		"QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical"
		"{"
		"background:rgba(0,0,0,10%);"
		"border-radius:4px;"
		"}"
		);
}

void UIOtherApp::CurrentIndex()
{
	m_CurrentIndex = ui.Auxiliary_tableWidget->currentRow();
}

void UIOtherApp::OK()
{
	done(2);
}

void UIOtherApp::CANCEL()
{
	done(0);
}

int UIOtherApp::getCurrentIndex()
{
	return m_CurrentIndex;
}

void UIOtherApp::resizeEvent(QResizeEvent *e)
{
	QBitmap bmp(this->size());
	bmp.fill();
	QPainter p(&bmp);
	p.setPen(Qt::NoPen);
	p.setBrush(Qt::black);
	p.drawRoundedRect(bmp.rect(), 10, 10);
	setMask(bmp);
	bmp.clear();
}

void UIOtherApp::closeEvent(QCloseEvent *)
{
	reject();
}