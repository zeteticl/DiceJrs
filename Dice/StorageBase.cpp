#include "StorageBase.h"
#include <utility>

StorageBase::StorageBase(std::string FilePath) : FilePath(std::move(FilePath))
{
}
