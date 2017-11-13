#ifndef TEACHERSTUDENTSETSUBJECTACTIVITYTAG_FILTERWIDGET_H
#define TEACHERSTUDENTSETSUBJECTACTIVITYTAG_FILTERWIDGET_H

#include <QWidget>
#include <QBoxLayout>
#include "rules.h"

namespace Ui {
class TeacherStudentSetSubjectActivityTag_FilterWidget;
}

class TeacherStudentSetSubjectActivityTag_FilterWidget : public QWidget
{
	Q_OBJECT

signals:
	 void FilterChanged();

public:
	explicit TeacherStudentSetSubjectActivityTag_FilterWidget(const Rules &rules);
	~TeacherStudentSetSubjectActivityTag_FilterWidget();

	QString teacher() const;
	QString studentsSet() const;
	QString subject() const;
	QString activityTag() const;
	QString room() const;

	void setTeachersVisible(bool visible = true);
	void setStudentSetsVisible(bool visible = true);
	void setSubjectsVisible(bool visible = true);
	void setActivityTagsVisible(bool visible = true);
	void setRoomsVisible(bool visible = true);

	/// Hide filter item labels
	void hideLabels();

	/// Organize filter items horizontally or vertically
	void setDirection(QBoxLayout::Direction direction);

	/// Check if given activity matches selected filter parameters
	bool filterActivity(const Activity *activity) const;
	/// Check if given activity set matches selected filter parameters
	/// It does not check whether every activity matches.
	bool filterActivitySet(const QSet<const Activity *> &activities) const;

private:
	Ui::TeacherStudentSetSubjectActivityTag_FilterWidget *ui;

	void populateTeachers(const Rules& rules);
	void populateStudentsSets(const Rules& rules);
	void populateSubjects(const Rules& rules);
	void populateActivityTags(const Rules& rules);
	void populateRooms(const Rules& rules);

private slots:
	void onChange();
};

#endif // TEACHERSTUDENTSETSUBJECTACTIVITYTAG_FILTERWIDGET_H
