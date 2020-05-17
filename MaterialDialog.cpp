#include <QFileDialog>
#include <QImage>
#include <QPixmap>
#include <QSpinBox>
#include "StateMachine.h"
#include "MaterialDialog.h"
#include "MaterialBase.h"
#include "Texture.h"
#include "Image.h"


MaterialDialog::MaterialDialog(Material * m,QWidget *parent)
	: material(m),QDialog(parent)
{
	ui.setupUi(this);
	init = false;
	sys = StateMachine::GetInstance();
	connect(ui.moB, &QPushButton::clicked, this, &MaterialDialog::OpenMainTexture);
	connect(ui.noB, &QPushButton::clicked, this, &MaterialDialog::OpenNormalTexture);
	connect(ui.meoB, &QPushButton::clicked, this, &MaterialDialog::OpenMetallicTexture);
	connect(ui.roB, &QPushButton::clicked, this, &MaterialDialog::OpenRoughnessTexture);
	connect(ui.aoB, &QPushButton::clicked, this, &MaterialDialog::OpenAOTexture);
	connect(ui.mrB, &QPushButton::clicked, this, &MaterialDialog::RemoveMainTexture);
	connect(ui.nrB, &QPushButton::clicked, this, &MaterialDialog::RemoveNormalTexture);
	connect(ui.merB, &QPushButton::clicked, this, &MaterialDialog::RemoveMetallicTexture);
	connect(ui.rrB, &QPushButton::clicked, this, &MaterialDialog::RemoveRoughnessTexture);
	connect(ui.arB, &QPushButton::clicked, this, &MaterialDialog::RemoveAOTexture);
	connect(ui.cSBR, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &MaterialDialog::SetMaterial);
	connect(ui.cSBG, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &MaterialDialog::SetMaterial);
	connect(ui.cSBB, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &MaterialDialog::SetMaterial);
	connect(ui.sSBR, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &MaterialDialog::SetMaterial);
	connect(ui.sSBG, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &MaterialDialog::SetMaterial);
	connect(ui.sSBB, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &MaterialDialog::SetMaterial);
	connect(ui.rFR, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &MaterialDialog::SetMaterial);
	connect(ui.rFG, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &MaterialDialog::SetMaterial);
	connect(ui.rFB, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &MaterialDialog::SetMaterial);
	connect(ui.gSB, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &MaterialDialog::SetMaterial);
	connect(ui.bSB, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &MaterialDialog::SetMaterial);
	connect(ui.mSB, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &MaterialDialog::SetMaterial);
	connect(ui.sSB, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &MaterialDialog::SetMaterial);

	ShowTexture(material->mainTex.get(), ui.mCav);
	ShowTexture(material->normalTex.get(), ui.nCav);
	ShowTexture(material->metallicTex.get(), ui.meCav);
	ShowTexture(material->roughnessTex.get(), ui.rCav);
	ShowTexture(material->aoTex.get(), ui.aCav);

	ui.cSBR->setValue(material->color.r * 255);
	ui.cSBG->setValue(material->color.g * 255);
	ui.cSBB->setValue(material->color.b * 255);

	ui.sSBR->setValue(material->specular.r * 255);
	ui.sSBG->setValue(material->specular.g * 255);
	ui.sSBB->setValue(material->specular.b * 255);

	ui.rFR->setValue(material->rF0.r * 255);
	ui.rFG->setValue(material->rF0.g * 255);
	ui.rFB->setValue(material->rF0.b * 255);

	ui.gSB->setValue(material->gloss);
	ui.bSB->setValue(material->bump);
	ui.mSB->setValue(material->metallic);
	ui.sSB->setValue(1.0f - material->roughness);

	init = true;
}

MaterialDialog::~MaterialDialog()
{
}

void MaterialDialog::ShowTexture(Texture2D * t, QLabel * cav)
{
	if (t == nullptr)
		return;
	Image * img = t->GetImage();
	QImage * tmp;
	if (img->channel == 3)
		tmp = new QImage(img->data, img->width, img->height, QImage::Format_RGB888);
	else if (img->channel == 4)
		tmp = new QImage(img->data, img->width, img->height, QImage::Format_RGBA8888);
	else if (img->channel == 1)
		tmp = new QImage(img->data, img->width, img->height, QImage::Format_Grayscale8);
	QImage image = tmp->scaled(100, 100);
	delete tmp;
	cav->setPixmap(QPixmap::fromImage(image));
}

void MaterialDialog::OpenMainTexture(){
	sys->mutex.lock();
	QFileDialog *fileDialog = new QFileDialog(this);
	fileDialog->setWindowTitle(QStringLiteral("打开纹理"));
	fileDialog->setDirectory(".");
	fileDialog->setNameFilter(tr("Texture File(*.bmp *.jpg *.jpeg *.png *.tga *.hdr)"));
	fileDialog->setFileMode(QFileDialog::ExistingFiles);
	fileDialog->setViewMode(QFileDialog::Detail);
	QStringList fileNames;
	if (fileDialog->exec()) {
		fileNames = fileDialog->selectedFiles();
		Texture2D * tex = new Texture2D(fileNames[0].toStdString());
		material->SetTexture(tex, 0);
		ShowTexture(tex, ui.mCav);
		delete tex;
	}
	sys->mutex.unlock();
}

