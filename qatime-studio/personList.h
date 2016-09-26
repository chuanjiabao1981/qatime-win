#ifndef PERSONLIST_H
#define PERSONLIST_H
#include <QListWidget>
#include <QMenu>
#include <QMouseEvent>
#include <QLineEdit>
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "personlistbuddy.h"
#include <QTimer>
//自定义QListWidget
class personList : public QListWidget 
{
    Q_OBJECT
public:
    explicit personList(QWidget *parent = 0);
    void mousePressEvent(QMouseEvent *event);		//鼠标点击事件
    void contextMenuEvent(QContextMenuEvent*);		//菜单事件，为了显示菜单
    void initMenu();//初始化菜单
    QMenu *blankMenu;//点击空白上的菜单
    QMenu *groupMenu;//点击组上的菜单
    QMenu *personMenu;//点击人上的菜单
	QMap<QListWidgetItem*, personListBuddy*> groupMap;   // 组容器 - key:项 value:组
	QMap<QString, QListWidgetItem*> allStudents;
	QMap<QString, QListWidgetItem*> IdStudents;
    QMap<QListWidgetItem*,bool> isHideMap;//用来判断该组是否隐藏了
    QLineEdit *groupNameEdit;//组的名字，重命名的时候需要用到
    QListWidgetItem *currentItem;//当前的项
	void addStrdent(QString imagePath, QString stuName, QString ID);
	void initFronUi();
	void initSecUi();
	void setOlineNum(int olineNum, int allNum);//设置在线人数的接口
	void cleanStudents();		//清空列表中的数据
signals:
	void signalChickChage(int,QString, QString);
public slots:
    void slotAddGroup();   //添加组
    void slotDelGroup();   //删除组
    void slotAddBuddy();   //添加好友
    void slotDelBuddy();   //删除好友
    void slotRename();     //重命名组
	void findName(const QString name);//查找好友
	personListBuddy* findID(const QString id);	  //查找ID
    void slotRenameEditFshed();//命名完成

	void chickChage(int b,QString ID, QString name);	//禁言的状态发生改变
	void stopAllTalk(bool, QString ID); //禁言所有的学生
	void timeOutSet();
private:
	personListBuddy *buddyFirst;
	personListBuddy *buddySec;
	personListBuddy *buddyLast;
	QTimer *timer;//设置提示弹出时间
    
};
#endif // PERSONLIST_H