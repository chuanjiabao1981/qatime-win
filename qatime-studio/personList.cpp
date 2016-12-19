
#include "personlist.h"
#include <QAction>
#include <QIcon>
#include "UIMessageBox.h"
#pragma execution_character_set("utf-8")
personList::personList(QWidget *parent) :
    QListWidget(parent)
	, currentItem(NULL)
	, buddyFirst(NULL)
	, buddySec(NULL)
	, buddyLast(NULL)
	, backImage(NULL)
{
    setFocusPolicy(Qt::NoFocus);       // 去除item选中时的虚线边框
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//水平滚动条关闭
    initMenu();	

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(timeOutSet()));
}
//初始化菜单
void personList::initMenu()
{
    //初始化：
    blankMenu = new QMenu();
    groupMenu = new QMenu();
    personMenu = new QMenu();
    groupNameEdit=new QLineEdit();
	QAction *rename = new QAction("重命名", this);
	QAction *addBuddy = new QAction("添加好友", this);
//    QAction *addGroup = new QAction("添加分组", this);
//    QAction *delGroup = new QAction("删除该组", this);    
//    QAction *delBuddy = new QAction("删除好友", this);
    //设置：
    groupNameEdit->setParent(this);  //设置父类
    groupNameEdit->hide(); //设置初始时隐藏
    groupNameEdit->setPlaceholderText("未命名");//设置初始时的内容
    //布局：
	blankMenu->addAction(addBuddy);
    groupMenu->addAction(rename);
    groupMenu->addAction(addBuddy);
 //   personMenu->addAction(delBuddy);
//   groupMenu->addAction(delGroup);
    //信息槽：
    connect(groupNameEdit,SIGNAL(editingFinished()),this,SLOT(slotRenameEditFshed()));
	connect(rename, SIGNAL(triggered()), this, SLOT(slotRename()));
	connect(addBuddy, SIGNAL(triggered()), this, SLOT(slotAddBuddy()));
//    connect(addGroup,SIGNAL(triggered()),this,SLOT(slotAddGroup()));
//    connect(delGroup,SIGNAL(triggered()),this,SLOT(slotDelGroup()));  
//    connect(delBuddy,SIGNAL(triggered()),this,SLOT(slotDelBuddy()));

}
//鼠标点击事件
void personList::mousePressEvent(QMouseEvent *event)
{
    QListWidget::mousePressEvent(event); // 如果不调用基类mousePressEvent，item被select会半天不响应,调用父类，让QSS起效，因为QSS基于父类QListWidget，子类就是子窗口，就是最上层窗口，是覆盖在父窗口上的，所以先于父窗口捕获消息
    //防止一种特殊情况：给新item命名、点击其他item或空白处时，指向新item的currentItem被赋予其他item
//     if(groupNameEdit->isVisible() && !(groupNameEdit->rect().contains(event->pos())))
//     {
//         if(groupNameEdit->text()!=NULL)
//             currentItem->setText(groupNameEdit->text());
//         groupNameEdit->setText("");
//         groupNameEdit->hide();
//     }
//     currentItem = this->itemAt(mapFromGlobal(QCursor::pos()));//鼠标位置的Item，不管右键左键都获取
//     if(event->button()==Qt::LeftButton && currentItem!=NULL && currentItem==groupMap.value(currentItem))//如果点击的左键并且是点击的是组
//     {
//         if(isHideMap.value(currentItem))                                  //如果先前是隐藏，则显示
//         {
//             foreach(QListWidgetItem* subItem, groupMap.keys(currentItem))//遍历组的对应的项（包括自身和好友）
//                 if(subItem!=currentItem)                                 //如果是组的话不进行处理
//                 {
//                     subItem->setHidden(false);                            //好友全部显示
//                 }
//             isHideMap.insert(currentItem,false);                          //设置该组为显示状态
//             currentItem->setIcon(QIcon(":/arrowDown"));
//         }
//         else                                                             //否则，先前是显示，则隐藏
//         {
//             foreach(QListWidgetItem* subItem, groupMap.keys(currentItem))//遍历组的对应的项（包括自身和好友）
//                 if(subItem!=currentItem)                                 //如果是组的话不进行处理
//                 {
//                     subItem->setHidden(true);                            //好友全部隐藏
//                 }
//              isHideMap.insert(currentItem,true);                          //设置该组为隐藏状态
//              currentItem->setIcon(QIcon(":/arrowRight"));
//         }
//     }
}
//菜单事件，为了显示菜单，点击鼠标右键响应，鼠标点击事件mousePressEvent优先于contextMenuEvent
void personList::contextMenuEvent(QContextMenuEvent *event)
{
    QListWidget::contextMenuEvent(event);           //调用基类事件
//     if(currentItem==NULL)                           //如果点击到的是空白处
//     {
//         blankMenu->exec(QCursor::pos());
//         return;
//     }
//     if(currentItem==groupMap.value(currentItem))    // 如果点击到的是组
//         groupMenu->exec(QCursor::pos());
//     else                                            //否则点击到的是好友
//         personMenu->exec(QCursor::pos());
}
//添加组
void personList::slotAddGroup()
{
//     QListWidgetItem *newItem=new QListWidgetItem(QIcon(":/arrowRight"),QString("未命名"));    //创建一个Item
//     newItem->setSizeHint(QSize(this->width(),25));//设置宽度、高度
//     this->addItem(newItem);         //加到QListWidget中
//     groupMap.insert(newItem,newItem);//加到容器groupMap里，key和value都为组
//     isHideMap.insert(newItem,true);  //设置该组隐藏状态
//     groupNameEdit->raise();
//     groupNameEdit->setText(QString("未命名")); //设置默认内容
//     groupNameEdit->selectAll();        //设置全选
//     groupNameEdit->setGeometry(this->visualItemRect(newItem).left()+15,this->visualItemRect(newItem).top()+1,this->visualItemRect(newItem).width(),this->visualItemRect(newItem).height()-2);//出现的位置
//     groupNameEdit->show();              //显示
//     groupNameEdit->setFocus();          //获取焦点
//     currentItem = newItem;	   // 因为要给group命名，所以当前的currentItem设为该group
}
//删除组
void personList::slotDelGroup()
{
//     foreach(QListWidgetItem* item, groupMap.keys(currentItem))  //遍历该组的所有好友和自身的组
//     {
//         groupMap.remove(item);   //移除
//         delete item;   //删除
//     }
//     isHideMap.remove(currentItem); //移除
}
//重命名
void personList::slotRename()
{
    groupNameEdit->raise();
    groupNameEdit->setGeometry(this->visualItemRect(currentItem).left()+15,this->visualItemRect(currentItem).top()+1,this->visualItemRect(currentItem).width(),this->visualItemRect(currentItem).height()-2);//出现的位置
    groupNameEdit->setText(currentItem->text());   //获取该组名内容
    groupNameEdit->show();                        //显示
    groupNameEdit->selectAll();                   //全选
    groupNameEdit->setFocus();                        //获取焦点
}

