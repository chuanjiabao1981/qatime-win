#include "UIViewimage.h"
#include <QDesktopWidget>
#include <QStandardPaths>
#include "windows.h"

UIViewImage::UIViewImage(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.scrollArea->setBackgroundRole(QPalette::Dark);
}

UIViewImage::~UIViewImage()
{

}

void UIViewImage::setImage(QString imgPath)
{
	imgPath.replace("/", "\\");
	QPixmap pixmap(imgPath);
	int iWidth = pixmap.width();
	int iHeight = pixmap.height();

	TCHAR path[MAX_PATH] = { 0 };
	TCHAR Dir[MAX_PATH] = { 0 };
	GetSystemDirectory(Dir, MAX_PATH);
	lstrcat(path, Dir);
	lstrcat(path, L"\\rundll32.exe");

	QString sDir = QString::fromStdWString(Dir);;
	QString strParm;
	strParm.append(QString("%1\\shimgvw.dll imageview_fullscreen %2").arg(sDir).arg(imgPath));
	LPCTSTR lpstrParm = (LPCTSTR)strParm.utf16();
	HINSTANCE hNewExe = ShellExecute(NULL, L"Open", path, lpstrParm, NULL, SW_SHOW);
 	if ((DWORD)hNewExe <= 32)
 	{
		int error = (DWORD)hNewExe;
 	}
}
