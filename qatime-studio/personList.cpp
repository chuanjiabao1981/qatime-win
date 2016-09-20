
#include "personlist.h"
#include <QAction>
#include <QIcon>
#include "UIMessageBox.h"
#pragma execution_character_set("utf-8")
personList::personList(QWidget *parent) :
    QListWidget(parent)
	, currentItem(NULL)
{
    setFocusPolicy(Qt::NoFocus);       // ȥ��itemѡ��ʱ�����߱߿�
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//ˮƽ�������ر�
    initMenu();	
	initFronUi();
	initSecUi();
}
//��ʼ���˵�
void personList::initMenu()
{
    //��ʼ����
    blankMenu = new QMenu();
    groupMenu = new QMenu();
    personMenu = new QMenu();
    groupNameEdit=new QLineEdit();
	QAction *rename = new QAction("������", this);
	QAction *addBuddy = new QAction("���Ӻ���", this);
//    QAction *addGroup = new QAction("���ӷ���", this);
//    QAction *delGroup = new QAction("ɾ������", this);    
//    QAction *delBuddy = new QAction("ɾ������", this);
    //���ã�
    groupNameEdit->setParent(this);  //���ø���
    groupNameEdit->hide(); //���ó�ʼʱ����
    groupNameEdit->setPlaceholderText("δ����");//���ó�ʼʱ������
    //���֣�
	blankMenu->addAction(addBuddy);
    groupMenu->addAction(rename);
    groupMenu->addAction(addBuddy);
 //   personMenu->addAction(delBuddy);
//   groupMenu->addAction(delGroup);
    //��Ϣ�ۣ�
    connect(groupNameEdit,SIGNAL(editingFinished()),this,SLOT(slotRenameEditFshed()));
	connect(rename, SIGNAL(triggered()), this, SLOT(slotRename()));
	connect(addBuddy, SIGNAL(triggered()), this, SLOT(slotAddBuddy()));
//    connect(addGroup,SIGNAL(triggered()),this,SLOT(slotAddGroup()));
//    connect(delGroup,SIGNAL(triggered()),this,SLOT(slotDelGroup()));  
//    connect(delBuddy,SIGNAL(triggered()),this,SLOT(slotDelBuddy()));

}
//������¼�
void personList::mousePressEvent(QMouseEvent *event)
{
    QListWidget::mousePressEvent(event); // ��������û���mousePressEvent��item��select����첻��Ӧ,���ø��࣬��QSS��Ч����ΪQSS���ڸ���QListWidget����������Ӵ��ڣ��������ϲ㴰�ڣ��Ǹ����ڸ������ϵģ��������ڸ����ڲ�����Ϣ
    //��ֹһ���������������item�������������item��հ״�ʱ��ָ����item��currentItem����������item
    if(groupNameEdit->isVisible() && !(groupNameEdit->rect().contains(event->pos())))
    {
        if(groupNameEdit->text()!=NULL)
            currentItem->setText(groupNameEdit->text());
        groupNameEdit->setText("");
        groupNameEdit->hide();
    }
    currentItem = this->itemAt(mapFromGlobal(QCursor::pos()));//���λ�õ�Item�������Ҽ��������ȡ
    if(event->button()==Qt::LeftButton && currentItem!=NULL && currentItem==groupMap.value(currentItem))//����������������ǵ��������
    {
        if(isHideMap.value(currentItem))                                  //�����ǰ�����أ�����ʾ
        {
            foreach(QListWidgetItem* subItem, groupMap.keys(currentItem))//������Ķ�Ӧ������������ͺ��ѣ�
                if(subItem!=currentItem)                                 //�������Ļ������д���
                {
                    subItem->setHidden(false);                            //����ȫ����ʾ
                }
            isHideMap.insert(currentItem,false);                          //���ø���Ϊ��ʾ״̬
            currentItem->setIcon(QIcon(":/arrowDown"));
        }
        else                                                             //������ǰ����ʾ��������
        {
            foreach(QListWidgetItem* subItem, groupMap.keys(currentItem))//������Ķ�Ӧ������������ͺ��ѣ�
                if(subItem!=currentItem)                                 //�������Ļ������д���
                {
                    subItem->setHidden(true);                            //����ȫ������
                }
             isHideMap.insert(currentItem,true);                          //���ø���Ϊ����״̬
             currentItem->setIcon(QIcon(":/arrowRight"));
        }
    }
}
//�˵��¼���Ϊ����ʾ�˵����������Ҽ���Ӧ��������¼�mousePressEvent������contextMenuEvent
void personList::contextMenuEvent(QContextMenuEvent *event)
{
    QListWidget::contextMenuEvent(event);           //���û����¼�
    if(currentItem==NULL)                           //�����������ǿհ״�
    {
        blankMenu->exec(QCursor::pos());
        return;
    }
    if(currentItem==groupMap.value(currentItem))    // ��������������
        groupMenu->exec(QCursor::pos());
    else                                            //�����������Ǻ���
        personMenu->exec(QCursor::pos());
}
//������
void personList::slotAddGroup()
{
    QListWidgetItem *newItem=new QListWidgetItem(QIcon(":/arrowRight"),QString("δ����"));    //����һ��Item
    newItem->setSizeHint(QSize(this->width(),25));//���ÿ��ȡ��߶�
    this->addItem(newItem);         //�ӵ�QListWidget��
    groupMap.insert(newItem,newItem);//�ӵ�����groupMap�key��value��Ϊ��
    isHideMap.insert(newItem,true);  //���ø�������״̬
    groupNameEdit->raise();
    groupNameEdit->setText(QString("δ����")); //����Ĭ������
    groupNameEdit->selectAll();        //����ȫѡ
    groupNameEdit->setGeometry(this->visualItemRect(newItem).left()+15,this->visualItemRect(newItem).top()+1,this->visualItemRect(newItem).width(),this->visualItemRect(newItem).height()-2);//���ֵ�λ��
    groupNameEdit->show();              //��ʾ
    groupNameEdit->setFocus();          //��ȡ����
    currentItem = newItem;	   // ��ΪҪ��group���������Ե�ǰ��currentItem��Ϊ��group
}
//ɾ����
void personList::slotDelGroup()
{
    foreach(QListWidgetItem* item, groupMap.keys(currentItem))  //������������к��Ѻ���������
    {
        groupMap.remove(item);   //�Ƴ�
        delete item;   //ɾ��
    }
    isHideMap.remove(currentItem); //�Ƴ�
}
//������
void personList::slotRename()
{
    groupNameEdit->raise();
    groupNameEdit->setGeometry(this->visualItemRect(currentItem).left()+15,this->visualItemRect(currentItem).top()+1,this->visualItemRect(currentItem).width(),this->visualItemRect(currentItem).height()-2);//���ֵ�λ��
    groupNameEdit->setText(currentItem->text());   //��ȡ����������
    groupNameEdit->show();                        //��ʾ
    groupNameEdit->selectAll();                   //ȫѡ
    groupNameEdit->setFocus();                        //��ȡ����
}

