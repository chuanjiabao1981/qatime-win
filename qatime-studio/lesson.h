#ifndef LESSON_H
#define LESSON_H

#include <QJsonObject>

#pragma execution_character_set("utf-8")

class Lesson
{
public:
    Lesson();
    void readJson(const QJsonObject &json);
    int courseId();
    void setCourseId(int courseId);
    QString name();
	QString LessonTime();
	QString LessonStatus();
	QString ChinaLessonStatus();
	QString LessonID();
	QString CourseID();
	QString CourseName();
	QString Date();

	QString BoardUrl();
	QString CameraUrl();
private:
    int mId;
    int mCourseId;
    QString mName;			// �γ�����
	QString mStatus;		// �γ�״̬
	QString mStartTime;		// �Ͽ�ʱ��
	QString mCourseID;		// ������ID
	QString mCourseName;	// ����������
	QString	mDate;			// ���� ��2017-2-28��

	QString	mBoardUrl;		// �װ�URL
	QString mCameraUrl;		// ����ͷURL
};

#endif // LESSON_H
