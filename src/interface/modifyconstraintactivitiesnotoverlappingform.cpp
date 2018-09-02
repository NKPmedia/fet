/***************************************************************************
                          modifyconstraintactivitiesnotoverlappingform.cpp  -  description
                             -------------------
    begin                : Feb 11, 2005
    copyright            : (C) 2005 by Lalescu Liviu
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
#include "centerwidgetonscreen.h"

#include "modifyconstraintactivitiesnotoverlappingform.h"
#include "fet.h"

#include <QListWidget>
#include <QAbstractItemView>
#include <QScrollBar>

#include "fetguisettings.h"
#include "studentscomboboxhelper.h"

ModifyConstraintActivitiesNotOverlappingForm::ModifyConstraintActivitiesNotOverlappingForm(QWidget* parent, ConstraintActivitiesNotOverlapping* ctr): QDialog(parent)
{
	setupUi(this);

	okPushButton->setDefault(true);

	activitiesListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	notOverlappingActivitiesListWidget->setSelectionMode(QAbstractItemView::SingleSelection);

	connect(cancelPushButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(okPushButton, SIGNAL(clicked()), this, SLOT(ok()));
	connect(activitiesListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(addActivity()));
	connect(addAllActivitiesPushButton, SIGNAL(clicked()), this, SLOT(addAllActivities()));
	connect(notOverlappingActivitiesListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(removeActivity()));
	connect(teachersComboBox, SIGNAL(activated(QString)), this, SLOT(filterChanged()));
	connect(studentsComboBox, SIGNAL(activated(QString)), this, SLOT(filterChanged()));
	connect(subjectsComboBox, SIGNAL(activated(QString)), this, SLOT(filterChanged()));
	connect(activityTagsComboBox, SIGNAL(activated(QString)), this, SLOT(filterChanged()));
	connect(clearPushButton, SIGNAL(clicked()), this, SLOT(clear()));

	centerWidgetOnScreen(this);
	restoreFETDialogGeometry(this);

	QSize tmp1=teachersComboBox->minimumSizeHint();
	Q_UNUSED(tmp1);
	QSize tmp2=studentsComboBox->minimumSizeHint();
	Q_UNUSED(tmp2);
	QSize tmp3=subjectsComboBox->minimumSizeHint();
	Q_UNUSED(tmp3);
	QSize tmp4=activityTagsComboBox->minimumSizeHint();
	Q_UNUSED(tmp4);
		
	this->_ctr=ctr;
	
	notOverlappingActivitiesList.clear();
	notOverlappingActivitiesListWidget->clear();
	for(int i=0; i<ctr->n_activities; i++){
		int actId=ctr->activitiesId[i];
		this->notOverlappingActivitiesList.append(actId);
		Activity* act=NULL;
		for(int k=0; k<gt.rules.activitiesList.size(); k++){
			act=gt.rules.activitiesList[k];
			if(act->id==actId)
				break;
		}
		assert(act);
		this->notOverlappingActivitiesListWidget->addItem(act->getDescription());
	}
	
	weightLineEdit->setText(CustomFETString::number(ctr->weightPercentage));
	
	////////////////
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

	filterChanged();
}

ModifyConstraintActivitiesNotOverlappingForm::~ModifyConstraintActivitiesNotOverlappingForm()
{
	saveFETDialogGeometry(this);
}

void ModifyConstraintActivitiesNotOverlappingForm::filterChanged()
{
	activitiesListWidget->clear();
	this->activitiesList.clear();

	for(int i=0; i<gt.rules.activitiesList.size(); i++){
		Activity* ac=gt.rules.activitiesList[i];
		if(filterOk(ac)){
			activitiesListWidget->addItem(ac->getDescription());
			this->activitiesList.append(ac->id);
		}
	}
	
	int q=activitiesListWidget->verticalScrollBar()->minimum();
	activitiesListWidget->verticalScrollBar()->setValue(q);
}

bool ModifyConstraintActivitiesNotOverlappingForm::filterOk(Activity* act)
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

void ModifyConstraintActivitiesNotOverlappingForm::ok()
{
	double weight;
	QString tmp=weightLineEdit->text();
	weight_sscanf(tmp, "%lf", &weight);
	if(weight<0.0 || weight>100.0){
		QMessageBox::warning(this, tr("FET information"),
			tr("Invalid weight (percentage)"));
		return;
	}

	if(this->notOverlappingActivitiesList.count()==0){
		QMessageBox::warning(this, tr("FET information"),
			tr("Empty list of not overlapping activities"));
		return;
	}
	if(this->notOverlappingActivitiesList.count()==1){
		QMessageBox::warning(this, tr("FET information"),
			tr("Only one selected activity"));
		return;
	}
	
	int i;
	QList<int>::iterator it;
	this->_ctr->activitiesId.clear();
	for(i=0, it=this->notOverlappingActivitiesList.begin(); it!=this->notOverlappingActivitiesList.end(); it++, i++){
		this->_ctr->activitiesId.append(*it);
	}
	this->_ctr->n_activities=i;
		
	this->_ctr->weightPercentage=weight;
	
	gt.rules.internalStructureComputed=false;
	gt.rules.setModified(true);
	
	this->close();
}

void ModifyConstraintActivitiesNotOverlappingForm::addActivity()
{
	if(activitiesListWidget->currentRow()<0)
		return;
	int tmp=activitiesListWidget->currentRow();
	int _id=this->activitiesList.at(tmp);
	
	QString actName=activitiesListWidget->currentItem()->text();
	assert(actName!="");
	int i;
	//duplicate?
	for(i=0; i<notOverlappingActivitiesListWidget->count(); i++)
		if(actName==notOverlappingActivitiesListWidget->item(i)->text())
			break;
	if(i<notOverlappingActivitiesListWidget->count())
		return;
	notOverlappingActivitiesListWidget->addItem(actName);
	notOverlappingActivitiesListWidget->setCurrentRow(notOverlappingActivitiesListWidget->count()-1);
	
	this->notOverlappingActivitiesList.append(_id);
}

void ModifyConstraintActivitiesNotOverlappingForm::addAllActivities()
{
	for(int tmp=0; tmp<activitiesListWidget->count(); tmp++){
		int _id=this->activitiesList.at(tmp);
	
		QString actName=activitiesListWidget->item(tmp)->text();
		assert(actName!="");
		int i;
		//duplicate?
		for(i=0; i<notOverlappingActivitiesList.count(); i++)
			if(notOverlappingActivitiesList.at(i)==_id)
				break;
		if(i<notOverlappingActivitiesList.count())
			continue;
		
		notOverlappingActivitiesListWidget->addItem(actName);
		this->notOverlappingActivitiesList.append(_id);
	}
	
	notOverlappingActivitiesListWidget->setCurrentRow(notOverlappingActivitiesListWidget->count()-1);
}

void ModifyConstraintActivitiesNotOverlappingForm::removeActivity()
{
	if(notOverlappingActivitiesListWidget->currentRow()<0 || notOverlappingActivitiesListWidget->count()<=0)
		return;
	int tmp=notOverlappingActivitiesListWidget->currentRow();
	
	notOverlappingActivitiesList.removeAt(tmp);

	notOverlappingActivitiesListWidget->setCurrentRow(-1);
	QListWidgetItem* item=notOverlappingActivitiesListWidget->takeItem(tmp);
	delete item;
	if(tmp<notOverlappingActivitiesListWidget->count())
		notOverlappingActivitiesListWidget->setCurrentRow(tmp);
	else
		notOverlappingActivitiesListWidget->setCurrentRow(notOverlappingActivitiesListWidget->count()-1);
}

void ModifyConstraintActivitiesNotOverlappingForm::clear()
{
	notOverlappingActivitiesListWidget->clear();
	notOverlappingActivitiesList.clear();
}

