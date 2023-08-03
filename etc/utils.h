#pragma once

#include <QString>
#include <exception>

#define FLF QString{"File: "} + __FILE__ + QString{"; Line: "} + QString::number(__LINE__) + QString{"; Function: "} + __FUNCTION__

class InternalErrorException : public std::runtime_error {
public:
	InternalErrorException(const QString& errorString) : std::runtime_error{errorString.toStdString().c_str()} { }
};
