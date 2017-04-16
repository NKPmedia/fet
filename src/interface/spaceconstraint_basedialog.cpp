/***************************************************************************
						  spaceconstraint_basedialog.cpp  -  description
							 -------------------
	begin                : 2017
	copyright            : (C) 2017 by Rodolfo RG
	This file is part of a modification of FET timetable (the original is developed by Liviu Lalescu)
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU Affero General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************/

#include "spaceconstraint_basedialog.h"

#include "fet.h"

SpaceConstraintBaseDialog::SpaceConstraintBaseDialog(QWidget *parent)
	: ConstraintBaseDialog(parent)
{

}

QDialog *SpaceConstraintBaseDialog::createModifyDialog(void *ctr)
{
	return createModifyDialog(static_cast<SpaceConstraint*>(ctr));
}

void SpaceConstraintBaseDialog::doRemoveConstraint(void *ctr)
{
	gt.rules.removeSpaceConstraint(static_cast<SpaceConstraint*>(ctr));
}

void SpaceConstraintBaseDialog::fillConstraintList(QList<void *> &list)
{
	foreach (SpaceConstraint *ctr, gt.rules.spaceConstraintsList){
		if(filterOk(ctr)){
			QString s=ctr->getDescription(gt.rules);
			list.append(ctr);
			constraintsListWidget->addItem(s);
		}
	}
}

QString SpaceConstraintBaseDialog::getConstraintDetailedDescription(const void *ctr) const
{
	return static_cast<const SpaceConstraint *>(ctr)->getDetailedDescription(gt.rules);
}
