/***************************************************************************
                          addconstraintactivitiesoccupymaxtimeslotsfromselectionform.cpp  -  description
                             -------------------
    begin                : Sept 26, 2011
    copyright            : (C) 2011 by Lalescu Liviu
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

#include <QMessageBox>

#include "longtextmessagebox.h"
#include "centerwidgetonscreen.h"

#include "addconstraintactivitiesoccupymaxtimeslotsfromselectionform.h"
#include "timeconstraint.h"

#include <QListWidget>
#include <QAbstractItemView>
#include <QScrollBar>

#include "fetguisettings.h"
#include "studentscomboboxhelper.h"

AddConstraintActivitiesOccupyMaxTimeSlotsFromSelectionForm::AddConstraintActivitiesOccupyMaxTimeSlotsFromSelectionForm(QWidget* parent): QDialog(parent)
{
	setupUi(this);

	addConstraintPushButton->setDefault(true);
	
	allActivitiesListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	selectedActivitiesListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	
	connect(addConstraintPushButton, SIGNAL(clicked()), this, SLOT(addCurrentConstraint()));
	connect(closePushButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(setAllUnselectedPushButton, SIGNAL(clicked()), this, SLOT(setAllUnselected()));
	connect(setAllSelectedPushButton, SIGNAL(clicked()), this, SLOT(setAllSelected()));
	connect(allActivitiesListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(addActivity()));
	connect(addAllActivitiesPushButton, SIGNAL(clicked()), this, SLOT(addAllActivities()));
	connect(selectedActivitiesListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(removeActivity()));
	connect(clearPushButton, SIGNAL(clicked()), this, SLOT(clear()));
	connect(teachersComboBox, SIGNAL(activated(QString)), this, SLOT(filterChanged()));
	connect(studentsComboBox, SIGNAL(activated(QString)), this, SLOT(filterChanged()));
	connect(subjectsComboBox, SIGNAL(activated(QString)), this, SLOT(filterChanged()));
	connect(activityTagsComboBox, SIGNAL(activated(QString)), this, SLOT(filterChanged()));
	
	centerWidgetOnScreen(this);
	restoreFETDialogGeometry(this);
							
	tabWidget->setCurrentIndex(0);

	maxOccupiedSpinBox->setMinimum(0);
	maxOccupiedSpinBox->setMaximum(gt.rules.nDaysPerWeek*gt.rules.nHoursPerDay);
	maxOccupiedSpinBox->setValue(0);

	selectedTimesTable->setHeaders(gt.rules);
	
	//activities
	QSize tmp1=teachersComboBox->minimumSizeHint();
	Q_UNUSED(tmp1);
	QSize tmp2=studentsComboBox->minimumSizeHint();
	Q_UNUSED(tmp2);
	QSize tmp3=subjectsComboBox->minimumSizeHint();
	Q_UNUSED(tmp3);
	QSize tmp4=activityTagsComboBox->minimumSizeHint();
	Q_UNUSED(tmp4);

	teachersComboBox->addItem("");
	for(int i=0; i<gt.rules.teachersList.size(); i++){
		Teacher* tch=gt.rules.teachersList[i];
		teachersComboBox->addItem(tch->name);
	}
	teachersComboBox->setCurrentIndex(0);

	subjectsComboBox->addItem("");
	for(int i=0; i<gt.rules.subjectsList.size(); i++){
		Subject* sb=gt.rules.subjectsList[i];
		subjectsComboBox->addItem(sb->name);
	}
	subjectsComboBox->setCurrentIndex(0);

	activityTagsComboBox->addItem("");
	for(int i=0; i<gt.rules.activityTagsList.size(); i++){
		ActivityTag* st=gt.rules.activityTagsList[i];
		activityTagsComboBox->addItem(st->name);
	}
	activityTagsComboBox->setCurrentIndex(0);

	StudentsComboBoxHelper::populateStudentsComboBox(gt.rules, studentsComboBox, QString(""), true);
	studentsComboBox->setCurrentIndex(0);

	selectedActivitiesListWidget->clear();
	selectedActivitiesList.clear();

	filterChanged();
}

AddConstraintActivitiesOccupyMaxTimeSlotsFromSelectionForm::~AddConstraintActivitiesOccupyMaxTimeSlotsFromSelectionForm()
{
	saveFETDialogGeometry(this);
}

void AddConstraintActivitiesOccupyMaxTimeSlotsFromSelectionForm::setAllUnselected()
{
	selectedTimesTable->setAllUnmarked();
}

void AddConstraintActivitiesOccupyMaxTimeSlotsFromSelectionForm::setAllSelected()
{
	selectedTimesTable->setAllMarked();
}

void AddConstraintActivitiesOccupyMaxTimeSlotsFromSelectionForm::addCurrentConstraint()
{
	TimeConstraint *ctr=NULL;

	double weight;
	QString tmp=weightLineEdit->text();
	weight_sscanf(tmp, "%lf", &weight);
	if(weight<100.0 || weight>100.0){
		QMessageBox::warning(this, tr("FET information"),
			tr("Invalid weight (percentage). It has to be 100"));
		return;
	}

	QList<int> days;
	QList<int> hours;
	for(int j=0; j<gt.rules.nDaysPerWeek; j++)
		for(int i=0; i<gt.rules.nHoursPerDay; i++)
			if(selectedTimesTable->isMarked(i, j)){
				days.append(j);
				hours.append(i);
			}
			
	int maxOccupiedSlots=maxOccupiedSpinBox->value();
	
	if(maxOccupiedSlots==0){
		QMessageBox::warning(this, tr("FET information"), tr("You specified max occupied time slots to be 0. This is "
		 "not perfect from efficiency point of view, because you can use instead constraint activity(ies) preferred time slots, "
		 "and help FET to find a timetable easier and faster, with an equivalent result. Please correct."));
		return;
	}
	
	if(this->selectedActivitiesList.count()==0){
		QMessageBox::warning(this, tr("FET information"),
		 tr("Empty list of activities"));
		return;
	}
	//we allow even only one activity
	/*if(this->selectedActivitiesList.count()==1){
		QMessageBox::warning(this, tr("FET information"),
		 tr("Only one selected activity"));
		return;
	}*/

	ctr=new ConstraintActivitiesOccupyMaxTimeSlotsFromSelection(weight, selectedActivitiesList, days, hours, maxOccupiedSlots);

	bool tmp2=gt.rules.addTimeConstraint(ctr);
	if(tmp2)
		LongTextMessageBox::information(this, tr("FET information"),
			tr("Constraint added:")+"\n\n"+ctr->getDetailedDescription(gt.rules));
	else{
		QMessageBox::warning(this, tr("FET information"),
			tr("Constraint NOT added - please report error"));
		delete ctr;
	}
}

