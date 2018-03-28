/***************************************************************************
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU Affero General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************/

#include <QtTest>

#include "constraints/paradoxmingapsvs2grouped.h"

#include "generate_pre.h"
#include "timetable.h"
Timetable gt; // Just to please generate_pre.cpp that yet require it. It must be not used in tests

class ParadoxMinGapsVs2GroupedTest : public QObject
{
	Q_OBJECT

public:
	ParadoxMinGapsVs2GroupedTest();

private Q_SLOTS:
	void init();
	void cleanup();
	void test_generate_error_message();
	void test_generate_error_message_reverse_order();
	void test_generate_multiple_error_messages();
	void test_reset_properly();
	void test_added_to_generate();
};

ParadoxMinGapsVs2GroupedTest::ParadoxMinGapsVs2GroupedTest()
{
}

void ParadoxMinGapsVs2GroupedTest::init()
{
	gt.rules.init();
}

void ParadoxMinGapsVs2GroupedTest::cleanup()
{
	gt.rules.kill();
}

void ParadoxMinGapsVs2GroupedTest::test_generate_error_message()
{
	QStringList teachers;
	QStringList activitytags;
	QStringList studentsnames;
	QString subject("subject1");
	QList<int> acts;
	acts << 12345 << 23456;

	Rules &rules = gt.rules;
	if (!rules.initialized)
		rules.init();
	Subject *psubject = new Subject();
	psubject->name = subject;
	rules.addSubjectFast(psubject);
	rules.addSimpleActivityFast(12345, 12345, teachers, subject, activitytags, studentsnames, 1, 1, true, false, 10, 10);
	rules.addSimpleActivityFast(23456, 23456, teachers, subject, activitytags, studentsnames, 1, 1, true, false, 10, 10);
	rules.computeInternalStructure();

	TimeConstraint * ctr = new ConstraintBasicCompulsoryTime();
	rules.addTimeConstraint(ctr);
	ConstraintBasicCompulsorySpace * sctr = new ConstraintBasicCompulsorySpace();
	rules.addSpaceConstraint(sctr);
	ctr = new ConstraintMinGapsBetweenActivities(100, 2, acts, 1);
	rules.addTimeConstraint(ctr);
	ctr = new ConstraintTwoActivitiesGrouped(100, 12345, 23456);
	rules.addTimeConstraint(ctr);
	rules.computeInternalStructure();

	ParadoxMinGapsVs2Grouped paradox;
	processTimeSpaceConstraints(NULL);
	bool result = paradox.prepare(rules);
	QVERIFY2(!result, "Paradox found");

	QCOMPARE(paradox.getErrors().size(), 1);
}

void ParadoxMinGapsVs2GroupedTest::test_generate_error_message_reverse_order()
{
	QStringList teachers;
	QStringList activitytags;
	QStringList studentsnames;
	QString subject("subject1");
	QList<int> acts;
	acts << 12345 << 23456;

	Rules &rules = gt.rules;
	if (!rules.initialized)
		rules.init();
	Subject *psubject = new Subject();
	psubject->name = subject;
	rules.addSubjectFast(psubject);
	rules.addSimpleActivityFast(12345, 12345, teachers, subject, activitytags, studentsnames, 1, 1, true, false, 10, 10);
	rules.addSimpleActivityFast(23456, 23456, teachers, subject, activitytags, studentsnames, 1, 1, true, false, 10, 10);
	rules.computeInternalStructure();

	TimeConstraint * ctr = new ConstraintBasicCompulsoryTime();
	rules.addTimeConstraint(ctr);
	ConstraintBasicCompulsorySpace * sctr = new ConstraintBasicCompulsorySpace();
	rules.addSpaceConstraint(sctr);
	ctr = new ConstraintMinGapsBetweenActivities(100, 2, acts, 1);
	rules.addTimeConstraint(ctr);
	ctr = new ConstraintTwoActivitiesGrouped(100, 23456, 12345);
	rules.addTimeConstraint(ctr);
	rules.computeInternalStructure();

	ParadoxMinGapsVs2Grouped paradox;
	processTimeSpaceConstraints(NULL);
	bool result = paradox.prepare(rules);
	QVERIFY2(!result, "Paradox found");

	QCOMPARE(paradox.getErrors().size(), 1);
}

