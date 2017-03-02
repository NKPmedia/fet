/***************************************************************************
                          constraintstudentsearlymaxbeginningsatsecondhourform.h  -  description
                             -------------------
    begin                : Feb 11, 2005
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

#ifndef CONSTRAINTSTUDENTSEARLYMAXBEGINNINGSATSECONDHOURFORM_H
#define CONSTRAINTSTUDENTSEARLYMAXBEGINNINGSATSECONDHOURFORM_H

#include "constraint_basedialog.h"

class ConstraintStudentsEarlyMaxBeginningsAtSecondHourForm : public ConstraintBaseDialog {
	Q_OBJECT
public:
	ConstraintStudentsEarlyMaxBeginningsAtSecondHourForm(QWidget* parent);
	~ConstraintStudentsEarlyMaxBeginningsAtSecondHourForm();

protected:
	virtual QDialog *createAddDialog();
	virtual QDialog *createModifyDialog(TimeConstraint *ctr);
	virtual bool filterOk(const TimeConstraint *ctr) const;
};

#endif