//activities
bool AddConstraintActivitiesOccupyMaxTimeSlotsFromSelectionForm::filterOk(Activity* act)
{
	QString tn=teachersComboBox->currentText();
	QString stn=studentsComboBox->currentText();
	QString sbn=subjectsComboBox->currentText();
	QString sbtn=activityTagsComboBox->currentText();
	int ok=true;

	//teacher
	if(tn!=""){
		bool ok2=false;
		for(QStringList::Iterator it=act->teachersNames.begin(); it!=act->teachersNames.end(); it++)
			if(*it == tn){
				ok2=true;
				break;
			}
		if(!ok2)
			ok=false;
	}

	//subject
	if(sbn!="" && sbn!=act->subjectName)
		ok=false;
		
	//activity tag
	if(sbtn!="" && !act->activityTagsNames.contains(sbtn))
		ok=false;
		
	//students
	if(stn!=""){
		bool ok2=false;
		for(QStringList::Iterator it=act->studentsNames.begin(); it!=act->studentsNames.end(); it++)
			if(*it == stn){
				ok2=true;
				break;
			}
		if(!ok2)
			ok=false;
	}
	
	return ok;
}

void AddConstraintActivitiesOccupyMaxTimeSlotsFromSelectionForm::filterChanged()
{
	this->updateActivitiesListWidget();
}

