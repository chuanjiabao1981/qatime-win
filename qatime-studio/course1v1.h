#ifndef Course1v1_H
#define Course1v1_H

#include <QJsonObject>
#include <QJsonArray>

class Course1v1
{
public:
    Course1v1();

    QString		id();
    QString		name() const;
	QString		progress() const;
	QString		status();
    void		readJson(const QJsonObject &json);
	QJsonArray  JsonLesson();
	QString		url();
	QString		CameraURL();
	QString		ChatId();
	QString		Grade();
	QString		TeacherName();
	QString		PicUrl();
private:
    int			mId;
    QString		mName;				// ����������
    QString		mStatus;
	QString		mProgress;			// ����
	QJsonArray	mJsonArray;			// �γ�json
	QString		mURL;				// �γ�������URL
	QString		mCameraURL;			// ����ͷ������ַ		
	QString		mChatId;			// ���ŻỰID
	QString     mGrade;				// ��Ŀ
	QString     mTeacherName;		// ��ʦ����
	QString		mPic;				// ������ͼƬ��ַ
};

#endif // Course1v1_H
