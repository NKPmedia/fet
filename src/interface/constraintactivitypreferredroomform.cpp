/***************************************************************************
                          constraintactivitypreferredroomform.cpp  -  description
                             -------------------
    begin                : Feb 13, 2005
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

#include "constraintactivitypreferredroomform.h"
#include "addconstraintactivitypreferredroomform.h"
#include "modifyconstraintactivitypreferredroomform.h"

#include "lockunlock.h"

#include "teacherstudentsetsubjectactivitytag_filterwidget.h"

#include "centerwidgetonscreen.h"

ConstraintActivityPreferredRoomForm::ConstraintActivityPreferredRoomForm(QWidget* parent): SpaceConstraintBaseDialog(parent)
{
	const char *context = "ConstraintActivityPreferredRoomForm_template";
	//: This is the title of the dialog to see the list of all constraints of this type
	setWindowTitle(QCoreApplication::translate(context, "Constraints activity preferred room"));

	TeacherStudentSetSubjectActivityTag_FilterWidget *filterWidget = new TeacherStudentSetSubjectActivityTag_FilterWidget(gt.rules);
	filterWidget->setTeachersVisible(true);
	filterWidget->setStudentSetsVisible(true);
	filterWidget->setSubjectsVisible(true);
	filterWidget->setActivityTagsVisible(true);
	filterWidget->setRoomsVisible(true);
	setFilterWidget(filterWidget);
	connect(filterWidget, &TeacherStudentSetSubjectActivityTag_FilterWidget::FilterChanged, this, &ConstraintActivityPreferredRoomForm::filterChanged);

	restoreFETDialogGeometry(this);
	this->filterChanged();
}

ConstraintActivityPreferredRoomForm::~ConstraintActivityPreferredRoomForm()
{
	saveFETDialogGeometry(this);
}

bool ConstraintActivityPreferredRoomForm::filterOk(const SpaceConstraint* ctr) const
{
	if(ctr->type!=CONSTRAINT_ACTIVITY_PREFERRED_ROOM)
		return false;
		
	const ConstraintActivityPreferredRoom* c=(const ConstraintActivityPreferredRoom*) ctr;
	int id=c->activityId;
	const Activity* act=NULL;
	foreach(const Activity* a, gt.rules.activitiesList) {
		if(a->id==id) {
			act=a;
			break;
		}
	}
	assert(act != NULL);

	const TeacherStudentSetSubjectActivityTag_FilterWidget * filterWidget = static_cast<TeacherStudentSetSubjectActivityTag_FilterWidget*>(getFilterWidget());
	bool found = filterWidget->filterActivity(act);
	if(!found)
		return false;
	
	return c->roomName==filterWidget->room() || filterWidget->room()=="";
}

QDialog * ConstraintActivityPreferredRoomForm::createAddDialog()
{
	return new AddConstraintActivityPreferredRoomForm(this);
}

QDialog * ConstraintActivityPreferredRoomForm::createModifyDialog(SpaceConstraint *ctr)
{
	return new ModifyConstraintActivityPreferredRoomForm(this, (ConstraintActivityPreferredRoom*)ctr);
}

void ConstraintActivityPreferredRoomForm::afterRemoveConstraint()
{
	LockUnlock::computeLockedUnlockedActivitiesOnlySpace(&gt.rules);
	LockUnlock::increaseCommunicationSpinBox();
}

void ConstraintActivityPreferredRoomForm::toggleActiveConstraint(void* ctr, bool checked) const
{
	SpaceConstraintBaseDialog::toggleActiveConstraint(ctr, checked);

	LockUnlock::computeLockedUnlockedActivitiesOnlyTime(&gt.rules);
	LockUnlock::increaseCommunicationSpinBox();
}