void AddConstraintActivitiesOccupyMaxTimeSlotsFromSelectionForm::updateActivitiesListWidget()
{
	allActivitiesListWidget->clear();
	this->activitiesList.clear();
	
	for(int i=0; i<gt.rules.activitiesList.size(); i++){
		Activity* ac=gt.rules.activitiesList[i];
		if(filterOk(ac)){
			allActivitiesListWidget->addItem(ac->getDescription());
			this->activitiesList.append(ac->id);
		}
	}
	
	int q=allActivitiesListWidget->verticalScrollBar()->minimum();
	allActivitiesListWidget->verticalScrollBar()->setValue(q);
}

void AddConstraintActivitiesOccupyMaxTimeSlotsFromSelectionForm::addActivity()
{
	if(allActivitiesListWidget->currentRow()<0)
		return;
	int tmp=allActivitiesListWidget->currentRow();
	int _id=this->activitiesList.at(tmp);
	
	QString actName=allActivitiesListWidget->currentItem()->text();
	assert(actName!="");
	int i;
	//duplicate?
	for(i=0; i<selectedActivitiesListWidget->count(); i++)
		if(actName==selectedActivitiesListWidget->item(i)->text())
			break;
	if(i<selectedActivitiesListWidget->count())
		return;
	selectedActivitiesListWidget->addItem(actName);
	selectedActivitiesListWidget->setCurrentRow(selectedActivitiesListWidget->count()-1);
	
	this->selectedActivitiesList.append(_id);
}

void AddConstraintActivitiesOccupyMaxTimeSlotsFromSelectionForm::addAllActivities()
{
	for(int tmp=0; tmp<allActivitiesListWidget->count(); tmp++){
		//int tmp=allActivitiesListWidget->currentRow();
		int _id=this->activitiesList.at(tmp);
	
		QString actName=allActivitiesListWidget->item(tmp)->text();
		assert(actName!="");
		int i;
		//duplicate?
		for(i=0; i<selectedActivitiesList.count(); i++)
			if(selectedActivitiesList.at(i)==_id)
				break;
		if(i<selectedActivitiesList.count())
			continue;
		
		/*for(i=0; i<selectedActivitiesListWidget->count(); i++)
			if(actName==selectedActivitiesListWidget->item(i)->text())
				break;
		if(i<selectedActivitiesListWidget->count())
			continue;*/
			
		selectedActivitiesListWidget->addItem(actName);
		this->selectedActivitiesList.append(_id);
	}
	
	selectedActivitiesListWidget->setCurrentRow(selectedActivitiesListWidget->count()-1);
}

void AddConstraintActivitiesOccupyMaxTimeSlotsFromSelectionForm::removeActivity()
{
	if(selectedActivitiesListWidget->currentRow()<0 || selectedActivitiesListWidget->count()<=0)
		return;
	int tmp=selectedActivitiesListWidget->currentRow();
	
	selectedActivitiesList.removeAt(tmp);

	selectedActivitiesListWidget->setCurrentRow(-1);
	QListWidgetItem* item=selectedActivitiesListWidget->takeItem(tmp);
	delete item;
	if(tmp<selectedActivitiesListWidget->count())
		selectedActivitiesListWidget->setCurrentRow(tmp);
	else
		selectedActivitiesListWidget->setCurrentRow(selectedActivitiesListWidget->count()-1);
}

void AddConstraintActivitiesOccupyMaxTimeSlotsFromSelectionForm::clear()
{
	selectedActivitiesListWidget->clear();
	selectedActivitiesList.clear();
}
