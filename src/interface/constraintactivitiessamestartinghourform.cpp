/***************************************************************************
                          constraintactivitiessamestartinghourform.cpp  -  description
                             -------------------
    begin                : 15 Feb 2005
    copyright            : (C) 2005 by Lalescu Liviu
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

#include "constraintactivitiessamestartinghourform.h"
#include "addconstraintactivitiessamestartinghourform.h"
#include "modifyconstraintactivitiessamestartinghourform.h"

#include "teacherstudentsetsubjectactivitytag_filterwidget.h"

ConstraintActivitiesSameStartingHourForm::ConstraintActivitiesSameStartingHourForm(QWidget* parent): TimeConstraintBaseDialog(parent)
{
	//: This is the title of the dialog to see the list of all constraints of this type
	setWindowTitle(QCoreApplication::translate("ConstraintActivitiesSameStartingHourForm_template", "Constraints activities same starting hour"));

	restoreFETDialogGeometry(this);

	TeacherStudentSetSubjectActivityTag_FilterWidget *filterWidget = new TeacherStudentSetSubjectActivityTag_FilterWidget(gt.rules);
	filterWidget->setTeachersVisible(true);
	filterWidget->setStudentSetsVisible(true);
	filterWidget->setSubjectsVisible(true);
	filterWidget->setActivityTagsVisible(true);
	setFilterWidget(filterWidget);
	connect(filterWidget, &TeacherStudentSetSubjectActivityTag_FilterWidget::FilterChanged, this, &ConstraintActivitiesSameStartingHourForm::filterChanged);

	filterChanged();
}

ConstraintActivitiesSameStartingHourForm::~ConstraintActivitiesSameStartingHourForm()
{
	saveFETDialogGeometry(this);
}

bool ConstraintActivitiesSameStartingHourForm::filterOk(const TimeConstraint* ctr) const
{
	if(ctr->type!=CONSTRAINT_ACTIVITIES_SAME_STARTING_HOUR)
		return false;

	ConstraintActivitiesSameStartingHour* c=(ConstraintActivitiesSameStartingHour*) ctr;
	const TeacherStudentSetSubjectActivityTag_FilterWidget * filterWidget = static_cast<TeacherStudentSetSubjectActivityTag_FilterWidget*>(getFilterWidget());
	QString tn=filterWidget->teacher();
	QString sbn=filterWidget->subject();
	QString sbtn=filterWidget->activityTag();
	QString stn=filterWidget->studentsSet();

	if(tn=="" && sbn=="" && sbtn=="" && stn=="")
		return true;
	
	bool foundTeacher=false, foundStudents=false, foundSubject=false, foundActivityTag=false;
		
	for(int i=0; i<c->n_activities; i++){
		//bool found=true;
	
		int id=c->activitiesId[i];
		Activity* act=NULL;
		foreach(Activity* a, gt.rules.activitiesList) {
			if(a->id==id) {
				act=a;
				break;
			}
		}
		
		if(act!=NULL){
			//teacher
			if(tn!=""){
				bool ok2=false;
				for(QStringList::Iterator it=act->teachersNames.begin(); it!=act->teachersNames.end(); it++)
					if(*it == tn){
						ok2=true;
						break;
					}
				if(ok2)
					foundTeacher=true;
			}
			else
				foundTeacher=true;

			//subject
			if(sbn!="" && sbn!=act->subjectName)
				;
			else
				foundSubject=true;
		
			//activity tag
			if(sbtn!="" && !act->activityTagsNames.contains(sbtn))
				;
			else
				foundActivityTag=true;
		
			//students
			if(stn!=""){
				bool ok2=false;
				for(QStringList::Iterator it=act->studentsNames.begin(); it!=act->studentsNames.end(); it++)
					if(*it == stn){
						ok2=true;
						break;
				}
				if(ok2)
					foundStudents=true;
			}
			else
				foundStudents=true;
		}
	}
	
	if(foundTeacher && foundStudents && foundSubject && foundActivityTag)
		return true;
	else
		return false;
}

QDialog * ConstraintActivitiesSameStartingHourForm::createAddDialog()
{
	return new AddConstraintActivitiesSameStartingHourForm(this);
}

QDialog * ConstraintActivitiesSameStartingHourForm::createModifyDialog(TimeConstraint* ctr)
{
	return new ModifyConstraintActivitiesSameStartingHourForm(this, (ConstraintActivitiesSameStartingHour*)ctr);
}