void MaterialDialog::OpenNormalTexture()
{
	sys->mutex.lock();
	QFileDialog *fileDialog = new QFileDialog(this);
	fileDialog->setWindowTitle(QStringLiteral("打开纹理"));
	fileDialog->setDirectory(".");
	fileDialog->setNameFilter(tr("Texture File(*.bmp *.jpg *.jpeg *.png *.tga *.hdr)"));
	fileDialog->setFileMode(QFileDialog::ExistingFiles);
	fileDialog->setViewMode(QFileDialog::Detail);
	QStringList fileNames;
	if (fileDialog->exec()) {
		fileNames = fileDialog->selectedFiles();
		Texture2D * tex = new Texture2D(fileNames[0].toStdString());
		material->SetTexture(tex, 1);
		ShowTexture(tex, ui.nCav);
		delete tex;
	}
	sys->mutex.unlock();
}

void MaterialDialog::OpenMetallicTexture()
{
	sys->mutex.lock();
	QFileDialog *fileDialog = new QFileDialog(this);
	fileDialog->setWindowTitle(QStringLiteral("打开纹理"));
	fileDialog->setDirectory(".");
	fileDialog->setNameFilter(tr("Texture File(*.bmp *.jpg *.jpeg *.png *.tga *.hdr)"));
	fileDialog->setFileMode(QFileDialog::ExistingFiles);
	fileDialog->setViewMode(QFileDialog::Detail);
	QStringList fileNames;
	if (fileDialog->exec()) {
		fileNames = fileDialog->selectedFiles();
		Texture2D * tex = new Texture2D(fileNames[0].toStdString());
		material->SetTexture(tex, 2);
		ShowTexture(tex, ui.meCav);
		delete tex;
	}
	sys->mutex.unlock();
}

void MaterialDialog::OpenRoughnessTexture()
{
	sys->mutex.lock();
	QFileDialog *fileDialog = new QFileDialog(this);
	fileDialog->setWindowTitle(QStringLiteral("打开纹理"));
	fileDialog->setDirectory(".");
	fileDialog->setNameFilter(tr("Texture File(*.bmp *.jpg *.jpeg *.png *.tga *.hdr)"));
	fileDialog->setFileMode(QFileDialog::ExistingFiles);
	fileDialog->setViewMode(QFileDialog::Detail);
	QStringList fileNames;
	if (fileDialog->exec()) {
		fileNames = fileDialog->selectedFiles();
		Texture2D * tex = new Texture2D(fileNames[0].toStdString());
		material->SetTexture(tex, 3);
		ShowTexture(tex, ui.rCav);
		delete tex;
	}
	sys->mutex.unlock();
}

void MaterialDialog::OpenAOTexture()
{
	sys->mutex.lock();
	QFileDialog *fileDialog = new QFileDialog(this);
	fileDialog->setWindowTitle(QStringLiteral("打开纹理"));
	fileDialog->setDirectory(".");
	fileDialog->setNameFilter(tr("Texture File(*.bmp *.jpg *.jpeg *.png *.tga *.hdr)"));
	fileDialog->setFileMode(QFileDialog::ExistingFiles);
	fileDialog->setViewMode(QFileDialog::Detail);
	QStringList fileNames;
	if (fileDialog->exec()) {
		fileNames = fileDialog->selectedFiles();
		Texture2D * tex = new Texture2D(fileNames[0].toStdString());
		material->SetTexture(tex, 4);
		ShowTexture(tex, ui.aCav);
		delete tex;
	}
	sys->mutex.unlock();
}

void MaterialDialog::RemoveMainTexture()
{
	sys->mutex.lock();
	material->mainTex = nullptr;
	ui.mCav->clear();
	sys->mutex.unlock();
}

void MaterialDialog::RemoveNormalTexture()
{
	sys->mutex.lock();
	material->normalTex = nullptr;
	ui.nCav->clear();
	sys->mutex.unlock();
}

void MaterialDialog::RemoveMetallicTexture()
{
	sys->mutex.lock();
	material->metallicTex = nullptr;
	ui.meCav->clear();
	sys->mutex.unlock();
}

void MaterialDialog::RemoveRoughnessTexture()
{
	sys->mutex.lock();
	material->roughnessTex = nullptr;
	ui.rCav->clear();
	sys->mutex.unlock();
}

void MaterialDialog::RemoveAOTexture()
{
	sys->mutex.lock();
	material->aoTex = nullptr;
	ui.aCav->clear();
	sys->mutex.unlock();
}

void MaterialDialog::SetMaterial()
{
	if (!init)
		return;
	material->color.r = ui.cSBR->value() / 255.0f;
	material->color.g = ui.cSBG->value() / 255.0f ;
	material->color.b = ui.cSBB->value() / 255.0f ;
	material->specular.r = ui.sSBR->value() / 255.0f ;
	material->specular.g = ui.sSBG->value() / 255.0f ;
	material->specular.b = ui.sSBB->value() / 255.0f ;
	material->rF0.r = ui.rFR->value() / 255.0f ;
	material->rF0.g = ui.rFG->value() / 255.0f ;
	material->rF0.b = ui.rFB->value() / 255.0f ;
	material->gloss = ui.gSB->value();
	material->bump = ui.bSB->value();
	material->metallic = ui.mSB->value();
	material->roughness = 1.0f - ui.sSB->value();
}
