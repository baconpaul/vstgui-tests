#include "vstgui/lib/cview.h"
#include "nanosvg.h"

class CSvgDisplay : public VSTGUI::CView
{
public:
    CSvgDisplay(const VSTGUI::CRect &size, std::string fname);
    virtual ~CSvgDisplay();

    virtual void draw(VSTGUI::CDrawContext *p) override;

private:
    struct NSVGimage *image;
};
