#pragma once

#include <QString>

#include "abstractdatastorage.h"
#include "controller_global.h"

namespace Controller {

class CONTROLLER_EXPORT ControllerApi {
public:
	QString name() const;
};

} // namespace Controller

