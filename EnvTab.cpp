#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QCheckBox>
#include <QString>
#include <QListView>
#include <QMutex>
#include "EnvTab.h"
#include "Light.h"
#include "Model.h"
#include "Mesh.h"
#include "Texture.h"
#include "MaterialBase.h"
#include "StateMachine.h"
#include "Function.h"

EnvTab::EnvTab(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	sys = StateMachine::GetInstance();
	glm::vec3 euler = Dir2Euler(sys->dirLights[0]->direction);
	ui.dirX->setValue(euler.x);
	ui.dirY->setValue(euler.y);
	ui.dirZ->setValue(euler.z);
	ui.lcR->setValue(sys->dirLights[0]->color.r * 255);
	ui.lcG->setValue(sys->dirLights[0]->color.g * 255);
	ui.lcB->setValue(sys->dirLights[0]->color.b * 255);
	ui.acR->setValue(sys->ambient.r * 255);
	ui.acG->setValue(sys->ambient.g * 255);
	ui.acB->setValue(sys->ambient.b * 255);

	connect(ui.dirX, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &EnvTab::SetLight);
	connect(ui.dirY, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &EnvTab::SetLight);
	connect(ui.dirZ, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &EnvTab::SetLight);
	connect(ui.lcR, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &EnvTab::SetLight);
	connect(ui.lcG, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &EnvTab::SetLight);
	connect(ui.lcB, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &EnvTab::SetLight);
	connect(ui.acR, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &EnvTab::SetLight);
	connect(ui.acG, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &EnvTab::SetLight);
	connect(ui.acB, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &EnvTab::SetLight);
	connect(ui.skyboxCB, &QCheckBox::clicked, this, &EnvTab::SetSkyBox);
	connect(ui.shadowCB, &QCheckBox::clicked, this, &EnvTab::SetShadow);
	connect(ui.csB, &QPushButton::clicked, this, &EnvTab::ChangeRenderScene);

	connect(ui.skyboxList, &QListView::clicked, this, &EnvTab::SelectionChange);
	ui.skyboxList->setEditTriggers(QAbstractItemView::NoEditTriggers);

	itemMdl = new QStandardItemModel(this);
	for (int i = 0; i < 3; i++)
	{
		if (sys->skyboxMap[i]) {
			QString mdl = QString("Skybox") + QString::number(i);
			QStandardItem *mdlItem = new QStandardItem(mdl);
			itemMdl->appendRow(mdlItem);
		}
	}
	ui.skyboxList->setModel(itemMdl);


}

EnvTab::~EnvTab()
{
	sys = nullptr;
}

void EnvTab::SetSkyBox()
{
	sys->drawSkyBox = !sys->drawSkyBox;
}


void EnvTab::SetShadow()
{
	sys->drawShadow = !sys->drawShadow;
}

void EnvTab::ChangeRenderScene()
{
	sys->ChangeRenderScene();
}

void EnvTab::SelectionChange(const QModelIndex & current)
{
	sys->currentSkybox = current.row();
}

void EnvTab::SetLight()
{
	glm::vec3 dir = glm::vec3(ui.dirX->value(), ui.dirY->value(), ui.dirZ->value());
	if (dir.x == 0.0f && dir.y == 0.0f && dir.z == 0.0f)
		return;
	dir = Euler2Dir(dir);
	sys->dirLights[0]->SetDirection(dir);
	sys->dirLights[0]->color.r = ui.lcR->value() / 255.0f;
	sys->dirLights[0]->color.g = ui.lcG->value() / 255.0f;
	sys->dirLights[0]->color.b = ui.lcB->value() / 255.0f;
	sys->ambient.r = ui.acR->value() / 255.0f;
	sys->ambient.g = ui.acG->value() / 255.0f;
	sys->ambient.b = ui.acB->value() / 255.0f;
}
