#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QFileDialog>
#include<QColorDialog>
#include<QFileDialog>
#include<QMessageBox>
#include<QInputDialog>
class ScribbleArea;
class QToolBar;

namespace Ui {
class BoardWindow;

}

class BoardWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BoardWindow(QWidget *parent = 0);
    ~BoardWindow();


private:
    Ui::BoardWindow *ui;
protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void open();
    void save();
    void penColor();
    void penWidth();
    void penBrush();
    void openWord();

private:
    void createActions();
    void createMenus();
    void createToolBars();

    bool maybeSave();
	bool saveFile(const QByteArray &fileFormat);

    ScribbleArea *scribbleArea;

    QMenu *saveAsMenu;
    QMenu *word;
    QMenu *fileMenu;
    QMenu *optionMenu;
    QMenu *helpMenu;

    QAction *openAct;
    QList<QAction *> saveAsActs;
    QAction *exitAct;
    QAction *penColorAct;
    QAction *penWidthAct;
    QAction *penBrushAct;
    QAction *offiec;//加载offiec
    QAction *clearScreenAct;
    QAction *nullAct;
    QAction *drawLineAct;
    QAction *drawRectAct;
    QAction *drawRoundRectAct;
    QAction *drawEllipseAct;
    QAction *circleEraseAct;
    QAction *rectEraseAct;
	QAction *insertTextAct;

    QToolBar *option1ToolBar;
    QToolBar *option2ToolBar;
    QToolBar *option3ToolBar;

//    Form     *from;//word等文档

};

#endif // BoardWINDOW_H
