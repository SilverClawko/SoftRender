#pragma once

#include <QDialog>
#include "ui_MaterialDialog.h"

class Material;
class Texture2D;
class StateMachine;

class MaterialDialog : public QDialog
{
	Q_OBJECT

public:
	MaterialDialog(Material * m,QWidget *parent = Q_NULLPTR);
	~MaterialDialog();

private:
	Ui::MaterialDialog ui;
	StateMachine * sys;
	Material * material;
	void ShowTexture(Texture2D * t,QLabel * cav);
	bool init;

protected slots:
	void OpenMainTexture();
	void OpenNormalTexture();
	void OpenMetallicTexture();
	void OpenRoughnessTexture();
	void OpenAOTexture();
	void RemoveMainTexture();
	void RemoveNormalTexture();
	void RemoveMetallicTexture();
	void RemoveRoughnessTexture();
	void RemoveAOTexture();
	void SetMaterial();
};
