#include "Menu_FiringMode.h"

Menu_FiringMode::Menu_FiringMode(Ui::MainMenu* ui)
{
	this->ui = ui;

	mutatorSettings_ = new MutatorSettings();
}

Menu_FiringMode::~Menu_FiringMode()
{
	delete mutatorSettings_;
}

void Menu_FiringMode::setSettingsMenu()	// TODO: Set good values for the sliders and make a good conversion
{
	FiringMode* firingMode = getFiringModeSettings();

	ui->checkBox_Weapon_Bullet->setChecked(firingMode->canShootBullet);
	ui->checkBox_Weapon_Scatter->setChecked(firingMode->canShootScatter);
	ui->checkBox_Weapon_Explosive->setChecked(firingMode->canShootExplosive);
	ui->horizontalSlider_Weapon_ClipSize->setValue(firingMode->clipSize);
	ui->horizontalSlider_Weapon_DamageModifier->setValue(static_cast<int>(firingMode->damageModifier * 100.0f));
	ui->horizontalSlider_Weapon_ExplosionSphereModifier->setValue(static_cast<int>(firingMode->explosionSphereModifier * 100.0f));
	ui->horizontalSlider_Weapon_RateOfFire->setValue(static_cast<int>(1.0f / firingMode->cooldownBetweenShots));
	ui->horizontalSlider_Weapon_ReloadTime->setValue(static_cast<int>(firingMode->reloadTime * 100.0f));

	ui->doubleSpinBox_FiringMode_ReloadTime->setValue(firingMode->reloadTime);
	ui->doubleSpinBox_FiringMode_DamageModifier->setValue(firingMode->damageModifier);
	ui->doubleSpinBox_FiringMode_ExplosionSphereModifier->setValue(firingMode->explosionSphereModifier);
}

void Menu_FiringMode::settingsMenuUpdated()
{
	FiringMode* firingMode = getFiringModeSettings();
	
	firingMode->canShootBullet = ui->checkBox_Weapon_Bullet->isChecked();
	firingMode->canShootScatter = ui->checkBox_Weapon_Scatter->isChecked();
	firingMode->canShootExplosive = ui->checkBox_Weapon_Explosive->isChecked();
	firingMode->clipSize = ui->horizontalSlider_Weapon_ClipSize->value();
	firingMode->cooldownBetweenShots = 1.0f / static_cast<float>(ui->horizontalSlider_Weapon_RateOfFire->value());
	firingMode->damageModifier = static_cast<float>(ui->horizontalSlider_Weapon_DamageModifier->value()) * 0.01f;
	firingMode->explosionSphereModifier = static_cast<float>(ui->horizontalSlider_Weapon_ExplosionSphereModifier->value()) * 0.01f;
	firingMode->reloadTime = static_cast<float>(ui->horizontalSlider_Weapon_ReloadTime->value()) * 0.01f;

	ui->doubleSpinBox_FiringMode_ReloadTime->setValue(firingMode->reloadTime);
	ui->doubleSpinBox_FiringMode_DamageModifier->setValue(firingMode->damageModifier);
	ui->doubleSpinBox_FiringMode_ExplosionSphereModifier->setValue(firingMode->explosionSphereModifier);
}

FiringMode* Menu_FiringMode::getFiringModeSettings()
{
	FiringMode::FiringModeType firingMode = FiringMode::SINGLE;

	if(ui->radioButton_Weapon_Single->isChecked())
	{
		firingMode = FiringMode::SINGLE;
	}
	else if(ui->radioButton_Weapon_Semi->isChecked())
	{
		firingMode = FiringMode::SEMI;
	}
	else if(ui->radioButton_Weapon_Auto->isChecked())
	{
		firingMode = FiringMode::AUTO;
	}

	return &mutatorSettings_->getStandardFiringMode(firingMode);
}