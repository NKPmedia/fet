/***************************************************************************
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU Affero General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************/

#include <QString>
#include <QtTest>

#include "constraints/paradoxmindaysvstwogrouped.h"

#include "generate_pre.h"
#include "timetable.h"
Timetable gt; // Just to please generate_pre.cpp that yet require it. It must be not used in tests

class ParadoxMinDaysVsTwoGroupedTest : public QObject
{
	Q_OBJECT

public:
	ParadoxMinDaysVsTwoGroupedTest();

private:
	Rules& rules;
	void populate_basic_data();

private slots:
	void init();
	void cleanup();
	void test_generate_error_message();
	void test_generate_error_message_reverse_order();
	void test_generate_multiple_error_messages();
	void test_reset_properly();
};

ParadoxMinDaysVsTwoGroupedTest::ParadoxMinDaysVsTwoGroupedTest()
	: rules(gt.rules)
{
}

void ParadoxMinDaysVsTwoGroupedTest::populate_basic_data()
{
	QStringList teachers;
	QStringList activitytags;
	QStringList studentsnames;
	QString subject("subject1");

	Rules &rules = gt.rules;
	if (!rules.initialized)
		rules.init();
	Subject *psubject = new Subject();
	psubject->name = subject;
	rules.addSubjectFast(psubject);
	rules.addSimpleActivityFast(12345, 12345, teachers, subject, activitytags, studentsnames, 1, 1, true, false, 10, 10);
	rules.addSimpleActivityFast(23456, 23456, teachers, subject, activitytags, studentsnames, 1, 1, true, false, 10, 10);
	rules.addSimpleActivityFast(34567, 34567, teachers, subject, activitytags, studentsnames, 1, 1, true, false, 10, 10);
	rules.computeInternalStructure();

	TimeConstraint * ctr = new ConstraintBasicCompulsoryTime();
	rules.addTimeConstraint(ctr);
	ConstraintBasicCompulsorySpace * sctr = new ConstraintBasicCompulsorySpace();
	rules.addSpaceConstraint(sctr);
}

void ParadoxMinDaysVsTwoGroupedTest::init()
{
	rules.init();
	populate_basic_data();
}

void ParadoxMinDaysVsTwoGroupedTest::cleanup()
{
	rules.kill();
}

void ParadoxMinDaysVsTwoGroupedTest::test_generate_error_message()
{
	QList<int> acts;
	acts << 12345 << 23456;

	TimeConstraint * ctr = new ConstraintMinDaysBetweenActivities(100, false, 2, acts, 1);
	rules.addTimeConstraint(ctr);
	ctr = new ConstraintTwoActivitiesGrouped(100, 12345, 23456);
	rules.addTimeConstraint(ctr);
	rules.computeInternalStructure();

	ParadoxMinDaysVsTwoGrouped paradox;
	processTimeSpaceConstraints(NULL);
	bool result = paradox.prepare(rules);
	QVERIFY2(!result, "Paradox found");

	QCOMPARE(paradox.getErrors().size(), 1);
}

void ParadoxMinDaysVsTwoGroupedTest::test_generate_error_message_reverse_order()
{
	QList<int> acts;
	acts << 12345 << 23456;

	TimeConstraint * ctr = new ConstraintMinDaysBetweenActivities(100, false, 2, acts, 1);
	rules.addTimeConstraint(ctr);
	ctr = new ConstraintTwoActivitiesGrouped(100, 23456, 12345);
	rules.addTimeConstraint(ctr);
	rules.computeInternalStructure();

	ParadoxMinDaysVsTwoGrouped paradox;
	processTimeSpaceConstraints(NULL);
	bool result = paradox.prepare(rules);
	QVERIFY2(!result, "Paradox found");

	QCOMPARE(paradox.getErrors().size(), 1);
}

void ParadoxMinDaysVsTwoGroupedTest::test_generate_multiple_error_messages()
{
	QList<int> acts;
	acts << 12345 << 23456 << 34567;

	TimeConstraint * ctr = new ConstraintMinDaysBetweenActivities(100, false, 3, acts, 1);
	rules.addTimeConstraint(ctr);
	ctr = new ConstraintTwoActivitiesGrouped(100, 12345, 23456);
	rules.addTimeConstraint(ctr);
	ctr = new ConstraintTwoActivitiesGrouped(100, 34567, 23456);
	rules.addTimeConstraint(ctr);
	rules.computeInternalStructure();

	ParadoxMinDaysVsTwoGrouped paradox;
	processTimeSpaceConstraints(NULL);
	bool result = paradox.prepare(rules);
	QVERIFY2(!result, "Paradox found");

	QCOMPARE(paradox.getErrors().size(), 2);
}

void ParadoxMinDaysVsTwoGroupedTest::test_reset_properly()
{
	QList<int> acts;
	acts << 12345 << 23456;

	TimeConstraint * ctr = new ConstraintMinDaysBetweenActivities(100, false, 2, acts, 1);
	rules.addTimeConstraint(ctr);
	ctr = new ConstraintTwoActivitiesGrouped(100, 12345, 23456);
	rules.addTimeConstraint(ctr);
	rules.computeInternalStructure();

	ParadoxMinDaysVsTwoGrouped paradox;
	processTimeSpaceConstraints(NULL);
	bool result = paradox.prepare(rules);
	QVERIFY2(!result, "Paradox found");

	QCOMPARE(paradox.getErrors().size(), 1);

	result = paradox.prepare(rules);
	QVERIFY2(!result, "Paradox found");

	QCOMPARE(paradox.getErrors().size(), 1);
}

QTEST_APPLESS_MAIN(ParadoxMinDaysVsTwoGroupedTest)

#include "tst_paradoxmindaysvstwogroupedtest.moc"
