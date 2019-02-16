#pragma once

#include <functional>
#include "vstgui/lib/vstguifwd.h"

void cocoa_minimal_main(std::function<void(VSTGUI::CFrame *f)> frameCb);
