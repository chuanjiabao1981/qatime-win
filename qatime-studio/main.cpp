#include "loginwindow.h"
#include <QtWidgets/QApplication>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QIcon>
#include "app_dump.h"
#include <gdiplus.h>
using namespace Gdiplus;

void registerMetaType()
{
	//注册该类型   用于非Qt类类型和非基本数据类型参数实现信号槽连接
	qRegisterMetaType<std::string>("std::string");
	qRegisterMetaType<__int64>("__int64");
	qRegisterMetaType<int64_t>("int64_t");
}

void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	static QMutex mutex;
	mutex.lock();

	QString text;
	switch (type)
	{
	case QtDebugMsg:
		text = QString("Debug:");
		break;

	case QtWarningMsg:
	{
		text = QString("Warning:");
		mutex.unlock();
		return;
		break;
	}
	case QtCriticalMsg:
		text = QString("Critical:");
		break;

	case QtFatalMsg:
		text = QString("Fatal:");
	}

	QString context_info = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
	QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
	QString current_date = QString("(%1)").arg(current_date_time);
	QString message = QString("%1 %2 %3 %4").arg(text).arg(context_info).arg(msg).arg(current_date);

	QFile file("log.txt");
	file.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream text_stream(&file);
	text_stream << message << "\r\n";
	file.flush();
	file.close();

	mutex.unlock();
}

int main(int argc, char *argv[])
{
// 	HANDLE hMutex = CreateMutex(NULL, TRUE, TEXT("QtTeacher_Mutex"));
// 	//已存在互斥量说明在运行  
// 	if (ERROR_ALREADY_EXISTS == GetLastError())
// 	{
// 		CloseHandle(hMutex);
// 		hMutex = NULL;
// 		return 0;
// 	}

	::SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;

	//注册MessageHandler
	qInstallMessageHandler(outputMessage);

	registerMetaType();
	//打印日志到文件中
	qDebug("start exe");

	QApplication a(argc, argv);
	QIcon qIcon("./images/favicon.png");
	a.setWindowIcon(qIcon);

 	//初始化GDI+.
 	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	LoginWindow w;
	w.setWindowFlags(Qt::FramelessWindowHint);
	w.ReadSetting();

	if (w.IsAutoLogin())
		w.hide();
	else
		w.show();
	return a.exec();
}
