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
//�Զ�����ϢItem��  
class personListBuddy : public QWidget  
{  
    Q_OBJECT  
public:  
    explicit personListBuddy(QWidget *parent = 0);  
	void initFirst();//��ʼ����һ�� 
	void initFindPeople();//��ʼ���ڶ���
	QLabel *peopleNum;		//��һ�е�label
	QCheckBox* firstButton; //��һ�еİ�ť
	QLineEdit* secLinEdit;		//�ڶ��е������
	QPushButton* secFindButton;	//�ڶ��еĲ��Ұ�ť
	void initUi(const QString &szUrl,QString strName,QString ID);//��ʼ��Ui  
//    QWidget *head;  //ͷ��  
	QLabel *head;  //ͷ��label  
    QLabel *name;  //�û���  
	bool isOline;	//�Ƿ�����
	QString m_ID;		//�û�ID
	QPixmap headPath;//��Ҫ���ص�ͷ��
	QCheckBox* button; //��ť  
//  QLabel *sign;  //����ǩ��  
	void setOlineNum(int olineNum, int allNum);			
signals:  
	void emitRadioChange(bool,QString,QString);
	void signalRadioChange(bool);
	void signalStopAllTalk(bool,QString);	//��������ѧ���ź�
 	void signalFindName(QString name);		//����ѧ�������ź�
	// 	void signalOnlyOnline(bool);
public slots :
	void radioChange(bool b);			//��ֹĳ��ѧ��������	
	void stopAllTalk(bool b);			//��ֹ����ѧ��������
	void onlyOnline(bool b);			//ֻ��ʾ������Ա
	void findName();		//����ѧ������
private:
	void setNetworkPic(const QString &szUrl);
	bool eventFilter(QObject *obj, QEvent *event);//�¼������� 
};  
#endif // PERSONLISTBUDDY_H  