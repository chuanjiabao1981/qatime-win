#ifndef PALETTE_H
#define PALETTE_H

#include <QWidget>
#include <QStack>
#include <QPen>

namespace Ui {
class Palette;
}

class Shape;

class Palette : public QWidget
{
    Q_OBJECT

public:
    explicit Palette(QWidget *parent = 0);
    ~Palette();

    void setPenColor(const QColor &color);		//设置画笔颜色
	QColor getPenColor();						//获取当前画笔颜色

	void revocation();							//撤销上一次操作
	void cleanUp();								//清空白板

	void setIsDraw(bool isDraw);				//设置是否需要画
	void RecData(const std::string& data, const std::string &uid);		//接收数据
	std::list<std::string> BoardStringTokenize(const char* input, const char* delimitor);	//解析数据

	void DrawUndo();							//对方撤销
	int  colorConvert(QColor color);			//解析数据
	void SendSyncDraw();						//发送同步数据
	void SendFullScreen(int iOpen);				//发送全屏消息,1：开启 0：关闭

	enum DrawOpType
	{
		DrawOpStart = 1,
		DrawOpMove,
		DrawOpEnd,
		DrawOpUndo,
		DrawOpPktId,
		DrawOpClear,
		DrawOpClearCb,
	};

	enum MultiBoardOpType
	{
		kMultiBoardOpUnKnow = -1,
		kMultiBoardOpStart = 1,
		kMultiBoardOpMove = 2,
		kMultiBoardOpEnd = 3,
		kMultiBoardOpUndo = 4,
		kMultiBoardOpPktId = 5,
		kMultiBoardOpClear = 6,
		kMultiBoardOpClearCb = 7,
		kMultiBoardOpSyncQuery = 8,
		kMultiBoardOpSync = 9,
		kMultiBoardOpSyncPrep = 10,
		kMultiBoardOpSyncPrepCb = 11,
		kMultiBoardOpSign = 12,
		kMultiBoardOpSignEnd = 13,
		kMultiBoardOpDocInfo = 14, //"14:id（文档id）,page_num(当前页数，1开始计算),page_count（总页数）,type(状态通知：0，翻页操作：1);" "14:8c17c252-1276-4817-92ce-688576b8df4c,2,4,1;"
		kMultiBoardOpFullScreen = 15, //全屏分享
	};

private:
	void setMouseStyle(bool isDraw);			//设置鼠标指针样式

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

signals:
	void PicData(QString);
private slots:
	void slot_onCountTimeout();
private:
    Ui::Palette *ui;

    QStack< Shape * > mShapeStack;
	QStack< Shape * > mSycnShapeStack;			//接收的数据显示

    bool mMouseIsPress;							//鼠标是否按下
	bool mIsDraw;								//是否需要画
    QPen mPen;
	QTimer*	 m_timer;
	int  mStatus;								//全屏共享状态 0：未开启 1：开启
	std::string	m_SenderUid;					//发送者的uid
};

#endif // PALETTE_H
