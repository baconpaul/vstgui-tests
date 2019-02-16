#include "CSvgDisplay.h"
#include <iostream>
#include <iomanip>
#include "vstgui/lib/cdrawcontext.h"
#include "vstgui/lib/cgraphicspath.h"

CSvgDisplay::CSvgDisplay(const VSTGUI::CRect &size, std::string fn) : VSTGUI::CView(size), image( nullptr )
{
    resetFile( fn );
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
    dc->setFrameColor(VSTGUI::kBlackCColor);
    dc->drawRect( viewS, VSTGUI::kDrawStroked );

    float scaleF = std::min( viewS.getWidth() / image->width, viewS.getHeight() / image->height );
    scaleF *= 0.9;
    std::cout << "Scale Factor is " << scaleF << std::endl;

    VSTGUI::CGraphicsTransform tf = VSTGUI::CGraphicsTransform().scale(scaleF, scaleF).translate( viewS.getTopLeft().x + 5, viewS.getTopLeft().y + 5 );
    
    
    VSTGUI::CDrawContext::Transform t( *dc, tf );
    for (auto shape = image->shapes; shape != NULL; shape = shape->next) {
		if (!(shape->flags & NSVG_FLAGS_VISIBLE))
			continue;
        
        // std::cout << "Shape ID=" << shape->id << " FT=" << (int)shape->fill.type << " ST=" << (int)shape->stroke.type << std::endl;
        //VSTGUI::CRect boundsRect(VSTGUI::CPoint(shape->bounds[0],shape->bounds[1]),
//                                 VSTGUI::CPoint(shape->bounds[2]-shape->bounds[0],shape->bounds[3]-shape->bounds[1]));
        //dc->setFillColor(VSTGUI::kRedCColor);
        //dc->drawRect(boundsRect, VSTGUI::kDrawStroked);

        
        for (auto path = shape->paths; path != NULL; path = path->next) {
            //std::cout << "   PATH with " << path->npts << std::endl;
            VSTGUI::CGraphicsPath *gp = dc->createGraphicsPath();

            for (auto i = 0; i < path->npts-1; i += 3) {
                float* p = &path->pts[i*2];

                if( i == 0 )
                    gp->beginSubpath(p[0],p[1]);
                gp->addBezierCurve(p[2],p[3], p[4],p[5], p[6],p[7]);
                //std::cout << "Drawing point at " << p[ 0 ] << " " << p[1] << std::endl;
                //dc->drawLine(VSTGUI::CPoint(p[0],p[1]),VSTGUI::CPoint(p[6],p[7]));
                // drawCubicBez(p[0],p[1],
            }
            if( shape->fill.type != NSVG_PAINT_NONE )
            {
                if( shape->fill.type == NSVG_PAINT_COLOR )
                {
                    dc->setFillColor( svgColorToCColor( shape->fill.color ) );
                }
                else
                {
                    std::cerr << "No gradient support yet" << std::endl;
                    dc->setFillColor( VSTGUI::kRedCColor );
                }
                dc->drawGraphicsPath(gp, VSTGUI::CDrawContext::kPathFilled );
            }
            if( shape->stroke.type != NSVG_PAINT_NONE )
            {
                if( shape->stroke.type == NSVG_PAINT_COLOR )
                {
                    dc->setFrameColor( svgColorToCColor( shape->stroke.color ) );
                }
                else
                {
                    std::cerr << "No gradient support yet" << std::endl;
                    dc->setFrameColor( VSTGUI::kRedCColor );
                }

                dc->setLineWidth(shape->strokeWidth);
                dc->drawGraphicsPath(gp, VSTGUI::CDrawContext::kPathStroked );
            }
            gp->forget();
        }
    }
}

void CSvgDisplay::resetFile(std::string fname)
{
    std::cout << "Reading file " << fname << std::endl;

    if( image )
    {
        nsvgDelete( image );
    }

    image = nsvgParseFromFile(fname.c_str(), "px", 96);
    if( ! image )
    {
        std::cout << "NO Image" << std::endl;
    }
    else
    {
        std::cout << "Image result size is " << image->width << " x " << image->height << std::endl;
    }

}
VSTGUI::CColor CSvgDisplay::svgColorToCColor(int svgColor)
{
    int a = ( svgColor & 0xFF000000 ) >> 24;
    int b = ( svgColor & 0x00FF0000 ) >> 16;
    int g = ( svgColor & 0x0000FF00 ) >> 8;
    int r = ( svgColor & 0x000000FF );
    return VSTGUI::CColor( r, g, b, a );
}
