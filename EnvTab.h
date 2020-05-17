#pragma once

#include <QWidget>
#include <QStandardItemModel>
#include <QModelIndex>
#include "ui_EnvTab.h"

class StateMachine;

class EnvTab : public QWidget
{
	Q_OBJECT

public:
	EnvTab(QWidget *parent = Q_NULLPTR);
	~EnvTab();

private:
	Ui::EnvTab ui;
	StateMachine * sys;
	QStandardItemModel *itemMdl;

protected slots:
	void SetLight();
	void SetSkyBox();
	void SetShadow();
	void ChangeRenderScene();
	void SelectionChange(const QModelIndex & current);
};
