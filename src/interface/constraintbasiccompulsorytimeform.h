/***************************************************************************
                          constraintbasiccompulsorytimeform.h  -  description
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

#ifndef CONSTRAINTBASICCOMPULSORYTIMEFORM_H
#define CONSTRAINTBASICCOMPULSORYTIMEFORM_H

#include "timeconstraint_basedialog.h"

class ConstraintBasicCompulsoryTimeForm : public TimeConstraintBaseDialog  {
	Q_OBJECT
public:
	ConstraintBasicCompulsoryTimeForm(QWidget* parent);
	~ConstraintBasicCompulsoryTimeForm();

protected:
	virtual QDialog *createAddDialog();
	virtual QDialog *createModifyDialog(TimeConstraint *ctr);
	bool beforeRemoveConstraint();
	virtual bool filterOk(const TimeConstraint *ctr) const;
};

#endif
