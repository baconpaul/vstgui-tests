#include "vstgui/lib/cview.h"
#include "nanosvg.h"

class CSvgDisplay : public VSTGUI::CView
{
public:
    CSvgDisplay(const VSTGUI::CRect &size, std::string fname);
    virtual ~CSvgDisplay();

    void resetFile( std::string fname );
    virtual void draw(VSTGUI::CDrawContext *p) override;
    void setZoomCmd( int z )
    {
        zoomCmd = z;
    }
    
private:
    int zoomCmd;
    struct NSVGimage *image;
    VSTGUI::CColor svgColorToCColor(int svgColor, float opacity = 1.0);
};
