#include <iostream>
#include <iomanip>

#include "cocoa_minimal_main.h"

#include "vstgui/lib/cframe.h"
#include "vstgui/lib/controls/ctextlabel.h"

#include <sstream>

void simpleFrameCB( VSTGUI::CFrame *f )
{
    f->setBackgroundColor( VSTGUI::CColor( 200, 200, 210 ) );
    std::cout << "SimpleFrameCB" << std::endl;
    for( int i=0; i<3; ++i )
    {
        std::cout << "Adding " << i << " th text" << std::endl;
        VSTGUI::CRect pos = VSTGUI::CRect( VSTGUI::CPoint( 20 + i * 100 , 20 + i * 10  ),
                                           VSTGUI::CPoint( 80, 30 ) );
        VSTGUI::CTextLabel *l = new VSTGUI::CTextLabel( pos );

        std::ostringstream ss;
        ss << "Label " << i;
        l->setText( ss.str().c_str() );
        l->setFontColor( VSTGUI::kRedCColor );
        l->setBackColor( VSTGUI::kWhiteCColor );
        l->setFrameColor( VSTGUI::kBlueCColor );
        f->addView( l, pos );
    }
}

int main( int argc, char **argv )
{
    // cocoa_minimal_main([](VSTGUI::CFrame *f) { std::cout << "Got a frame with address " << f << std::endl; } );
    cocoa_minimal_main(simpleFrameCB);
}
