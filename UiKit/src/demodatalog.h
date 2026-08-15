#pragma once

#include <QLoggingCategory>

// Shared by DemoDataWorker (which does the actual logging, from the
// background thread) and DemoDataGenerator (a couple of lifecycle/guard
// messages on the GUI thread).
Q_DECLARE_LOGGING_CATEGORY(demoDataLog)
