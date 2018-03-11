//
//
// Description: This file is part of FET
//
//
/***************************************************************************
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU Affero General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************/

#include "errorcode.h"

#include <QCoreApplication>

ErrorCode::ErrorCode()
	: severity(NoError), groupId(0)
{
}

ErrorCode::ErrorCode(ErrorCode::Severity severity, QString message, int groupId)
	: severity(severity), message(message), groupId(groupId)
{
}

bool ErrorCode::isError() const
{
	return severity == Error || severity == Fatal;
}

QString ErrorCode::getSeverityTitle() const
{
	return getSeverityTitle(severity);
}

QString ErrorCode::getSeverityTitle(ErrorCode::Severity severity)
{
	switch (severity) {
	case Fatal:
		return QCoreApplication::translate("Rules", "FET critical");
	case Error:
		return QCoreApplication::translate("Rules", "FET error");
	case Warning:
		return QCoreApplication::translate("Rules", "FET warning");
	case Info:
		return QCoreApplication::translate("Rules", "FET information");
	default:
		return QCoreApplication::translate("Rules", "FET message");
	}
	// "FET confirmation" "FET help" "FET question"
}

int ErrorCode::nextGroupId()
{
	return ++_nextGroupId;
}

int ErrorCode::_nextGroupId = 0;

ErrorCode::operator bool() const
{
	return severity != NoError;
}

bool ErrorList::hasError() const
{
	foreach (const ErrorCode& erc, *this) {
		if (erc.isError())
			return true;
	}
	return false;
}

bool ErrorList::hasFatal() const
{
	foreach (const ErrorCode& erc, *this) {
		if (erc.severity == ErrorCode::Fatal)
			return true;
	}
	return false;
}