void personList::initFronUi()
{
	buddyFirst = new personListBuddy();
	buddyFirst->initFirst();		
	QListWidgetItem *newItem = new QListWidgetItem();
	newItem->setSizeHint(QSize(300, 30));
	this->insertItem(0, newItem);
	this->setItemWidget(newItem, buddyFirst);
	newItem->setHidden(false);	
}
void personList::initSecUi()
{
	buddySec = new personListBuddy();	
	buddySec->initFindPeople();
	connect(buddySec, SIGNAL(signalFindName(const QString)), this, SLOT(findName(const QString)));
	connect(buddySec, SIGNAL(signalFindNameNULL(const QString)), this, SLOT(findName(const QString)));	
	QListWidgetItem *newItem = new QListWidgetItem();
	newItem->setSizeHint(QSize(30, 30));
	this->insertItem(1 , newItem);
	this->setItemWidget(newItem, buddySec);	
	newItem->setHidden(false);
	buddySec->secLinEdit->raise();
	newItem->setBackgroundColor(QColor(153,153,153));

	buddyLast = new personListBuddy();
	buddyLast->initNotFind();	
	connect(buddyLast, SIGNAL(signalStopAllTalk(bool, QString)), this, SLOT(stopAllTalk(bool, QString)));
	QListWidgetItem *newItemLast = new QListWidgetItem();
	newItemLast->setSizeHint(QSize(30, 30));
	this->insertItem(2, newItemLast);
	this->setItemWidget(newItemLast, buddyLast);
	newItemLast->setHidden(false);
}
void personList::setStrdentNumbers(int num)
{
//	buddyFirst->setOlineNum(num, 0);
}
void personList::addStrdent(QString imagesUrl,QString stuName,QString ID)
{
	personListBuddy *buddy = new personListBuddy();
	buddy->initUi(imagesUrl, stuName, ID);
	connect(buddy, SIGNAL(emitRadioChange(int, QString, QString)), this, SLOT(chickChage(int, QString, QString)));
	QList<QListWidgetItem*> tem = groupMap.keys(buddy);
	QListWidgetItem *newItem = new QListWidgetItem();       
	newItem->setSizeHint(QSize(30, 30));
	int num = row(currentItem) + tem.count();
	this->insertItem(num + 4, newItem);
	this->setItemWidget(newItem, buddy);
	groupMap.insert(newItem, buddy);
	allStudents.insert(stuName, newItem);
	IdStudents.insert(ID, newItem);
	newItem->setHidden(false);
}
void personList::slotAddBuddy()
{  
//    buddy->sign->setText("用通俗的语言，讲深刻的技术。");   //设置个性签名
//     QList<QListWidgetItem*> tem = groupMap.keys(currentItem);//当前组对应的项（包括组本身和好友）复制给tem
//     //关键代码
//     QListWidgetItem *newItem = new QListWidgetItem();       //创建一个newItem
// 	newItem->setSizeHint(QSize(30, 30));
//     this->insertItem(row(currentItem)+tem.count(),newItem); //将该newItem插入到后面
//     this->setItemWidget(newItem, buddy); //将buddy赋给该newItem
//     groupMap.insert(newItem,currentItem);   //加进容器，key为好友，value为组
//     if(isHideMap.value(currentItem))          //如果该组是隐藏，则加进去的好友设置为隐藏
//         newItem->setHidden(true);
//     else                                      //否则，该好友设置为显示
//         newItem->setHidden(false);
}
//删除好友
void personList::slotDelBuddy()
{
    groupMap.remove(currentItem);  //移除该好友
    delete currentItem;            //删除
}
//查找好友
void personList::findName(const QString name)
{
	if (name.isEmpty())
	{
		for (int i = 0; i < allStudents.size(); i++)
		{			
			allStudents.values().at(i)->setHidden(false);			
		}
// 		buddyLast->notFind->setText("输入姓名，查找！");
// 		buddyLast->notFind->show();
// 		timer->start(3000);
		buddyLast->firstButton->show();
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
				allStudents.values().at(i)->setHidden(true);
			}
			buddyLast->notFind->setText("没有找到该学生！");
			buddyLast->notFind->show();
			buddyLast->firstButton->hide();
			timer->start(3000);
		}
		else
		{
			buddyLast->notFind->hide();
		}
	}
	
}
//重命名完成
void personList::slotRenameEditFshed()
{
    if(groupNameEdit->text()!=NULL)      //如果重命名编辑框不为空
        currentItem->setText(groupNameEdit->text());  //更新组名
    groupNameEdit->setText("");
    groupNameEdit->hide();  //隐藏重命名编辑框
}
//禁言学生
void personList::chickChage(int b, QString ID, QString name)
{
	emit signalChickChage(b, ID, name);
}
//禁言所有的学生
void personList::stopAllTalk(bool b, QString ID)
{
	for (int i = 0; i < groupMap.size();i++)
	{
		groupMap.values().at(i)->button->setChecked(b);
	}
}

