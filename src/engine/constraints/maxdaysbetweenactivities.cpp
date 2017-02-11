#include "maxdaysbetweenactivities.h"

#include "generate_pre.h"

QStringList MaxDaysBetweenActivities::getErrors() const
{
	return errors;
}

MaxDaysBetweenActivities::MaxDaysBetweenActivities()
{

}

bool MaxDaysBetweenActivities::prepare(const Rules &rules)
{
	maxDaysListOfActivities.resize(rules.nInternalActivities);
	maxDaysListOfMaxDays.resize(rules.nInternalActivities);
	maxDaysListOfWeightPercentages.resize(rules.nInternalActivities);

	errors.clear();

	bool ok=true;

	for(int j=0; j<rules.nInternalActivities; j++){
		maxDaysListOfActivities[j].clear();
		maxDaysListOfMaxDays[j].clear();
		maxDaysListOfWeightPercentages[j].clear();
	}

	QSet<const ConstraintMaxDaysBetweenActivities*> mdset;

	for(int i=0; i<rules.nInternalTimeConstraints; i++)
		if(rules.internalTimeConstraintsList[i]->type==CONSTRAINT_MAX_DAYS_BETWEEN_ACTIVITIES){
			const ConstraintMaxDaysBetweenActivities* md=
			 (const ConstraintMaxDaysBetweenActivities*)rules.internalTimeConstraintsList[i];

			for(int j=0; j<md->_n_activities; j++){
				int ai1=md->_activities[j];
				for(int k=0; k<md->_n_activities; k++)
					if(j!=k){
						int ai2=md->_activities[k];
						if(ai1==ai2){
							ok=false;

							if(!mdset.contains(md)){
								mdset.insert(md);

								errors.append(
											GeneratePreTranslate::tr("Cannot optimize, because you have a constraint max days between activities with duplicate activities. The constraint "
											"is: %1. Please correct that.").arg(md->getDetailedDescription(rules))
											);
								continue;
							}
						}
						int m=md->maxDays;

						maxDaysListOfActivities[ai1].append(ai2);
						maxDaysListOfMaxDays[ai1].append(m);
						assert(md->weightPercentage >=0 && md->weightPercentage<=100);
						maxDaysListOfWeightPercentages[ai1].append(md->weightPercentage);
					}
			}
		}

	return ok;
}
