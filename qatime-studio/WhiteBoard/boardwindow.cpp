#include "Boardwindow.h"
#include "ui_Boardwindow.h"
#include <QtGui>
#include "scribblearea.h"
#include <QStatusBar>

BoardWindow::BoardWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BoardWindow)
{
    ui->setupUi(this);
	ui->statusBar->hide();

    scribbleArea = new ScribbleArea(this);
    setCentralWidget(scribbleArea);

    createActions();
//    createMenus();
    createToolBars();
	resize(960, 540);

	scribbleArea->show();
	scribbleArea->raise();
}

BoardWindow::~BoardWindow()
{
    delete ui;
}
void BoardWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void BoardWindow::open()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this,
                                   tr("Open File"), QDir::currentPath());
        if (!fileName.isEmpty())
            scribbleArea->openImage(fileName);
    }
}

void BoardWindow::save()
{
    QAction *action = qobject_cast<QAction *>(sender());
    QByteArray fileFormat = action->data().toByteArray();
    saveFile(fileFormat);
}
//打开word等文件
void BoardWindow::openWord()
{
    scribbleArea->show();
    scribbleArea->raise();

}
void BoardWindow::penColor()
{
    QColor newColor = QColorDialog::getColor(scribbleArea->penColor());
    if (newColor.isValid())
        scribbleArea->setPenColor(newColor);
}
void BoardWindow::penBrush()
{
    QColor newBrush = QColorDialog::getColor(scribbleArea->penColor());
    if (newBrush.isValid())
        scribbleArea->setPenBrush(newBrush);
}

void BoardWindow::penWidth()
{
    bool ok;
    int newWidth = QInputDialog::getInt(this, tr("画笔 "),
                                            tr(" 画笔大小: "),
                                            scribbleArea->penWidth(),
											1, 50, 1, &ok, Qt::WindowCloseButtonHint);
    if (ok)
        scribbleArea->setPenWidth(newWidth);
}