void personList::initFronUi()
{
	buddyFirst = new personListBuddy();
	connect(buddyFirst, SIGNAL(signalStopAllTalk(bool, QString)), this, SLOT(stopAllTalk(bool, QString)));
	buddyFirst->initFirst();
	QListWidgetItem *newItem = new QListWidgetItem();
	newItem->setSizeHint(QSize(300, 30));
	this->insertItem(0, newItem);
	this->setItemWidget(newItem, buddyFirst);
	newItem->setHidden(false);
	groupMap.insert(newItem, currentItem);
}
void personList::initSecUi()
{
	buddySec = new personListBuddy();	
	buddySec->initFindPeople();
	connect(buddySec, SIGNAL(signalFindName(QString)), this, SLOT(findName(QString)));	
	QListWidgetItem *newItem = new QListWidgetItem();
	newItem->setSizeHint(QSize(30, 30));
	this->insertItem(1 , newItem);
	this->setItemWidget(newItem, buddySec);	
	newItem->setHidden(false);
	groupMap.insert(newItem, currentItem);
}
void personList::addStrdent(QString imagesUrl,QString stuName,QString ID)
{
	personListBuddy *buddy = new personListBuddy();
	buddy->initUi("http://img4.duitang.com/uploads/item/201303/08/20130308111421_3msAX.png", stuName,ID);
	connect(buddy, SIGNAL(emitRadioChange(bool, QString, QString)), this, SLOT(chickChage(bool, QString, QString)));
	QList<QListWidgetItem*> tem = groupMap.keys(currentItem);	
	QListWidgetItem *newItem = new QListWidgetItem();       
	newItem->setSizeHint(QSize(30, 30));
	int num = row(currentItem) + tem.count();
	this->insertItem(num + 3, newItem);
	this->setItemWidget(newItem, buddy);
	groupMap.insert(newItem, currentItem);  
	allStudents.insert(stuName, newItem);
	newItem->setHidden(false);
}
void personList::slotAddBuddy()
{    
	addStrdent("", "111111","1");
	addStrdent("", "��ȥ","2");
	addStrdent("", "���¿�","2");
	addStrdent("", "�Ǻ���","4"); 
	addStrdent("", "ôô��","5");
	addStrdent("", "���õ�","6");
	addStrdent("", "������","7");
	addStrdent("", "Ҷ����","8");
	return;
//    buddy->sign->setText("��ͨ�׵����ԣ�����̵ļ�����");   //���ø���ǩ��
//     QList<QListWidgetItem*> tem = groupMap.keys(currentItem);//��ǰ���Ӧ��������鱾���ͺ��ѣ����Ƹ�tem
//     //�ؼ�����
//     QListWidgetItem *newItem = new QListWidgetItem();       //����һ��newItem
// 	newItem->setSizeHint(QSize(30, 30));
//     this->insertItem(row(currentItem)+tem.count(),newItem); //����newItem���뵽����
//     this->setItemWidget(newItem, buddy); //��buddy������newItem
//     groupMap.insert(newItem,currentItem);   //�ӽ�������keyΪ���ѣ�valueΪ��
//     if(isHideMap.value(currentItem))          //������������أ���ӽ�ȥ�ĺ�������Ϊ����
//         newItem->setHidden(true);
//     else                                      //���򣬸ú�������Ϊ��ʾ
//         newItem->setHidden(false);
}
//ɾ������
void personList::slotDelBuddy()
{
    groupMap.remove(currentItem);  //�Ƴ��ú���
    delete currentItem;            //ɾ��
}
//���Һ���
void personList::findName(QString name)
{
	if (name.isEmpty())
	{
		for (int i = 0; i < allStudents.size(); i++)
		{			
			allStudents.values().at(i)->setHidden(false);			
		}
		CMessageBox::showMessage(
			QString("����ʱ��"),
			QString("������Ҫ���ҵ�����"),
			QString("ȷ��"),
			QString());
	}
	else
	{
		int num = 0;
		for (int i = 0; i < allStudents.size(); i++)
		{
			if (allStudents.keys().at(i) != name)
			{
				allStudents.values().at(i)->setHidden(true);
				num++;
			}

		}
		if (num == allStudents.size())
		{
			for (int i = 0; i < allStudents.size(); i++)
			{
				allStudents.values().at(i)->setHidden(false);
			}
			CMessageBox::showMessage(
				QString("����ʱ��"),
				QString("û���������"),
				QString("ȷ��"),
				QString());
		}
	}
	
}
//���������
void personList::slotRenameEditFshed()
{
    if(groupNameEdit->text()!=NULL)      //����������༭��Ϊ��
        currentItem->setText(groupNameEdit->text());  //��������
    groupNameEdit->setText("");
    groupNameEdit->hide();  //�����������༭��
}
//����ѧ��
void personList::chickChage(bool b, QString ID, QString name)
{
	emit signalChickChage(b, ID, name);
}
//�������е�ѧ��
void personList::stopAllTalk(bool b, QString ID)
{
	emit signalAllTalk(b, ID);
}
void personList::setOlineNum(int olineNum, int allNum)
{
	buddyFirst->setOlineNum(olineNum, allNum);
}

void personList::cleanStudents()
{
	for (int i = 0; i < groupMap.size(); i++)
	{
		if (groupMap.keys().at(i))
		{
			delete (groupMap.keys().at(i));
		}		
	}	
	groupMap.clear();
	allStudents.clear();
	this->clear();
	initFronUi();
	initSecUi();
}