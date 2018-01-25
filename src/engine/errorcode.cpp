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
	: severity(NO_ERROR)
{
}

ErrorCode::ErrorCode(ErrorCode::Severity severity, QString message)
	: severity(severity), message(message)
{
}

bool ErrorCode::isError() const
{
	return severity == ERROR || severity == FATAL;
}

QString ErrorCode::getSeverityTitle() const
{
	return getSeverityTitle(severity);
}

QString ErrorCode::getSeverityTitle(ErrorCode::Severity severity)
{
	switch (severity) {
	case FATAL:
		return QCoreApplication::translate("Rules", "FET critical");
	case ERROR:
		return QCoreApplication::translate("Rules", "FET error");
	case WARNING:
		return QCoreApplication::translate("Rules", "FET warning");
	case INFO:
		return QCoreApplication::translate("Rules", "FET information");
	default:
		return QCoreApplication::translate("Rules", "FET message");
	}
	// "FET confirmation" "FET help" "FET question"
}

ErrorCode::operator bool() const
{
	return severity != NO_ERROR;
}
