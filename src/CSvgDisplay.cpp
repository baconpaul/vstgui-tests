#include "CSvgDisplay.h"
#include <iostream>
#include <iomanip>
#include "vstgui/lib/cdrawcontext.h"

CSvgDisplay::CSvgDisplay(const VSTGUI::CRect &size, std::string fn) : VSTGUI::CView(size)
{
    std::cout << "Reading file " << fn << std::endl;
}

void CSvgDisplay::draw( VSTGUI::CDrawContext *dc )
{
    VSTGUI::CRect viewS = getViewSize();
    std::cout << "Drawing " << viewS.getTopLeft().x << " " << viewS.getTopLeft().y << " " << viewS.getWidth() << " " << viewS.getHeight() << std::endl;

    dc->setFillColor(VSTGUI::kWhiteCColor);
    dc->drawRect( viewS, VSTGUI::kDrawFilled );
    dc->setFillColor(VSTGUI::kBlackCColor);
    dc->drawRect( viewS, VSTGUI::kDrawStroked );
}
