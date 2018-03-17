/***************************************************************************
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU Affero General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************/

#include "paradoxmindaysvstwogrouped.h"
#include "generate_pre.h"

ParadoxMinDaysVsTwoGrouped::ParadoxMinDaysVsTwoGrouped()
{

}

bool ParadoxMinDaysVsTwoGrouped::prepare(const Rules& rules)
{
	bool ok = true;
	int errorGroupId = ErrorCode::nextGroupId();
	errors.clear();

	QSet<QPair<int,int> > groups;

	for(int ai1=0; ai1<rules.nInternalActivities; ai1++){
		for (int i = 0; i < constrTwoActivitiesGroupedPercentages[ai1].size(); i++){
			if (constrTwoActivitiesGroupedPercentages[ai1][i] >= 100) {
				int ai2 = constrTwoActivitiesGroupedActivities[ai1][i];
				// check whether this group was already visited, but in reversed order
				const QPair<int,int> g(ai1, ai2);
				const QPair<int,int> invg(ai2, ai1);
				if (groups.contains(invg))
					continue;
				groups << g;
				// search for contradictory constraints Min days between ai1 and ai2
				for (int mai1 = 0; mai1 < minDaysBetweenActivitiesList.activities[ai1].size(); mai1++) {
					if (minDaysBetweenActivitiesList.activities[ai1][mai1] == ai2 && minDaysBetweenActivitiesList.weightPercentages[ai1][mai1] >= 100) {
						if (minDaysBetweenActivitiesList.minDays[ai1][mai1] > 0) {
							QString s=GeneratePreTranslate::tr("There are contradictory constraints with activities %1 and %2.")
							 .arg(rules.internalActivitiesList[ai1].id)
							 .arg(rules.internalActivitiesList[ai2].id);
							s+="\n";
							s+=GeneratePreTranslate::tr("There should be %1 min days between them but they must also be grouped activities.")
							 .arg(minDaysBetweenActivitiesList.minDays[ai1][mai1]);
							s+=QString("\n----\n%1\n----\n%2")
							 .arg(rules.internalActivitiesList[ai1].getDescription())
							 .arg(rules.internalActivitiesList[ai2].getDescription());
							s+="\n\n";
							s+=GeneratePreTranslate::tr("Please modify your data accordingly and try again");

							errors << ErrorCode(ErrorCode::Error, s, errorGroupId);

							ok = false;
						}
					}
				}
			}
		}
	}
	return ok;
}
