#pragma once
#ifndef JRS_STORAGE_BASE
#define JRS_STORAGE_BASE
#include <string>

class StorageBase
{
protected:
	const std::string FilePath;
public:
	virtual void read() = 0;
	virtual void save() const = 0;
	StorageBase(std::string FilePath);
	virtual ~StorageBase() = default;
};
#endif /*JRS_STORAGE_BASE*/
