#include "CSvgDisplay.h"
#include <iostream>
#include <iomanip>
#include "vstgui/lib/cdrawcontext.h"
#include "vstgui/lib/cgraphicspath.h"
#include "vstgui/lib/cgradient.h"

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

        std::cout << "Shape ID=" << shape->id << " FT=" << (int)shape->fill.type << " ST=" << (int)shape->stroke.type << std::endl;

        //VSTGUI::CRect boundsRect(VSTGUI::CPoint(shape->bounds[0],shape->bounds[1]),
//                                 VSTGUI::CPoint(shape->bounds[2]-shape->bounds[0],shape->bounds[3]-shape->bounds[1]));
        //dc->setFillColor(VSTGUI::kRedCColor);
        //dc->drawRect(boundsRect, VSTGUI::kDrawStroked);


        
        VSTGUI::CGraphicsPath *gp = dc->createGraphicsPath();
        for (auto path = shape->paths; path != NULL; path = path->next) {
            for (auto i = 0; i < path->npts-1; i += 3) {
                float* p = &path->pts[i*2];

                if( i == 0 )
                    gp->beginSubpath(p[0],p[1]);
                gp->addBezierCurve(p[2],p[3], p[4],p[5], p[6],p[7]);
            }
            if( path->closed )
                gp->closeSubpath();
        }
        if( shape->fill.type != NSVG_PAINT_NONE )
        {
            if( shape->fill.type == NSVG_PAINT_COLOR )
            {
                dc->setFillColor( svgColorToCColor( shape->fill.color, shape->opacity ) );
                
                std::cout << "Filling path with " << std::hex << shape->fill.color << std::dec << std::endl;
                VSTGUI::CDrawContext::PathDrawMode pdm = VSTGUI::CDrawContext::kPathFilled;
                if( shape->fillRule == NSVGfillRule::NSVG_FILLRULE_EVENODD )
                {
                    pdm = VSTGUI::CDrawContext::kPathFilledEvenOdd;
                }
                //std::cout << "PDM is " << (int)pdm << std::endl;
                dc->drawGraphicsPath(gp, pdm);
            }
            else if( shape->fill.type == NSVG_PAINT_LINEAR_GRADIENT )
            {
                bool evenOdd = (shape->fillRule == NSVGfillRule::NSVG_FILLRULE_EVENODD);
                NSVGgradient *ngrad = shape->fill.gradient;
                std::cout << "Shape ID=" << shape->id << " FT=" << (int)shape->fill.type << " ST=" << (int)shape->stroke.type << std::endl;
                std::cout << "Gradient NPoints = " << ngrad->nstops << " FX=" << ngrad->fx << " FY=" << ngrad->fy << " spread=" << (int)ngrad->spread << std::endl;
                std::cout << "   xform = ";
                for( int i=0; i<6; ++i ) std::cout << ngrad->xform[ i ] << " ";
                std::cout << std::endl;
                
                float *x = ngrad->xform;
                VSTGUI::CGraphicsTransform gradXform( x[0], x[1], x[2], x[3], x[4], x[5] );
                VSTGUI::CGradient::ColorStopMap csm;
                VSTGUI::CGradient *cg = VSTGUI::CGradient::create(csm);
                
                for( int i=0; i<ngrad->nstops; ++i )
                {
                    auto stop = ngrad->stops[ i ];
                    std::cout << "   " << std::setw(8) <<  stop.offset << " " << std::hex << stop.color << std::dec << std::endl;
                    cg->addColorStop(stop.offset, svgColorToCColor(stop.color));
                }
                VSTGUI::CPoint s0(0,0), s1(0,1);
                VSTGUI::CPoint p0 =    gradXform.inverse().transform( s0 );
                VSTGUI::CPoint p1 =    gradXform.inverse().transform( s1 );
                
                dc->fillLinearGradient(gp, *cg,
                                       p0, p1,
                                       evenOdd );
            }
            else
            {
                std::cerr << "No radial gradient support yet" << std::endl;
                dc->setFillColor( VSTGUI::kRedCColor );
            }
            
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
            VSTGUI::CLineStyle cs((VSTGUI::CLineStyle::LineCap)(shape->strokeLineCap),
                                  (VSTGUI::CLineStyle::LineJoin)(shape->strokeLineJoin));
            dc->setLineStyle(cs);
            dc->drawGraphicsPath(gp, VSTGUI::CDrawContext::kPathStroked );
        }
        gp->forget();

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
VSTGUI::CColor CSvgDisplay::svgColorToCColor(int svgColor, float opacity)
{
    int a = ( ( svgColor & 0xFF000000 ) >> 24 ) * opacity;
    int b = ( svgColor & 0x00FF0000 ) >> 16;
    int g = ( svgColor & 0x0000FF00 ) >> 8;
    int r = ( svgColor & 0x000000FF );
    return VSTGUI::CColor( r, g, b, a );
}