void personList::timeOutSet()
{
	buddyLast->notFind->setText(" ");
	timer->stop();
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
			delete (groupMap.values().at(i));
		}		
	}	
	groupMap.clear();
	allStudents.clear();
	IdStudents.clear();
	this->clear();
//	initFronUi();
	initSecUi();
}

personListBuddy* personList::findID(const QString id)
{
	personListBuddy* Buddy = NULL;

	if (!id.isEmpty() && !IdStudents.isEmpty() && !groupMap.isEmpty())
	{
		QMap<QString, QListWidgetItem*>::iterator iter;
		QListWidgetItem* item = NULL;
		iter = IdStudents.find(id);
		if (iter == IdStudents.end())
		{
			return Buddy;
		}
		else
		{
			item = *iter;
			Buddy = *groupMap.find(item);
		}
	}
	
	return Buddy;
}

// 拉伸屏幕宽度，按钮也需要改变位置
void  personList::setAllWidth(int iWidth)
{
	if (iWidth == 0)
		return;

	int iCount = this->count();
	if (iCount == 0)
		return;

	personListBuddy* Buddy = NULL;
	for (int i = 2; i < iCount;i++)
	{
		QListWidgetItem* pItem = this->item(i);
		if (pItem)
		{
			Buddy = *groupMap.find(pItem);
			if (Buddy)
			{
				QRect rc;
				if (Buddy->button)
				{
					rc = Buddy->button->geometry();
					Buddy->button->move(rc.left() + iWidth, rc.top());
					Buddy->setFixedWidth(Buddy->width() + iWidth);
				}
			}
		}
	}

	if (buddySec)
	{
		buddySec->secLinEdit->setFixedWidth(buddySec->secLinEdit->width() + iWidth);
		QRect rc;
		rc = buddySec->secFindButton->geometry();
		buddySec->secFindButton->move(rc.left() + iWidth, rc.top());
		buddySec->setFixedWidth(buddySec->width() + iWidth);
	}

	if (buddyLast)
	{
		QRect rc;
		rc = buddyLast->firstButton->geometry();
		buddyLast->firstButton->move(rc.left() + iWidth, rc.top());
		buddyLast->setFixedWidth(buddyLast->width() + iWidth);
	}
}