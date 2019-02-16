#include "vstgui/lib/cview.h"

class CSvgDisplay : public VSTGUI::CView
{
public:
    CSvgDisplay(const VSTGUI::CRect &size, std::string fname);

    virtual void draw(VSTGUI::CDrawContext *p) override;
};
