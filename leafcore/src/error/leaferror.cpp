/**
 * @file        error/leaferror.cpp
 * @author      Max Kofler (kofler.max.dev@gmail.com)
 * @brief       The implementations of the LeafError class
 * @copyright   Copyright (c) 2022 Max Kofler and the AcaciaLinux developers
 */

#include "log.h"
#include "leafdebug.h"

#include "error.h"

LeafError::LeafError(const Error::ec& errorCode){
    FUN();

    _errorCode = errorCode;
}

LeafError::LeafError(const Error::ec& errorCode, const std::string& additional){
    FUN();

    _errorCode = errorCode;
    _additional = additional;
}

LeafError::LeafError(const Error::ec& errorCode, const std::error_code& stdErrorCode){
    FUN();

    _errorCode = errorCode;
    _stdErrorCode = stdErrorCode;
}

LeafError::LeafError(const Error::ec& errorCode, const std::string& additional, const std::error_code& stdErrorCode){
    FUN();

    _errorCode = errorCode;
    _additional = additional;
    _stdErrorCode = stdErrorCode;
}

LeafError::~LeafError() noexcept(false){
    FUN();
    LEAF_DEBUG_EX("LeafError::~LeafError()");
}

std::string LeafError::what() const{
    std::string ret = getErrorCodeMessage();

    if (!_additional.empty())
        ret += ": " + _additional;

    if (_stdErrorCode)
        ret += ": " + _stdErrorCode.message();

    return ret;
}

Error::ec LeafError::getErrorCode() const{
    return _errorCode;
}

