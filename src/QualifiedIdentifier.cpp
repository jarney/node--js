#include "node--js/QualifiedIdentifier.hpp"

using namespace NodeJS::core;

const std::string &
QualifiedIdentifier::getName() const
{ return mName; }

bool
QualifiedIdentifier::isQualified() const
{ return mIsQualified; }

const std::string &
QualifiedIdentifier::getNamespace() const
{ return mNamespace; }

QualifiedIdentifier::QualifiedIdentifier(
    std::string aQualifiedIdentifier
    )
{
    auto findpos = aQualifiedIdentifier.rfind('.');
    if (findpos == std::string::npos) {
	mName = aQualifiedIdentifier;
	mIsQualified = false;
	mNamespace = "";
    }
    else {
	mName = aQualifiedIdentifier.substr(findpos+1, std::string::npos);
	mNamespace = aQualifiedIdentifier.substr(0, findpos);
	mIsQualified = true;
    }
    
}


