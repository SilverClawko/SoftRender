#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QListView>
#include <QDoubleSpinBox>
#include "ModelTab.h"
#include "StateMachine.h"
#include "Model.h"

bool changed = false;

ModelTab::ModelTab(QWidget *parent)
	: QWidget(parent), itemMdl(nullptr)
{
	ui.setupUi(this);

	sys = StateMachine::GetInstance();

	connect(ui.openFileButton, &QPushButton::clicked, this, &ModelTab::OpenModel);
	connect(ui.removeButton, &QPushButton::clicked, this, &ModelTab::RemoveSelectedModel);
	connect(ui.mdlListView, &QListView::clicked, this, &ModelTab::SelectionChange);
	connect(ui.rdMdCB, &QCheckBox::clicked, this, &ModelTab::ChangeRenderMode);
	connect(ui.matButton, &QPushButton::clicked, this, &ModelTab::SetMaterial);

	connect(ui.posXSpin, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &ModelTab::ChangeModelMatrix);
	connect(ui.posYSpin, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &ModelTab::ChangeModelMatrix);
	connect(ui.posZSpin, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &ModelTab::ChangeModelMatrix);
	connect(ui.rotXSpin, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &ModelTab::ChangeModelMatrix);
	connect(ui.rotYSpin, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &ModelTab::ChangeModelMatrix);
	connect(ui.rotZSpin, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &ModelTab::ChangeModelMatrix);
	connect(ui.scaleSpin, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &ModelTab::ChangeModelMatrix);

	ui.mdlListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.posXSpin->setDisabled(true);
	ui.posYSpin->setDisabled(true);
	ui.posZSpin->setDisabled(true);
	ui.rotXSpin->setDisabled(true);
	ui.rotYSpin->setDisabled(true);
	ui.rotZSpin->setDisabled(true);
	ui.scaleSpin->setDisabled(true);
	ui.matButton->setDisabled(true);

	UpdateText();
}

ModelTab::~ModelTab()
{
	sys = nullptr;
}


void ModelTab::OpenModel()
{
	QFileDialog *fileDialog = new QFileDialog(this);
	fileDialog->setWindowTitle(QStringLiteral("打开模型"));
	fileDialog->setDirectory(".");
	fileDialog->setNameFilter(tr("Obj File(*.obj)"));
	fileDialog->setFileMode(QFileDialog::ExistingFiles);
	fileDialog->setViewMode(QFileDialog::Detail);
	QStringList fileNames;
	if (fileDialog->exec()) {
		fileNames = fileDialog->selectedFiles();
		for (int i = 0; i < fileNames.size(); i++) {
			Model * tmp = new Model(fileNames[i].toStdString());
			sys->AddModel(tmp);
		}
	}
	UpdateText();
}

int SELECTED_MODEL = -1;
int SELECTED_MESH = -1;

void ModelTab::RemoveSelectedModel()
{
	ui.removeButton->setDisabled(true);
	if (SELECTED_MODEL == -1 && SELECTED_MESH == -1) {
		ui.removeButton->setDisabled(false);
		return;
	}
	if (SELECTED_MESH != -1) {
		sys->mutex.lock();
		sys->models[SELECTED_MODEL]->RemoveMesh(SELECTED_MESH);
		if (sys->models[SELECTED_MODEL]->meshes.empty())
			sys->RemoveModel(SELECTED_MODEL);
		UpdateText();
		sys->mutex.unlock();
		ui.removeButton->setDisabled(false);
		ui.matButton->setDisabled(true);
		return;
	}
	if (SELECTED_MODEL != -1) {
		sys->mutex.lock();
		sys->RemoveModel(SELECTED_MODEL);
		UpdateText();
		sys->mutex.unlock();
		ui.removeButton->setDisabled(false);
		ui.matButton->setDisabled(true);
		return;
	}
	ui.removeButton->setDisabled(false);
}