void ParadoxMinGapsVs2GroupedTest::test_generate_multiple_error_messages()
{
	QStringList teachers;
	QStringList activitytags;
	QStringList studentsnames;
	QString subject("subject1");
	QList<int> acts;
	acts << 12345 << 23456 << 34567;

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
	ctr = new ConstraintMinGapsBetweenActivities(100, 3, acts, 1);
	rules.addTimeConstraint(ctr);
	ctr = new ConstraintTwoActivitiesGrouped(100, 12345, 23456);
	rules.addTimeConstraint(ctr);
	ctr = new ConstraintTwoActivitiesGrouped(100, 23456, 34567);
	rules.addTimeConstraint(ctr);
	rules.computeInternalStructure();

	ParadoxMinGapsVs2Grouped paradox;
	processTimeSpaceConstraints(NULL);
	bool result = paradox.prepare(rules);
	QVERIFY2(!result, "Paradox found");

	QCOMPARE(paradox.getErrors().size(), 2);
}

void ParadoxMinGapsVs2GroupedTest::test_reset_properly()
{
	QStringList teachers;
	QStringList activitytags;
	QStringList studentsnames;
	QString subject("subject1");
	QList<int> acts;
	acts << 12345 << 23456;

	Rules &rules = gt.rules;
	if (!rules.initialized)
		rules.init();
	Subject *psubject = new Subject();
	psubject->name = subject;
	rules.addSubjectFast(psubject);
	rules.addSimpleActivityFast(12345, 12345, teachers, subject, activitytags, studentsnames, 1, 1, true, false, 10, 10);
	rules.addSimpleActivityFast(23456, 23456, teachers, subject, activitytags, studentsnames, 1, 1, true, false, 10, 10);
	rules.computeInternalStructure();

	TimeConstraint * ctr = new ConstraintBasicCompulsoryTime();
	rules.addTimeConstraint(ctr);
	ConstraintBasicCompulsorySpace * sctr = new ConstraintBasicCompulsorySpace();
	rules.addSpaceConstraint(sctr);
	ctr = new ConstraintMinGapsBetweenActivities(100, 2, acts, 1);
	rules.addTimeConstraint(ctr);
	ctr = new ConstraintTwoActivitiesGrouped(100, 12345, 23456);
	rules.addTimeConstraint(ctr);
	rules.computeInternalStructure();

	ParadoxMinGapsVs2Grouped paradox;
	processTimeSpaceConstraints(NULL);
	bool result = paradox.prepare(rules);
	QVERIFY2(!result, "Paradox found");

	QCOMPARE(paradox.getErrors().size(), 1);

	result = paradox.prepare(rules);
	QVERIFY2(!result, "Paradox found");

	QCOMPARE(paradox.getErrors().size(), 1);
}

void ParadoxMinGapsVs2GroupedTest::test_added_to_generate()
{
	QStringList teachers;
	QStringList activitytags;
	QStringList studentsnames;
	QString subject("subject1");
	QList<int> acts;
	acts << 12345 << 23456;

	Rules &rules = gt.rules;
	if (!rules.initialized)
		rules.init();
	Subject *psubject = new Subject();
	psubject->name = subject;
	rules.addSubjectFast(psubject);
	rules.addSimpleActivityFast(12345, 12345, teachers, subject, activitytags, studentsnames, 1, 1, true, false, 10, 10);
	rules.addSimpleActivityFast(23456, 23456, teachers, subject, activitytags, studentsnames, 1, 1, true, false, 10, 10);
	rules.computeInternalStructure();

	TimeConstraint * ctr = new ConstraintBasicCompulsoryTime();
	rules.addTimeConstraint(ctr);
	ConstraintBasicCompulsorySpace * sctr = new ConstraintBasicCompulsorySpace();
	rules.addSpaceConstraint(sctr);
	ctr = new ConstraintMinGapsBetweenActivities(100, 2, acts, 1);
	rules.addTimeConstraint(ctr);
	ctr = new ConstraintTwoActivitiesGrouped(100, 12345, 23456);
	rules.addTimeConstraint(ctr);
	rules.computeInternalStructure();

	bool success = processTimeSpaceConstraints(NULL);
	QVERIFY2(!success, "Paradox not checked in generate_pre");
}

QTEST_APPLESS_MAIN(ParadoxMinGapsVs2GroupedTest)

#include "tst_paradoxmingapsvs2groupedtest.moc"
