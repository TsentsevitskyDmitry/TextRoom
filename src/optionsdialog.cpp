/****************************************************************************
** Copyright (C) 2008 Petar Toushkov <peter dot toushkov at gmail.com>
** Copyright (C) 2008 Omer Bahri Gordebak <gordebak at gmail.com>
**
** Additional help, code and insights by:
** adamvert - from http://ubuntuforums.org/
** zebulon M - from http://ubuntuforums.org/
**
** Parts of the following code are from the Phrasis project:
**
** http://code.google.com/p/phrasis/
**
** and are
**
** Copyright (C) 2006 Jacob R Rideout <jacob@jacobrideout.net>
** All rights reserved.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <QtGui>

#include "optionsdialog.h"
#include "textroom.h"

OptionsDialog::OptionsDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	reaSettings();

	connect(ui.statusbarSpinBox, SIGNAL( valueChanged(int) ), this, SLOT( activateApply() ) );
	connect(ui.wordCountSpinBox, SIGNAL( valueChanged(int) ), this, SLOT( activateApply() ) );
	connect(ui.editorSpinBox, SIGNAL( valueChanged(int) ), this, SLOT( activateApply() ) );
	connect(ui.editorWidthSpinBox, SIGNAL( valueChanged(int) ), this, SLOT(activateApply()) );
	connect(ui.soundCheckBox, SIGNAL( clicked() ), this, SLOT( activateApply() ) );
	connect(ui.editorBoldCheckBox, SIGNAL( clicked() ), this, SLOT( activateApply() ) );
	connect(ui.editorItalicCheckBox, SIGNAL( clicked() ), this, SLOT( activateApply() ) );
	connect(ui.statusbarBoldCheckBox, SIGNAL( clicked() ), this, SLOT( activateApply() ) );
	connect(ui.statusbarItalicCheckBox, SIGNAL( clicked() ), this, SLOT( activateApply() ) );
	connect(ui.loadOnStartCheckBox, SIGNAL( clicked() ), this, SLOT( activateApply() ) );
	connect(ui.saveCursorCheckBox, SIGNAL( clicked() ), this, SLOT( activateApply() ) );
	connect(ui.fullScreenCheckBox, SIGNAL( clicked() ), this, SLOT( activateApply() ) );
	connect(ui.splashScreenCheckBox, SIGNAL( clicked() ), this, SLOT( activateApply() ) );
	connect(ui.autoSaveCheckBox, SIGNAL( clicked() ), this, SLOT( activateApply() ) );
	connect(ui.flowModeCheckBox, SIGNAL( clicked() ), this, SLOT( activateApply() ) );
	connect(ui.editorFontComboBox, SIGNAL( activated(int) ), this, SLOT( activateApply() ) );
	connect(ui.statusbarFontComboBox, SIGNAL( activated(int) ), this, SLOT( activateApply() ) );
	connect(ui.calendarWidget, SIGNAL( selectionChanged() ), this, SLOT( activateApply() ) );
	connect(ui.pushButton, SIGNAL( clicked() ), this, SLOT( startAlarm() ) );
}

void OptionsDialog::activateApply()
{
	ui.pushButtonApply->setEnabled(true);
}

void OptionsDialog::startAlarm()
{
	alarm = ui.spinBox->value();
	writSettings();
	close();
}

void OptionsDialog::reaSettings()
{
#ifdef Q_OS_WIN32
	QSettings settings(QDir::homePath()+"/Application Data/"+qApp->applicationName()+".ini", QSettings::IniFormat);
#else

	QSettings settings;
#endif

	QDateTime today = QDateTime::currentDateTime();
	QString todaytext = today.toString("yyyyMMdd");
	QStringList fontS;
	QFont font;

	fontS << settings.value("Font/Font_Settings",  ui.editorFontComboBox->currentFont() ).toString() 
		<< settings.value("Font/FileName_Settings", ui.statusbarFontComboBox->currentFont() ).toString()
		<< settings.value("Font/Statistics_Settings", ui.statusbarFontComboBox->currentFont() ).toString();
	
	font.fromString(fontS.at(0));
	ui.editorFontComboBox->setCurrentFont( font );
	ui.editorBoldCheckBox->setChecked( font.bold() );
	ui.editorItalicCheckBox->setChecked( font.italic() );
	ui.editorSpinBox->setValue( font.pointSize() );
	
	font.fromString(fontS.at(1));
	ui.statusbarFontComboBox->setCurrentFont( font );
	ui.statusbarBoldCheckBox->setChecked( font.bold() );
	ui.statusbarItalicCheckBox->setChecked( font.italic() );
	ui.statusbarSpinBox->setValue( font.pointSize() );
	
	ui.loadOnStartCheckBox->setChecked( settings.value( "RecentFiles/OpenLastFile", true ).toBool() );
	ui.saveCursorCheckBox->setChecked( settings.value( "RecentFiles/SavePosition", true ).toBool() );
	if ( !ui.loadOnStartCheckBox->isChecked() )
		ui.saveCursorCheckBox->setEnabled( false );

	ui.wordCountSpinBox->setValue( settings.value("WordCount", 0).toInt());
	ui.fullScreenCheckBox->setChecked( settings.value("WindowState/ShowFullScreen", true).toBool() );
	ui.splashScreenCheckBox->setChecked( settings.value("WindowState/ShowSplashScreen", true).toBool() );
	ui.soundCheckBox->setChecked( settings.value("Sound", true).toBool() );
	ui.autoSaveCheckBox->setChecked( settings.value("AutoSave", false).toBool() );
	ui.flowModeCheckBox->setChecked( settings.value("FlowMode", false).toBool() );
	QString datetext = settings.value("Deadline", todaytext).toString();
	QDate date;
	QDate dateselected = date.fromString(datetext, "yyyyMMdd");
	ui.calendarWidget->setSelectedDate(dateselected);
	ui.editorWidthSpinBox->setValue( settings.value	("EditorWidth", 800).toInt());    
	int alarm = settings.value("TimedWriting", 0 ).toInt();

	QPalette palette;

	palette.setColor(ui.pbEditorColor->backgroundRole(),
		fgcolor = settings.value("Colors/Foreground", "#d0a100" ).toString());
	ui.pbEditorColor->setPalette(palette);
	
	palette.setColor(ui.pbStatusBarColor->backgroundRole(),
		scolor = settings.value("Colors/StatusColor", "#404040" ).toString());
	ui.pbStatusBarColor->setPalette(palette);

	
	palette.setColor(ui.pbEditorBackColor->backgroundRole(),
		bgcolor = settings.value("Colors/Background", "black" ).toString());
	ui.pbEditorBackColor->setPalette(palette);
	
	palette.setColor(ui.pbStatusBarBgColor->backgroundRole(),
		sbcolor = settings.value("Colors/StatusBg", "#e8e8e8").toString());
	ui.pbStatusBarBgColor->setPalette(palette);


}

void OptionsDialog::writSettings()
{

#ifdef Q_OS_WIN32
	QSettings settings(QDir::homePath()+"/Application Data/"+qApp->applicationName()+".ini", QSettings::IniFormat);
#else

	QSettings settings;
#endif

	
	settings.setValue("Colors/Foreground", fgcolor.name() );
	settings.setValue("Colors/Background", bgcolor.name() );
	settings.setValue("Colors/StatusColor", scolor.name() );
	settings.setValue("Colors/StatusBg", sbcolor.name() );

	settings.setValue("RecentFiles/OpenLastFile", ui.loadOnStartCheckBox->isChecked() );
	settings.setValue("RecentFiles/SavePosition", ui.saveCursorCheckBox->isChecked() );

	settings.setValue("WindowState/ShowFullScreen", ui.fullScreenCheckBox->isChecked() );
	settings.setValue("WindowState/ShowSplashScreen", ui.splashScreenCheckBox->isChecked() );
	settings.setValue("Sound", ui.soundCheckBox->isChecked() );
	settings.setValue("AutoSave", ui.autoSaveCheckBox->isChecked() );
	settings.setValue("FlowMode", ui.flowModeCheckBox->isChecked() );
	settings.setValue("WordCount", ui.wordCountSpinBox->value() );
	settings.setValue("Deadline", ui.calendarWidget->selectedDate().toString("yyyyMMdd"));
	settings.setValue("EditorWidth", ui.editorWidthSpinBox->value() );
	settings.setValue("TimedWriting", alarm );

	QFont font;
	
	font = ui.editorFontComboBox->currentFont();
	font.setBold(ui.editorBoldCheckBox->isChecked() );
	font.setItalic(ui.editorItalicCheckBox->isChecked() );
	font.setPointSize(ui.editorSpinBox->value() );
	settings.setValue("Font/Font_Settings", font.toString() );
	
	font = ui.statusbarFontComboBox->currentFont();
	font.setBold(ui.statusbarBoldCheckBox->isChecked() );
	font.setItalic(ui.statusbarItalicCheckBox->isChecked() );
	font.setPointSize(ui.statusbarSpinBox->value() );
	settings.setValue("Font/FileName_Settings", font.toString() );
	
	font = ui.statusbarFontComboBox->currentFont();
	font.setBold(ui.statusbarBoldCheckBox->isChecked() );
	font.setItalic(ui.statusbarItalicCheckBox->isChecked() );
	font.setPointSize(ui.statusbarSpinBox->value() );
	settings.setValue("Font/Statistics_Settings", font.toString() );

   	
}

void OptionsDialog::on_pushButtonOK_clicked()
{
	writSettings();
}

void OptionsDialog::on_pushButtonApply_clicked()
{
	writSettings();
	ui.pushButtonApply->setEnabled(false);
}

void OptionsDialog::showEvent( QShowEvent * )
{
	reaSettings();
}

// FOREGROUND

void OptionsDialog::on_pbEditorColor_clicked()
{
	showForegroundDialog();
	ui.pushButtonApply->setEnabled(true);
}

void OptionsDialog::showForegroundDialog()
{
	QColor c = QColorDialog::getColor(fgcolor, this);
	if (c.isValid())
	{
		QPalette palette;
		palette.setColor(ui.pbEditorColor->backgroundRole(), fgcolor = c);
		ui.pbEditorColor->setPalette(palette);
		ui.pbEditorColor->setAutoFillBackground(false);
	}
}

//STATUS BACKGROUND
void OptionsDialog::on_pbStatusBarBgColor_clicked()
{
	showStatusBarBgColorDialog();
	ui.pushButtonApply->setEnabled(true);
}

void OptionsDialog::showStatusBarBgColorDialog()
{
	QColor c = QColorDialog::getColor(sbcolor, this);
	if (c.isValid())
	{
		QPalette palette;
		palette.setColor(ui.pbStatusBarBgColor->backgroundRole(), sbcolor = c);
		ui.pbStatusBarBgColor->setPalette(palette);
		ui.pbStatusBarBgColor->setAutoFillBackground(true);
	}
}


// BACKGROUND

void OptionsDialog::on_pbEditorBackColor_clicked()
{
	showBackgroundDialog();
	ui.pushButtonApply->setEnabled(true);
}

void OptionsDialog::showBackgroundDialog()
{
	QColor c = QColorDialog::getColor(bgcolor, this);
	if (c.isValid())
	{
		QPalette palette;
		palette.setColor(ui.pbEditorBackColor->backgroundRole(), bgcolor = c);
		ui.pbEditorBackColor->setPalette(palette);
		ui.pbEditorBackColor->setAutoFillBackground(true);
	}
}

// STATUSBARTEXTCOLOR

void OptionsDialog::on_pbStatusBarColor_clicked()
{
	showStatusColorDialog();
	ui.pushButtonApply->setEnabled(true);
}

void OptionsDialog::showStatusColorDialog()
{
	QColor c = QColorDialog::getColor(scolor, this);
	if (c.isValid())
	{
		QPalette palette;
		palette.setColor(ui.pbStatusBarColor->backgroundRole(), scolor = c);
		ui.pbStatusBarColor->setPalette(palette);
		ui.pbStatusBarColor->setAutoFillBackground(false);
	}
}
