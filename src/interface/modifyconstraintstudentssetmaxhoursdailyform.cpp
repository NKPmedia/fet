/***************************************************************************
                          modifyconstraintstudentssetmaxhoursdailyform.cpp  -  description
                             -------------------
    begin                : July 19, 2007
    copyright            : (C) 2007 by Lalescu Liviu
    email                : Please see https://lalescu.ro/liviu/ for details about contacting Liviu Lalescu (in particular, you can find here the e-mail address)
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU Affero General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************/

#include <QMessageBox>
#include "centerwidgetonscreen.h"
#include "invisiblesubgrouphelper.h"

#include "modifyconstraintstudentssetmaxhoursdailyform.h"
#include "timeconstraint.h"

#include "fetguisettings.h"

ModifyConstraintStudentsSetMaxHoursDailyForm::ModifyConstraintStudentsSetMaxHoursDailyForm(QWidget* parent, ConstraintStudentsSetMaxHoursDaily* ctr): QDialog(parent)
{
	setupUi(this);

	okPushButton->setDefault(true);

	connect(okPushButton, SIGNAL(clicked()), this, SLOT(ok()));
	connect(cancelPushButton, SIGNAL(clicked()), this, SLOT(close()));

	centerWidgetOnScreen(this);
	restoreFETDialogGeometry(this);

	QSize tmp2=studentsComboBox->minimumSizeHint();
	Q_UNUSED(tmp2);
	
	this->_ctr=ctr;

	weightLineEdit->setText(CustomFETString::number(ctr->weightPercentage));
	
	updateStudentsComboBox();

	maxHoursSpinBox->setMinimum(1);
	maxHoursSpinBox->setMaximum(gt.rules.nHoursPerDay);
	maxHoursSpinBox->setValue(ctr->maxHoursDaily);
}

ModifyConstraintStudentsSetMaxHoursDailyForm::~ModifyConstraintStudentsSetMaxHoursDailyForm()
{
	saveFETDialogGeometry(this);
}

void ModifyConstraintStudentsSetMaxHoursDailyForm::updateStudentsComboBox(){
	int j=populateStudentsComboBox(studentsComboBox, this->_ctr->students, true);
	if(j<0)
		InvisibleSubgroupHelper::showWarningForConstraintCase(this, this->_ctr->students);
	studentsComboBox->setCurrentIndex(j);
}

void ModifyConstraintStudentsSetMaxHoursDailyForm::ok()
{
	if(studentsComboBox->currentIndex()<0){
		InvisibleSubgroupHelper::showWarningCannotModifyConstraintCase(this, this->_ctr->students);
		return;
	}

	double weight;
	QString tmp=weightLineEdit->text();
	weight_sscanf(tmp, "%lf", &weight);
	if(weight<0.0 || weight>100.0){
		QMessageBox::warning(this, tr("FET information"),
			tr("Invalid weight (percentage)"));
		return;
	}

	QString students_name=studentsComboBox->currentText();
	StudentsSet* s=gt.rules.searchStudentsSet(students_name);
	if(s==NULL){
		QMessageBox::warning(this, tr("FET information"),
			tr("Invalid students set"));
		return;
	}

	this->_ctr->weightPercentage=weight;
	this->_ctr->students=students_name;
	this->_ctr->maxHoursDaily=maxHoursSpinBox->value();

	gt.rules.internalStructureComputed=false;
	gt.rules.setModified(true);
	
	this->close();
}
