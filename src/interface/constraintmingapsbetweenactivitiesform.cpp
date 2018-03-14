/***************************************************************************
                          constraintmingapsbetweenactivitiesform.cpp  -  description
                             -------------------
    begin                : July 10, 2008
    copyright            : (C) 2008 by Lalescu Liviu
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

#include "constraintmingapsbetweenactivitiesform.h"
#include "addconstraintmingapsbetweenactivitiesform.h"
#include "modifyconstraintmingapsbetweenactivitiesform.h"

#include "teacherstudentsetsubjectactivitytag_filterwidget.h"

#include "centerwidgetonscreen.h"

ConstraintMinGapsBetweenActivitiesForm::ConstraintMinGapsBetweenActivitiesForm(QWidget* parent): TimeConstraintBaseDialog(parent)
{
	//: This is the title of the dialog to see the list of all constraints of this type
	setWindowTitle(QCoreApplication::translate("ConstraintMinGapsBetweenActivitiesForm_template", "Constraints min gaps between activities"));

	QString instruction = QCoreApplication::translate("ConstraintMinGapsBetweenActivitiesForm_template", "This constraint ensures, for a set of activities: if any pair of activities is in the same day, they must be separated by at least min gaps (hours).");
	instruction += "\n\n" + QCoreApplication::translate("ConstraintMinGapsBetweenActivitiesForm_template", "Note: please click Help for important advice");
	setInstructionText(instruction);

	setHelp();

	TeacherStudentSetSubjectActivityTag_FilterWidget *filterWidget = new TeacherStudentSetSubjectActivityTag_FilterWidget(gt.rules);
	filterWidget->setTeachersVisible(true);
	filterWidget->setStudentSetsVisible(true);
	filterWidget->setSubjectsVisible(true);
	filterWidget->setActivityTagsVisible(true);
	setFilterWidget(filterWidget);
	connect(filterWidget, &TeacherStudentSetSubjectActivityTag_FilterWidget::FilterChanged, this, &ConstraintMinGapsBetweenActivitiesForm::filterChanged);

	restoreFETDialogGeometry(this);
	this->filterChanged();
}

ConstraintMinGapsBetweenActivitiesForm::~ConstraintMinGapsBetweenActivitiesForm()
{
	saveFETDialogGeometry(this);
}

bool ConstraintMinGapsBetweenActivitiesForm::filterOk(const TimeConstraint* ctr) const
{
	if(ctr->type!=CONSTRAINT_MIN_GAPS_BETWEEN_ACTIVITIES)
		return false;
		
	const ConstraintMinGapsBetweenActivities* c=(const ConstraintMinGapsBetweenActivities*) ctr;
	QSet<const Activity *> activities;
	foreach(int id, c->activitiesId){
		foreach(const Activity* a, gt.rules.activitiesList) {
			if(a->id==id) {
				activities << a;
				break;
			}
		}
	}

	TeacherStudentSetSubjectActivityTag_FilterWidget *filter_widget = static_cast<TeacherStudentSetSubjectActivityTag_FilterWidget*>(getFilterWidget());
	return filter_widget->filterActivitySet(activities);
}

QDialog * ConstraintMinGapsBetweenActivitiesForm::createAddDialog()
{
	return new AddConstraintMinGapsBetweenActivitiesForm(this);
}

QDialog * ConstraintMinGapsBetweenActivitiesForm::createModifyDialog(TimeConstraint *ctr)
{
	return new ModifyConstraintMinGapsBetweenActivitiesForm(this, (ConstraintMinGapsBetweenActivities*)ctr);
}

void ConstraintMinGapsBetweenActivitiesForm::setHelp()
{
	QString s=tr("Please make sure that the selected activities are not forced to be"
		" consecutive by some other constraint 'min days between activities' (with"
		" 'consecutive if same day' true), by a constraint 'two activities consecutive',"
		" or by a constraint 'two activities grouped' (also, if you have a constraint"
		" 'three activities grouped' related to the selected activities, make sure that the"
		" constraints do not contradict).");
		
	setHelpText(s);
}
