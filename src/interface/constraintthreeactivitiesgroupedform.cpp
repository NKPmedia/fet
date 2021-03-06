/***************************************************************************
                          constraintthreeactivitiesgroupedform.cpp  -  description
                             -------------------
    begin                : Aug 14, 2009
    copyright            : (C) 2009 by Lalescu Liviu
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

#include "constraintthreeactivitiesgroupedform.h"
#include "addconstraintthreeactivitiesgroupedform.h"
#include "modifyconstraintthreeactivitiesgroupedform.h"

#include "teacherstudentsetsubjectactivitytag_filterwidget.h"

#include "centerwidgetonscreen.h"

ConstraintThreeActivitiesGroupedForm::ConstraintThreeActivitiesGroupedForm(QWidget* parent): TimeConstraintBaseDialog(parent)
{
	//: This is the title of the dialog to see the list of all constraints of this type
	setWindowTitle(QCoreApplication::translate("ConstraintThreeActivitiesGroupedForm_template", "Constraints three activies grouped"));

	QString instruction = QCoreApplication::translate("ConstraintThreeActivitiesGroupedForm_template", "This constraint forces 3 activities to be in the same day, one immediately following the other, as a block of 3 activities, in any order, possibly separated by breaks");
	instruction += "\n\n" + QCoreApplication::translate("ConstraintThreeActivitiesGroupedForm_template", "It is a good practice NOT to add this type of constraint to activities which are constrained to be in different days by min days between activities constraints. Please read Help/Important tips, tip number 3).");
	setInstructionText(instruction);

	restoreFETDialogGeometry(this);

	TeacherStudentSetSubjectActivityTag_FilterWidget *filterWidget = new TeacherStudentSetSubjectActivityTag_FilterWidget(gt.rules);
	filterWidget->setTeachersVisible(true);
	filterWidget->setStudentSetsVisible(true);
	filterWidget->setSubjectsVisible(true);
	filterWidget->setActivityTagsVisible(true);
	setFilterWidget(filterWidget);
	connect(filterWidget, &TeacherStudentSetSubjectActivityTag_FilterWidget::FilterChanged, this, &ConstraintThreeActivitiesGroupedForm::filterChanged);

	this->filterChanged();
}

ConstraintThreeActivitiesGroupedForm::~ConstraintThreeActivitiesGroupedForm()
{
	saveFETDialogGeometry(this);
}

bool ConstraintThreeActivitiesGroupedForm::filterOk(const TimeConstraint* ctr) const
{
	if(ctr->type!=CONSTRAINT_THREE_ACTIVITIES_GROUPED)
		return false;

	const ConstraintThreeActivitiesGrouped* c=(const ConstraintThreeActivitiesGrouped*) ctr;
	QSet<int> activitiesIds;
	activitiesIds << c->firstActivityId << c->secondActivityId << c->thirdActivityId;

	QSet<const Activity *> activities;
	for(int id : qAsConst(activitiesIds)){
		for(const Activity* a : qAsConst(gt.rules.activitiesList)) {
			if(a->id==id) {
				activities << a;
				break;
			}
		}
	}

	TeacherStudentSetSubjectActivityTag_FilterWidget *filter_widget = static_cast<TeacherStudentSetSubjectActivityTag_FilterWidget*>(getFilterWidget());
	return filter_widget->filterActivitySet(activities);
}

QDialog * ConstraintThreeActivitiesGroupedForm::createAddDialog()
{
	return new AddConstraintThreeActivitiesGroupedForm(this);
}

QDialog * ConstraintThreeActivitiesGroupedForm::createModifyDialog(TimeConstraint *ctr)
{
	return new ModifyConstraintThreeActivitiesGroupedForm(this, (ConstraintThreeActivitiesGrouped*)ctr);
}
