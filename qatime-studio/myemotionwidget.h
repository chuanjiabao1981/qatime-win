#ifndef MYEMOTIONWIDGET_H
#define MYEMOTIONWIDGET_H

#include <QWidget>
#include <QTableWidget>

class MyEmotionWidget : public QTableWidget
{
	Q_OBJECT

public:
	MyEmotionWidget(QWidget *parent = 0);
	~MyEmotionWidget();

	void setRowAndColumn(int row , int column);
	void setEmotionSize(QSize emotionSize);
	void setEmotionMovieSize(QSize emotionMovieSize);
	void setMaxRow(int maxRow);
	void addEmotionItem(QString fileName, QString toolTip = "");

	void initTableWidget();

	void showDynamicEmotion(QPoint cursorPos);
private:
	void loadStyleSheet();
	void initWindow();
	bool eventFilter(QObject *obj, QEvent *event);
	bool nativeEvent(const QByteArray &eventType, void *message, long *result);
public slots:
	void getFileName(QString fileName);
signals:
	void emitFileName(QString fileName);
private:

	int m_preRow;
	int m_preColumn;
	int m_tableRow;
	int m_tableColumn;
	int m_maxRow;
	QSize m_emotionSize;
	QSize m_emotionMovieSize;
	QList<QString> m_emotionList;
};

#endif // MYEMOTIONWIDGET_H