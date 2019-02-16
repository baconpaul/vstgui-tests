#include "CSvgDisplay.h"
#include <iostream>
#include <iomanip>
#include "vstgui/lib/cdrawcontext.h"

CSvgDisplay::CSvgDisplay(const VSTGUI::CRect &size, std::string fn) : VSTGUI::CView(size)
{
    std::cout << "Reading file " << fn << std::endl;
    image = nsvgParseFromFile(fn.c_str(), "px", 96);
    if( ! image )
    {
        std::cout << "NO Image" << std::endl;
    }
    else
    {
        std::cout << "Image result size is " << image->width << " x " << image->height << std::endl;
    }
}

CSvgDisplay::~CSvgDisplay()
{
    if( image )
        nsvgDelete( image );
}

void CSvgDisplay::draw( VSTGUI::CDrawContext *dc )
{
    VSTGUI::CRect viewS = getViewSize();
    std::cout << "Drawing " << viewS.getTopLeft().x << " " << viewS.getTopLeft().y << " " << viewS.getWidth() << " " << viewS.getHeight() << std::endl;

    dc->setFillColor(VSTGUI::kWhiteCColor);
    dc->drawRect( viewS, VSTGUI::kDrawFilled );
    dc->setFillColor(VSTGUI::kBlackCColor);
    dc->drawRect( viewS, VSTGUI::kDrawStroked );

    VSTGUI::CGraphicsTransform tf = VSTGUI::CGraphicsTransform().translate(viewS.getTopLeft().x,viewS.getTopLeft().y).scale(3.0, 3.0 );
    VSTGUI::CDrawContext::Transform t( *dc, tf );
    for (auto shape = image->shapes; shape != NULL; shape = shape->next) {
        for (auto path = shape->paths; path != NULL; path = path->next) {
            for (auto i = 0; i < path->npts-1; i += 3) {
                float* p = &path->pts[i*2];
                //std::cout << "Drawing point at " << p[ 0 ] << " " << p[1] << std::endl;
                dc->drawPoint(VSTGUI::CPoint(p[0],p[1]),VSTGUI::kRedCColor);
                // drawCubicBez(p[0],p[1], p[2],p[3], p[4],p[5], p[6],p[7]);
            }
	}
}
}
