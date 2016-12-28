#ifndef PERSONLISTBUDDY_H  
#define PERSONLISTBUDDY_H  
#include <QWidget>  
#include <QLabel>  
#include <QEvent>  
#include <QCheckBox>
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QLineEdit>
#include <QPushButton>

//自定义信息Item类  
class personListBuddy : public QWidget  
{  
    Q_OBJECT  
public:  
    explicit personListBuddy(QWidget *parent = 0);  
	~personListBuddy();
	void initFirst();			//初始化第一行 
	void initFindPeople(int iWidth);		//初始化第二行
	void initNotFind(int iWidth);			//初始化没有找到好友提示
	QLabel *peopleNum;			//第一行的label
	QCheckBox* firstButton;		//第一行的按钮
	QLineEdit* secLinEdit;		//第二行的输入框
	QPushButton* secFindButton;	//第二行的查找按钮
	QLabel *notFind;			//最后一行。没有找到好友的时候显示。
	void initUi(const QString &szUrl, QString strName, QString ID, int iWidth);//初始化Ui
	QLabel *head;  //头像label  
    QLabel *name;  //用户名  
	bool isOline;	//是否在线
	QString m_ID;		//用户ID
	QPixmap headPath;//需要加载的头像
	QCheckBox* button; //按钮  
	bool	m_bZoom;
//  QLabel *sign;  //个性签名  
	void setOlineNum(int olineNum, int allNum);	
	void setCheckBox(bool bCheck);
	bool IsZoom();
	void SetZoom(bool bZoom);
	void setMute(bool bMute);
protected:
	virtual bool eventFilter(QObject *target, QEvent *event);
signals:  
	void emitRadioChange(int,QString,QString);
	void signalRadioChange(bool);
	void signalStopAllTalk(bool,QString);	//禁言所有学生信号
 	void signalFindName(const QString name);		//查找学生名字信号
	void signalFindNameNULL(const QString name);
	// 	void signalOnlyOnline(bool);
public slots :
	void radioChange(int b);			//禁止某个学生的言论	
	void stopAllTalk(bool b);			//禁止所有学生的言论
	void onlyOnline(bool b);			//只显示在线人员
	void findName();					//查找学生名字
	void findName(const QString);		//当查找学生编辑框为NULL时候，显示所有学生名字
private:
	void setNetworkPic(const QString &szUrl);
//	bool eventFilter(QObject *obj, QEvent *event);//事件过滤器 
};  
#endif // PERSONLISTBUDDY_H  