#include "UUIDUtils.h"

#include <uuid/uuid.h>

std::string UUIDUtils::generateUUID()
{
    uuid_t uuid;
    char str[36];

    uuid_generate(uuid);
    uuid_unparse(uuid, str);

    std::string result = str;
    return result;
}