void BoardWindow::createActions()
{
    openAct = new QAction(tr("&打开..."), this);
    openAct->setShortcut(tr("Ctrl+O"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    foreach (QByteArray format, QImageWriter::supportedImageFormats())
       {
        QString text = tr("%1...").arg(QString(format).toUpper());

        QAction *action = new QAction(text, this);
        action->setData(format);
        connect(action, SIGNAL(triggered()), this, SLOT(save()));
        saveAsActs.append(action);
       }

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    penColorAct = new QAction(tr("&画笔颜色..."), this);
    connect(penColorAct, SIGNAL(triggered()), this, SLOT(penColor()));

    offiec = new QAction(tr("&加载word"), this);
    connect(offiec, SIGNAL(triggered()), this, SLOT(openWord()));


//    penBrushAct = new QAction(tr("&画刷"), this);
//    connect(penBrushAct, SIGNAL(triggered()), this, SLOT(penBrush()));

    penWidthAct = new QAction(tr("&画笔大小"), this);
    connect(penWidthAct, SIGNAL(triggered()), this, SLOT(penWidth()));

    clearScreenAct = new QAction(tr("&清屏"), this);
    clearScreenAct->setShortcut(tr("Ctrl+L"));
    connect(clearScreenAct, SIGNAL(triggered()),
            scribbleArea, SLOT(clearImage()));

    nullAct = new QAction(tr("& 点线 "), this);
    connect(nullAct, SIGNAL(triggered()),
            scribbleArea, SLOT(setStatusNull()));

    drawLineAct = new QAction(tr("&划线"), this);
    connect(drawLineAct, SIGNAL(triggered()),
            scribbleArea, SLOT(setStatusDrawLine()));

    drawRectAct = new QAction(tr("&矩形"), this);
    connect(drawRectAct, SIGNAL(triggered()),
            scribbleArea, SLOT(setStatusDrawRect()));

    drawRoundRectAct = new QAction(tr("&圆边矩形"), this);
    connect(drawRoundRectAct, SIGNAL(triggered()),
            scribbleArea, SLOT(setStatusDrawRoundRect()));

    drawEllipseAct = new QAction(tr("&圆形"), this);
    connect(drawEllipseAct, SIGNAL(triggered()),
            scribbleArea, SLOT(setStatusDrawEllipse()));

//     circleEraseAct = new QAction(tr("&CircleErase"), this);
//     connect(circleEraseAct, SIGNAL(triggered()),
//             scribbleArea, SLOT(setStatusCircleErase()));
// 
//     rectEraseAct = new QAction(tr("&RectErase"), this);
//     connect(rectEraseAct, SIGNAL(triggered()),
//             scribbleArea, SLOT(setStatusRectErase()));

	insertTextAct = new QAction(tr("&插入文字"), this);
	connect(insertTextAct, SIGNAL(triggered()),
		scribbleArea, SLOT(setStatusText()));
}

void BoardWindow::createMenus()
{
    saveAsMenu = new QMenu(tr("&Save As"), this);
    foreach (QAction *action, saveAsActs)
        saveAsMenu->addAction(action);

    fileMenu = new QMenu(tr("&文件"), this);
    fileMenu->addAction(openAct);
    fileMenu->addMenu(saveAsMenu);

    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    optionMenu = new QMenu(tr("&工具"), this);
    optionMenu->addAction(penColorAct);
//    optionMenu->addAction(penBrushAct);
    optionMenu->addAction(penWidthAct);
    optionMenu->addSeparator();

    optionMenu->addAction(nullAct);
    optionMenu->addAction(drawLineAct);
    optionMenu->addAction(drawRectAct);
    optionMenu->addAction(drawRoundRectAct);
    optionMenu->addAction(drawEllipseAct);
    optionMenu->addSeparator();
//     optionMenu->addAction(circleEraseAct);
//     optionMenu->addAction(rectEraseAct);
	optionMenu->addAction(insertTextAct);
    optionMenu->addAction(clearScreenAct);

    helpMenu = new QMenu(tr("&Help"), this);

    word = new QMenu(tr("&加载"));
    word->addAction(offiec);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(optionMenu);
    menuBar()->addMenu(helpMenu);
    menuBar()->addMenu(word);
}

void BoardWindow::createToolBars()
{

   option1ToolBar = addToolBar(tr("Option1"));
   option1ToolBar->addAction(penColorAct);
//   option1ToolBar->addAction(penBrushAct);
   option1ToolBar->addAction(penWidthAct);
   option2ToolBar = addToolBar(tr("Option2"));
   option2ToolBar->addAction(nullAct);
   option2ToolBar->addAction(drawLineAct);
   option2ToolBar->addAction(drawRectAct);
   option2ToolBar->addAction(drawRoundRectAct);
   option2ToolBar->addAction(drawEllipseAct);
   option3ToolBar = addToolBar(tr("Option3"));
//    option3ToolBar->addAction(circleEraseAct);
//    option3ToolBar->addAction(rectEraseAct);
   option3ToolBar->addAction(insertTextAct);
   option3ToolBar->addAction(clearScreenAct);

 //  option1ToolBar->setAttribute(Qt::BottomToolBarArea);
 //  option1ToolBar->setAllowedAreas( Qt::BottomToolBarArea);

 //  option1ToolBar->setCorner(Qt::TopLeftCorner,Qt::LeftDockWidgetArea);
}



bool BoardWindow::maybeSave()
{
    if (scribbleArea->isModified()) {
       QMessageBox::StandardButton ret;
       ret = QMessageBox::warning(this, tr("Scribble"),
                          tr("The image has been modified.\n"
                             "Do you want to save your changes?"),
                          QMessageBox::Save | QMessageBox::Discard
                          | QMessageBox::Cancel);
        if (ret == QMessageBox::Save) {
            return saveFile("png");
        } else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

bool BoardWindow::saveFile(const QByteArray &fileFormat)
{
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                               initialPath,
                               tr("%1 Files (*.%2);;All Files (*)")
                               .arg(QString(fileFormat.toUpper()))
                               .arg(QString(fileFormat)));
    if (fileName.isEmpty()) {
        return false;
    } else {
        return scribbleArea->saveImage(fileName, fileFormat);
    }
}