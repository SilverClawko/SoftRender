#pragma once

#include <QtWidgets/QWidget>
#include <QString>
#include <QImage>
#include <QPainter>
#include <QThread>
#include <QTimer>
#include <QKeyEvent>
#include <QVector>
#include "RenderLoop.h"
#include "ui_RenderWidget.h"
#include "ModelTab.h"
#include "EnvTab.h"


class StateMachine;

class RenderWidget : public QWidget
{
	Q_OBJECT

public:
	explicit RenderWidget(QWidget *parent = Q_NULLPTR);
	~RenderWidget();
//public slots:
	//void OnClick1(void);
	//void OnClick2(void);
	//void OnClick3(void);

protected slots:
	void Vsync(unsigned char *image, const unsigned int &num_triangles,
		const unsigned int &num_vertices);
	void GetFps();
	void DealInput();

protected:
	void mouseMoveEvent(QMouseEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);
	void wheelEvent(QWheelEvent *event);
	void RenderWidget::paintEvent(QPaintEvent *event) override;

private:
	Ui::RenderWidgetClass ui;
	QImage * canvas;
	RenderLoop * renderLoop;
	QThread * renderThread;
	QTimer * timer;
	QTimer * inputTimer;
	QVector<Qt::Key> pressedKeys;
	bool firstMouseMove;
	QPoint preMousePos;
	int fps;
	StateMachine * sys;

	ModelTab * mdTab;
	EnvTab * envTab;
};
