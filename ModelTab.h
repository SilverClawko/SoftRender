#pragma once

#include <QWidget>
#include <QStandardItemModel>
#include <QModelIndex>
#include "ui_ModelTab.h"
#include "MaterialDialog.h"

class StateMachine;

class ModelTab : public QWidget
{
	Q_OBJECT

public:
	ModelTab(QWidget *parent = Q_NULLPTR);
	~ModelTab();

protected slots:
	void OpenModel();
	void RemoveSelectedModel();
	void SelectionChange(const QModelIndex & current);
	void ChangeRenderMode();
	void ChangeModelMatrix(double x);
	void SetMaterial();

private:
	Ui::ModelTab ui;
	StateMachine * sys;

	QStandardItemModel *itemMdl;

	void UpdateText();
	void UpdateSpinBox();
};
