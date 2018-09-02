/***************************************************************************
                          addconstraintactivitiespreferredtimeslotsform.cpp  -  description
                             -------------------
    begin                : 15 May 2004
    copyright            : (C) 2004 by Lalescu Liviu
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

#include "longtextmessagebox.h"
#include "centerwidgetonscreen.h"

#include "addconstraintactivitiespreferredtimeslotsform.h"
#include "timeconstraint.h"

#include "fetguisettings.h"
#include "studentscomboboxhelper.h"

AddConstraintActivitiesPreferredTimeSlotsForm::AddConstraintActivitiesPreferredTimeSlotsForm(QWidget* parent): QDialog(parent)
{
	setupUi(this);

	durationCheckBox->setChecked(false);
	durationSpinBox->setEnabled(false);
	durationSpinBox->setMinimum(1);
	durationSpinBox->setMaximum(gt.rules.nHoursPerDay);
	durationSpinBox->setValue(1);

	addConstraintPushButton->setDefault(true);

	connect(addConstraintPushButton, SIGNAL(clicked()), this, SLOT(addConstraint()));
	connect(closePushButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(setAllAllowedPushButton, SIGNAL(clicked()), this, SLOT(setAllSlotsAllowed()));
	connect(setAllNotAllowedPushButton, SIGNAL(clicked()), this, SLOT(setAllSlotsNotAllowed()));

	centerWidgetOnScreen(this);
	restoreFETDialogGeometry(this);
	
	QSize tmp1=teachersComboBox->minimumSizeHint();
	Q_UNUSED(tmp1);
	QSize tmp2=studentsComboBox->minimumSizeHint();
	Q_UNUSED(tmp2);
	QSize tmp3=subjectsComboBox->minimumSizeHint();
	Q_UNUSED(tmp3);
	QSize tmp4=activityTagsComboBox->minimumSizeHint();
	Q_UNUSED(tmp4);
	
	updateTeachersComboBox();
	updateStudentsComboBox();
	updateSubjectsComboBox();
	updateActivityTagsComboBox();

	preferredTimesTable->setHeaders(gt.rules);
}

AddConstraintActivitiesPreferredTimeSlotsForm::~AddConstraintActivitiesPreferredTimeSlotsForm()
{
	saveFETDialogGeometry(this);
}

void AddConstraintActivitiesPreferredTimeSlotsForm::setAllSlotsAllowed()
{
	preferredTimesTable->setAllUnmarked();
}

void AddConstraintActivitiesPreferredTimeSlotsForm::setAllSlotsNotAllowed()
{
	preferredTimesTable->setAllMarked();
}

void AddConstraintActivitiesPreferredTimeSlotsForm::updateTeachersComboBox(){
	teachersComboBox->clear();
	teachersComboBox->addItem("");
	for(int i=0; i<gt.rules.teachersList.size(); i++){
		Teacher* t=gt.rules.teachersList[i];
		teachersComboBox->addItem(t->name);
	}
}

void AddConstraintActivitiesPreferredTimeSlotsForm::updateStudentsComboBox(){
	StudentsComboBoxHelper::populateStudentsComboBox(gt.rules, studentsComboBox, QString(""), true);
}

void AddConstraintActivitiesPreferredTimeSlotsForm::updateSubjectsComboBox(){
	subjectsComboBox->clear();
	subjectsComboBox->addItem("");
	for(int i=0; i<gt.rules.subjectsList.size(); i++){
		Subject* s=gt.rules.subjectsList[i];
		subjectsComboBox->addItem(s->name);
	}
}

void AddConstraintActivitiesPreferredTimeSlotsForm::updateActivityTagsComboBox(){
	activityTagsComboBox->clear();
	activityTagsComboBox->addItem("");
	for(int i=0; i<gt.rules.activityTagsList.size(); i++){
		ActivityTag* s=gt.rules.activityTagsList[i];
		activityTagsComboBox->addItem(s->name);
	}
}

void AddConstraintActivitiesPreferredTimeSlotsForm::addConstraint()
{
	TimeConstraint *ctr=NULL;

	int duration=-1;
	if(durationCheckBox->isChecked()){
		assert(durationSpinBox->isEnabled());
		duration=durationSpinBox->value();
	}

	double weight;
	QString tmp=weightLineEdit->text();
	weight_sscanf(tmp, "%lf", &weight);
	if(weight<0.0 || weight>100.0){
		QMessageBox::warning(this, tr("FET information"),
			tr("Invalid weight (percentage)"));
		return;
	}

	/*bool compulsory=false;
	if(compulsoryCheckBox->isChecked())
		compulsory=true;*/

	QString teacher=teachersComboBox->currentText();
	if(teacher!="")
		assert(gt.rules.searchTeacher(teacher)>=0);

	QString students=studentsComboBox->currentText();
	if(students!="")
		assert(gt.rules.searchStudentsSet(students)!=NULL);

	QString subject=subjectsComboBox->currentText();
	if(subject!="")
		assert(gt.rules.searchSubject(subject)>=0);
		
	QString activityTag=activityTagsComboBox->currentText();
	if(activityTag!="")
		assert(gt.rules.searchActivityTag(activityTag)>=0);
		
	if(duration==-1 && teacher=="" && students=="" && subject=="" && activityTag==""){
		int t=QMessageBox::question(this, tr("FET question"),
		 tr("You specified all the activities. This might be a small problem: if you specify"
		  " a not allowed slot between two allowed slots, this not allowed slot will"
		  " be counted as a gap in the teachers' and students' timetable.\n\n"
		  " The best practice would be to use constraint break times.\n\n"
		  " If you need weight under 100%, then you can use this constraint, but be careful"
		  " not to obtain an impossible timetable (if your teachers/students are constrained on gaps"
		  " or early gaps and if you leave a not allowed slot between 2 allowed slots or"
		  " a not allowed slot early in the day and more allowed slots after it,"
		  " this possible gap might be counted in teachers' and students' timetable)")
		  +"\n\n"+tr("Do you want to add current constraint?"),
		 QMessageBox::Yes, QMessageBox::Cancel);
						 
		if(t==QMessageBox::Cancel)
				return;
	}

	if(duration==-1 && teacher!="" && students=="" && subject=="" && activityTag==""){
		int t=QMessageBox::question(this, tr("FET question"),
		 tr("You specified only the teacher. This might be a small problem: if you specify"
		  " a not allowed slot between two allowed slots, this not allowed slot will"
		  " be counted as a gap in the teacher's timetable.\n\n"
		  " The best practice would be to use constraint teacher not available times.\n\n"
		  " If you need weight under 100%, then you can use this constraint, but be careful"
		  " not to obtain an impossible timetable (if your teacher is constrained on gaps"
		  " and if you leave a not allowed slot between 2 allowed slots, this possible"
		  " gap might be counted in teacher's timetable)")
		  +"\n\n"+tr("Do you want to add current constraint?"),
		 QMessageBox::Yes, QMessageBox::Cancel);
						 
		if(t==QMessageBox::Cancel)
				return;
	}
	if(duration==-1 && teacher=="" && students!="" && subject=="" && activityTag==""){
		int t=QMessageBox::question(this, tr("FET question"),
		 tr("You specified only the students set. This might be a small problem: if you specify"
		  " a not allowed slot between two allowed slots (or a not allowed slot before allowed slots),"
		  " this not allowed slot will"
		  " be counted as a gap (or early gap) in the students' timetable.\n\n"
		  " The best practice would be to use constraint students set not available times.\n\n"
		  " If you need weight under 100%, then you can use this constraint, but be careful"
		  " not to obtain an impossible timetable (if your students set is constrained on gaps or early gaps"
		  " and if you leave a not allowed slot between 2 allowed slots (or a not allowed slot before allowed slots), this possible"
		  " gap might be counted in students' timetable)")
		  +"\n\n"+tr("Do you want to add current constraint?"),
		 QMessageBox::Yes, QMessageBox::Cancel);
						 
		if(t==QMessageBox::Cancel)
				return;
	}

	QList<int> days_L;
	QList<int> hours_L;
	/*int days[MAX_N_CONSTRAINT_ACTIVITIES_PREFERRED_TIME_SLOTS];
	int hours[MAX_N_CONSTRAINT_ACTIVITIES_PREFERRED_TIME_SLOTS];*/
	int n=0;
	for(int j=0; j<gt.rules.nDaysPerWeek; j++)
		for(int i=0; i<gt.rules.nHoursPerDay; i++)
			if(!preferredTimesTable->isMarked(i, j)){
				/*if(n>=MAX_N_CONSTRAINT_ACTIVITIES_PREFERRED_TIME_SLOTS){
					QString s=tr("Not enough slots (too many \"Yes\" values).");
					s+="\n";
					s+=tr("Please increase the variable MAX_N_CONSTRAINT_ACTIVITIES_PREFERRED_TIME_SLOTS");
					s+="\n";
					s+=tr("Currently, it is %1").arg(MAX_N_CONSTRAINT_ACTIVITIES_PREFERRED_TIME_SLOTS);
					QMessageBox::warning(this, tr("FET information"), s);
					
					return;
				}*/
				
				days_L.append(j);
				hours_L.append(i);
				n++;
			}

	if(n<=0){
		int t=QMessageBox::question(this, tr("FET question"),
		 tr("Warning: 0 slots selected. Are you sure?"),
		 QMessageBox::Yes, QMessageBox::Cancel);
						 
		if(t==QMessageBox::Cancel)
				return;
	}

	ctr=new ConstraintActivitiesPreferredTimeSlots(weight, /*compulsory,*/ teacher, students, subject, activityTag, duration, n, days_L, hours_L);

	bool tmp2=gt.rules.addTimeConstraint(ctr);
	if(tmp2){
		QString s=tr("Constraint added:");
		s+="\n\n";
		s+=ctr->getDetailedDescription(gt.rules);
		LongTextMessageBox::information(this, tr("FET information"), s);
	}
	else{
		QMessageBox::warning(this, tr("FET information"),
			tr("Constraint NOT added - duplicate?", "'Duplicate' means that the constraint is identical to an existing constraint and cannot be added."));
		delete ctr;
	}
}

void AddConstraintActivitiesPreferredTimeSlotsForm::on_durationCheckBox_toggled()
{
	durationSpinBox->setEnabled(durationCheckBox->isChecked());
}
