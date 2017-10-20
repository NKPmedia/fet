//
//
// Description: This file is part of FET
//
//
// Author: Liviu Lalescu <Please see https://lalescu.ro/liviu/ for details about contacting Liviu Lalescu (in particular, you can find here the e-mail address)>
// Copyright (C) 2003 Liviu Lalescu <https://lalescu.ro/liviu/>
//
/***************************************************************************
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU Affero General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************/

#include "subject.h"
#include "rules.h"

Subject::Subject()
{
}

Subject::~Subject()
{
}

QString Subject::getXmlDescription() const
{
	QString s="<Subject>\n";
	s+="	<Name>"+protect(name)+"</Name>\n";
	s+="	<Comments>"+protect(comments)+"</Comments>\n";
	s+="</Subject>\n";

	return s;
}

QString Subject::getDescription() const
{
	QString s=tr("N:%1", "The name of the subject").arg(name);
	
	if(!comments.isEmpty())
		s+=", "+tr("C: %1", "Comments").arg(comments);
	
	return s;
}

QString Subject::getDetailedDescription() const
{
	QString s=tr("Subject");
	s+="\n";
	s+=tr("Name=%1", "The name of the subject").arg(name);
	s+="\n";

	//Has comments?
	if(!comments.isEmpty()){
		s+=tr("Comments=%1").arg(comments);
		s+="\n";
	}

	return s;
}

QString Subject::getDetailedDescriptionWithConstraints(const Rules &r) const
{
	QString s=this->getDetailedDescription();

	s+="--------------------------------------------------\n";
	s+=tr("Time constraints directly related to this subject:");
	s+="\n";
	for(TimeConstraintsList::const_iterator it = r.timeConstraintsList.constBegin(); it != r.timeConstraintsList.constEnd(); ++it){
		const TimeConstraint *c = *it;
		if(c->isRelatedToSubject(this)){
			s+="\n";
			s+=c->getDetailedDescription(r);
		}
	}

	s+="--------------------------------------------------\n";
	s+=tr("Space constraints directly related to this subject:");
	s+="\n";
	for(SpaceConstraintsList::const_iterator it = r.spaceConstraintsList.constBegin(); it != r.spaceConstraintsList.constEnd(); ++it){
		const SpaceConstraint *c = *it;
		if(c->isRelatedToSubject(this)){
			s+="\n";
			s+=c->getDetailedDescription(r);
		}
	}
	s+="--------------------------------------------------\n";

	return s;
}

bool subjectsAscending(const Subject* s1, const Subject* s2)
{
	return s1->name.localeAwareCompare(s2->name) < 0;
}
