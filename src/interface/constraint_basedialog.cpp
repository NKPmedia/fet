/***************************************************************************
						  constraint_basedialog.cpp  -  description
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

#include "constraint_basedialog.h"

#include <QMessageBox>
#include <QScrollBar>

#include <cassert>

#include "longtextmessagebox.h"

#include "centerwidgetonscreen.h"

ConstraintBaseDialog::ConstraintBaseDialog(QWidget* parent): QDialog(parent),
	  filterWidget(nullptr)
{
	setupUi(this);

	currentConstraintTextEdit->setReadOnly(true);

	modifyConstraintPushButton->setDefault(true);

	helpPushButton->setVisible(false);

	instructionsLabel->setVisible(false);

	constraintsListWidget->setSelectionMode(QAbstractItemView::SingleSelection);

	connect(constraintsListWidget, SIGNAL(currentRowChanged(int)), this, SLOT(constraintChanged(int)));
	connect(addConstraintPushButton, SIGNAL(clicked()), this, SLOT(addConstraint()));
	connect(closePushButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(removeConstraintPushButton, SIGNAL(clicked()), this, SLOT(removeConstraint()));
	connect(modifyConstraintPushButton, SIGNAL(clicked()), this, SLOT(modifyConstraint()));
	connect(activeCheckBox, SIGNAL(clicked(bool)), this, SLOT(toggleActiveConstraint(bool)));
	connect(constraintsListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(modifyConstraint()));
	connect(helpPushButton, SIGNAL(clicked()), this, SLOT(help()));

	centerWidgetOnScreen(this);

	modifyConstraintPushButton->setEnabled(false);
	removeConstraintPushButton->setEnabled(false);
	activeCheckBox->setEnabled(false);
//	populateFilters();
//	filterChanged();
}

ConstraintBaseDialog::~ConstraintBaseDialog()
{
	delete filterWidget;
}

void ConstraintBaseDialog::setFilterWidget(QWidget *widget)
{
	if (filterWidget != nullptr) {
		rightVerticalLayout->removeWidget(filterWidget);
	}
	filterWidget = widget;
	if (widget != nullptr) {
		rightVerticalLayout->addWidget(widget);
	}
}

void ConstraintBaseDialog::filterChanged()
{
	visibleConstraintsList.clear();
	constraintsListWidget->clear();

	fillConstraintList(visibleConstraintsList);

	if(constraintsListWidget->count()>0)
		constraintsListWidget->setCurrentRow(0);
	else
		this->constraintChanged(-1);
}

void ConstraintBaseDialog::constraintChanged(int index)
{
	if(index<0){
		currentConstraintTextEdit->setPlainText("");
		modifyConstraintPushButton->setEnabled(false);
		removeConstraintPushButton->setEnabled(false);
		activeCheckBox->setEnabled(false);
		activeCheckBox->setChecked(false);
		return;
	}

	assert(index<this->visibleConstraintsList.size());
	QString s=getConstraintDetailedDescription(visibleConstraintsList.at(index));
	currentConstraintTextEdit->setPlainText(s);
	modifyConstraintPushButton->setEnabled(true);
	removeConstraintPushButton->setEnabled(true);
	activeCheckBox->setEnabled(true);
	activeCheckBox->setChecked(isConstraintActive(visibleConstraintsList.at(index)));
}

void ConstraintBaseDialog::addConstraint()
{
	QDialog *form = createAddDialog();
//	setParentAndOtherThings(form, this);
	form->exec();
	delete form;

	filterChanged();

	constraintsListWidget->setCurrentRow(constraintsListWidget->count()-1);
}

void ConstraintBaseDialog::modifyConstraint()
{
	int valv=constraintsListWidget->verticalScrollBar()->value();
	int valh=constraintsListWidget->horizontalScrollBar()->value();

	int i=constraintsListWidget->currentRow();
	if(i<0){
		QMessageBox::information(this, tr("FET information"), tr("Invalid selected constraint"));
		return;
	}
	void* ctr=this->visibleConstraintsList.at(i);

	QDialog *form = createModifyDialog(ctr);
//	setParentAndOtherThings(form, this);
	form->exec();
	delete form;

	filterChanged();

	constraintsListWidget->verticalScrollBar()->setValue(valv);
	constraintsListWidget->horizontalScrollBar()->setValue(valh);

	if(i>=constraintsListWidget->count())
		i=constraintsListWidget->count()-1;

	if(i>=0)
		constraintsListWidget->setCurrentRow(i);
	else
		this->constraintChanged(-1);
}

void ConstraintBaseDialog::removeConstraint()
{
	int i=constraintsListWidget->currentRow();
	if(i<0){
		QMessageBox::information(this, tr("FET information"), tr("Invalid selected constraint"));
		return;
	}
	void* ctr=this->visibleConstraintsList.at(i);
	QString s;
	s=tr("Remove constraint?");
	s+="\n\n";
	s+=getConstraintDetailedDescription(ctr);

	QListWidgetItem* item;

	switch( LongTextMessageBox::confirmation( this, tr("FET confirmation"),
		s, tr("Yes"), tr("No"), 0, 0, 1 ) ){
	case 0: // The user clicked the OK button or pressed Enter
		if (!beforeRemoveConstraint())
			break;

		doRemoveConstraint(ctr);

		visibleConstraintsList.removeAt(i);
		constraintsListWidget->setCurrentRow(-1);
		item=constraintsListWidget->takeItem(i);
		delete item;

		afterRemoveConstraint();

		break;
	case 1: // The user clicked the Cancel button or pressed Escape
		break;
	}

	if(i>=constraintsListWidget->count())
		i=constraintsListWidget->count()-1;
	if(i>=0)
		constraintsListWidget->setCurrentRow(i);
	else
		this->constraintChanged(-1);
}

void ConstraintBaseDialog::toggleActiveConstraint(bool checked)
{
	int i=constraintsListWidget->currentRow();
	if(i<0){
		QMessageBox::information(this, tr("FET information"), tr("Invalid selected constraint"));
		return;
	}

	void* ctr=this->visibleConstraintsList.at(i);
	toggleActiveConstraint(ctr, checked);

	filterChanged();
	constraintsListWidget->setCurrentRow(i);
}

bool ConstraintBaseDialog::beforeRemoveConstraint()
{
	return true;
}

void ConstraintBaseDialog::afterRemoveConstraint()
{
}

void ConstraintBaseDialog::setHelpText(QString msg)
{
	helpPushButton->setHidden(msg.isEmpty());
	helpMsg = msg;
}

void ConstraintBaseDialog::help()
{
	LongTextMessageBox::largeInformation(this, tr("FET help"), helpMsg);
}

QWidget *ConstraintBaseDialog::getFilterWidget() const
{
	return filterWidget;
}

void ConstraintBaseDialog::setInstructionText(QString msg)
{
	instructionsLabel->setHidden(msg.isEmpty());
	instructionsLabel->setText(msg);
}
