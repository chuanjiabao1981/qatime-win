#ifndef CLICKEDLABEL_H_
#define CLICKEDLABEL_H_
#include <QLabel>
#include <QWidget>
class myLabel : public QLabel
{
    Q_OBJECT
signals:
    void Clicked();
public:
	myLabel(QWidget *parent = 0) : QLabel(parent) {};
	~myLabel() {};
protected:
    void mousePressEvent( QMouseEvent* );
private:
    QString m_str;
};
#endif /* CLICKEDLABEL_H_ */
