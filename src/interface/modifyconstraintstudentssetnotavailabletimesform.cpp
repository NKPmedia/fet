/***************************************************************************
                          modifyconstraintstudentssetnotavailabletimesform.cpp  -  description
                             -------------------
    begin                : Feb 10, 2005
    copyright            : (C) 2005 by Lalescu Liviu
    email                : Please see http://lalescu.ro/liviu/ for details about contacting Liviu Lalescu (in particular, you can find here the e-mail address)
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

#include "modifyconstraintstudentssetnotavailabletimesform.h"
#include "timeconstraint.h"

ModifyConstraintStudentsSetNotAvailableTimesForm::ModifyConstraintStudentsSetNotAvailableTimesForm(QWidget* parent, ConstraintStudentsSetNotAvailableTimes* ctr): QDialog(parent)
{
	setupUi(this);

	okPushButton->setDefault(true);

	connect(okPushButton, SIGNAL(clicked()), this, SLOT(ok()));
	connect(cancelPushButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(setAllAvailablePushButton, SIGNAL(clicked()), this, SLOT(setAllAvailable()));
	connect(setAllNotAvailablePushButton, SIGNAL(clicked()), this, SLOT(setAllNotAvailable()));

	centerWidgetOnScreen(this);
	restoreFETDialogGeometry(this);

	QSize tmp2=studentsComboBox->minimumSizeHint();
	Q_UNUSED(tmp2);
		
	this->_ctr=ctr;
	
	weightLineEdit->setText(CustomFETString::number(ctr->weightPercentage));
	
	updateStudentsComboBox();

	notAllowedTimesTable->setHeaders(gt.rules);

	assert(ctr->days.count()==ctr->hours.count());	
	for(int k=0; k<ctr->days.count(); k++){
		if(ctr->hours.at(k)==-1 || ctr->days.at(k)==-1)
			assert(0);
		int i=ctr->hours.at(k);
		int j=ctr->days.at(k);
		if(i>=0 && i<gt.rules.nHoursPerDay && j>=0 && j<gt.rules.nDaysPerWeek)
			notAllowedTimesTable->setMarked(i,j, true);
	}
}

ModifyConstraintStudentsSetNotAvailableTimesForm::~ModifyConstraintStudentsSetNotAvailableTimesForm()
{
	saveFETDialogGeometry(this);
}

void ModifyConstraintStudentsSetNotAvailableTimesForm::setAllAvailable()
{
	notAllowedTimesTable->setAllUnmarked();
}

void ModifyConstraintStudentsSetNotAvailableTimesForm::setAllNotAvailable()
{
	notAllowedTimesTable->setAllMarked();
}

void ModifyConstraintStudentsSetNotAvailableTimesForm::updateStudentsComboBox(){
	studentsComboBox->clear();
	for(int i=0; i<gt.rules.yearsList.size(); i++){
		StudentsYear* sty=gt.rules.yearsList[i];
		studentsComboBox->addItem(sty->name);
		for(int j=0; j<sty->groupsList.size(); j++){
			StudentsGroup* stg=sty->groupsList[j];
			studentsComboBox->addItem(stg->name);
			if(SHOW_SUBGROUPS_IN_COMBO_BOXES) for(int k=0; k<stg->subgroupsList.size(); k++){
				StudentsSubgroup* sts=stg->subgroupsList[k];
				studentsComboBox->addItem(sts->name);
			}
		}
	}
	if (studentsComboBox->findText(this->_ctr->students) < 0)
		showWarningForInvisibleSubgroupConstraint(this, this->_ctr->students);
	studentsComboBox->setCurrentText(this->_ctr->students);
}

void ModifyConstraintStudentsSetNotAvailableTimesForm::ok()
{
	if(studentsComboBox->currentIndex()<0){
		showWarningCannotModifyConstraintInvisibleSubgroupConstraint(this, this->_ctr->students);
		return;
	}

	double weight;
	QString tmp=weightLineEdit->text();
	weight_sscanf(tmp, "%lf", &weight);
	if(weight<100.0 || weight>100.0){
		QMessageBox::warning(this, tr("FET information"),
			tr("Invalid weight (percentage). It has to be 100"));
		return;
	}

	QString students_name=studentsComboBox->currentText();
	StudentsSet* s=gt.rules.searchStudentsSet(students_name);
	if(s==NULL){
		QMessageBox::warning(this, tr("FET information"),
			tr("Invalid students set"));
		return;
	}

	foreach(TimeConstraint* c, gt.rules.timeConstraintsList)
		if(c!=this->_ctr && c->type==CONSTRAINT_STUDENTS_SET_NOT_AVAILABLE_TIMES){
			ConstraintStudentsSetNotAvailableTimes* cc=(ConstraintStudentsSetNotAvailableTimes*)c;
			if(cc->students==students_name){
				QMessageBox::warning(this, tr("FET information"),
				tr("A constraint of this type exists for the same students set - cannot proceed"));
				return;
			}
		}
		
	this->_ctr->weightPercentage=weight;
	
	if(_ctr->students!=students_name){
		QString oldName=_ctr->students;
		QString newName=students_name;

		QSet<ConstraintStudentsSetNotAvailableTimes*> cs=gt.rules.ssnatHash.value(oldName, QSet<ConstraintStudentsSetNotAvailableTimes*>());
		assert(cs.contains(_ctr));
		cs.remove(_ctr);
		gt.rules.ssnatHash.insert(oldName, cs);

		cs=gt.rules.ssnatHash.value(newName, QSet<ConstraintStudentsSetNotAvailableTimes*>());
		assert(!cs.contains(_ctr));
		cs.insert(_ctr);
		gt.rules.ssnatHash.insert(newName, cs);

		this->_ctr->students=students_name;
	}

	QList<int> days;
	QList<int> hours;
	for(int j=0; j<gt.rules.nDaysPerWeek; j++)
		for(int i=0; i<gt.rules.nHoursPerDay; i++)
			if(notAllowedTimesTable->isMarked(i, j)){
				days.append(j);
				hours.append(i);
			}

	this->_ctr->days=days;
	this->_ctr->hours=hours;

	gt.rules.internalStructureComputed=false;
	gt.rules.setModified(true);
	
	this->close();
}
