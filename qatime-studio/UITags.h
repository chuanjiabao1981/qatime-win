#ifndef UITAGS_H
#define UITAGS_H

#include <QWidget>
#include "ui_UITags.h"
#include "UIChatRoom.h"

class UIChatRoom;
class UITags : public QWidget
{
	Q_OBJECT

public:
	UITags(QWidget *parent = 0);
	~UITags();

	QString			m_name;
	QString			m_id;
	QString			m_chatID;
private:
	Ui::UITags ui;
	UIChatRoom*			m_room;
	bool				m_bSelect;			// 当前是否被选中
	QString				m_boardStream;		// 白板直播流地址
	QString				m_cameraStream;		// 摄像头直播流地址
	bool				m_bModle;			// 当前窗口模式
	int					m_mLessonType;		// 当前窗口课程类型
	QString				m_boardUrl;
	QString				m_cameraUrl;
protected:
	virtual void paintEvent(QPaintEvent *event);
	virtual void mousePressEvent(QMouseEvent *e);       //--鼠标按下事件 

signals:
	void clickTag(UITags* tag);
    void DeleteTag(UITags* tag);

private slots :
	void CloseDialog();			// 最大化

public:
	void SetCourseNameAndID(QString sName, QString id, QString chatID, QString board, QString camera);
	UIChatRoom* GetRoom();			// 获取当前的聊天窗口
	void SetRoom(UIChatRoom* room);	// 设置关联的聊天窗口
	void setStyle(bool bSel);		// 选中标签样式
	bool IsSelect();				// 当前是否被选中
	QString ChatID();
	QString CourseID();
	QString BoardStream();
	QString CameraStream();
	void setBoardStream(QString strBoard);			
	void setCameraStream(QString strCamera);
	void setModle(bool bModle);		// 设置模式 true为直播状态 false为聊天状态，默认都为聊天状态
	bool IsModle();
	void setLessonType(int mLessonType);
	int GetLessonType();
};

#endif // UITAGS_H