void ModelTab::SelectionChange(const QModelIndex & current)
{
	int index = current.row();
	if (index == -1) {
		ui.matButton->setDisabled(true);
		UpdateSpinBox();
		return;
	}
	int sb = 0;
	for (int i = 0; i < sys->models.size(); i++) {
		if (sb == index) {
			SELECTED_MODEL = i;
			SELECTED_MESH = -1;
			ui.matButton->setDisabled(true);
			UpdateSpinBox();
			return;
		}
		sb++;
		for (int j = 0; j < sys->models[i]->meshes.size(); j++) {
			if (sb == index) {
				SELECTED_MODEL = i;
				SELECTED_MESH = j;
				ui.matButton->setDisabled(false);
				UpdateSpinBox();
				return;
			}
			sb++;
		}
	}
	SELECTED_MODEL = -1;
	SELECTED_MESH = -1;
	UpdateSpinBox();
}

void ModelTab::ChangeRenderMode()
{
	if (sys->renderMode == Line)
		sys->renderMode = Fill;
	else
		sys->renderMode = Line;
}

void ModelTab::ChangeModelMatrix(double x)
{
	if (changed)
		return;
	sys->models[SELECTED_MODEL]->position.x = ui.posXSpin->value();
	sys->models[SELECTED_MODEL]->position.y = ui.posYSpin->value();
	sys->models[SELECTED_MODEL]->position.z = ui.posZSpin->value();
	sys->models[SELECTED_MODEL]->pitch = ui.rotXSpin->value();
	sys->models[SELECTED_MODEL]->yaw = ui.rotYSpin->value();
	sys->models[SELECTED_MODEL]->roll = ui.rotZSpin->value();
	sys->models[SELECTED_MODEL]->scale = ui.scaleSpin->value();
}

void ModelTab::SetMaterial()
{
	MaterialDialog * dialog = new MaterialDialog(sys->models[SELECTED_MODEL]->meshes[SELECTED_MESH]->material);
	dialog->show();
}

void ModelTab::UpdateText()
{
	if (itemMdl)
		delete itemMdl;
	itemMdl = new QStandardItemModel(this);
	for (int i = 0; i < sys->models.size(); i++)
	{
		QString mdl = static_cast<QString>(sys->models[i]->name.c_str());
		QStandardItem *mdlItem = new QStandardItem(mdl);
		itemMdl->appendRow(mdlItem);
		for (int j = 0; j < sys->models[i]->meshes.size(); j++) {
			QString obj = static_cast<QString>(("    " + sys->models[i]->meshes[j]->name).c_str());
			QStandardItem * objItem = new QStandardItem(obj);
			itemMdl->appendRow(objItem);
		}
	}
	ui.mdlListView->setModel(itemMdl);
}

void ModelTab::UpdateSpinBox()
{
	if (SELECTED_MODEL == -1) {
		ui.posXSpin->setDisabled(true);
		ui.posYSpin->setDisabled(true);
		ui.posZSpin->setDisabled(true);
		ui.rotXSpin->setDisabled(true);
		ui.rotYSpin->setDisabled(true);
		ui.rotZSpin->setDisabled(true);
		ui.scaleSpin->setDisabled(true);
		return;
	}
	changed = true;
	ui.posXSpin->setDisabled(false);
	ui.posXSpin->setValue(sys->models[SELECTED_MODEL]->position.x);
	ui.posYSpin->setDisabled(false);
	ui.posYSpin->setValue(sys->models[SELECTED_MODEL]->position.y);
	ui.posZSpin->setDisabled(false);
	ui.posZSpin->setValue(sys->models[SELECTED_MODEL]->position.z);
	ui.rotXSpin->setDisabled(false);
	ui.rotXSpin->setValue(sys->models[SELECTED_MODEL]->pitch);
	ui.rotYSpin->setDisabled(false);
	ui.rotYSpin->setValue(sys->models[SELECTED_MODEL]->yaw);
	ui.rotZSpin->setDisabled(false);
	ui.rotZSpin->setValue(sys->models[SELECTED_MODEL]->roll);
	ui.scaleSpin->setDisabled(false);
	ui.scaleSpin->setValue(sys->models[SELECTED_MODEL]->scale);
	changed = false;
}
