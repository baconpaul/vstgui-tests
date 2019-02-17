#include <iostream>
#include <iomanip>

#include "cocoa_minimal_main.h"
#include "CSvgDisplay.h"

#include "vstgui/lib/cframe.h"
#include "vstgui/lib/controls/ctextlabel.h"

#include <sstream>
#include <vector>
#include "dirent.h"

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

struct SvgBrowser : public VSTGUI::IKeyboardHook
{
    SvgBrowser(VSTGUI::CFrame *f) : frame(f)
    {
        f->setBackgroundColor( VSTGUI::CColor( 200, 200, 210 ) );
        std::cout << "SimpleFrameCB" << std::endl;

        DIR *dir;
        struct dirent *dp;
        char * file_name;
        dir = opendir("resources/svg");
        while ((dp=readdir(dir)) != NULL) {
            if( strstr( dp->d_name, ".svg" ) )
            {
                file_name = dp->d_name; // use it
                files.push_back( std::string( "resources/svg/" ) + file_name );
            }
        }
        closedir(dir);

        std::sort(files.begin(), files.end(), [](std::string &a, std::string &b) { return strcmp( a.c_str(), b.c_str() ) < 0; } );
        
        VSTGUI::CRect sz;
        f->getSize(sz);
        
        {
            VSTGUI::CRect pos( VSTGUI::CPoint( 0, 0 ), VSTGUI::CPoint( sz.getWidth(), 20 ) );
            l = new VSTGUI::CTextLabel(pos);
            l->setText( "SVG Test" );
            l->setBackColor( VSTGUI::kWhiteCColor );
            l->setFontColor( VSTGUI::kBlueCColor );
            l->setFrameColor( VSTGUI::kBlueCColor );
            f->addView( l, pos );
        }
        
        {
            VSTGUI::CRect pos( VSTGUI::CPoint( 10, 30 ), VSTGUI::CPoint( sz.getWidth() - 20, sz.getHeight() - 40 ) );
            //CSvgDisplay *d = new CSvgDisplay( pos, "resources/svg/drawing.svg" );
            //CSvgDisplay *d = new CSvgDisplay( pos, "resources/svg/daves-123.svg" );
            svgDisplay = new CSvgDisplay( pos, files[ 0 ] );
            filePos = 0;
            f->addView( svgDisplay, pos );
        }
        std::cout << "REGISTERING HOOK" << std::endl;
        f->registerKeyboardHook( this );
        pickFile( 0 );
    }

    void pickFile( int filePos ) {
        svgDisplay->resetFile(files[filePos]);
        svgDisplay->invalid();
        l->setText(files[filePos].c_str());
        l->setDirty(true);
        frame->invalid();
    }
    virtual int32_t onKeyDown(const VstKeyCode &code, VSTGUI::CFrame *f) override {
        std::cout << "onKeyDown " << code.character << std::endl;
        switch( code.character )
        {
        case 'n':
            std::cout << "Next image please" << std::endl;
            filePos ++; if( filePos >= files.size() ) filePos = 0;
            pickFile(filePos);
            return 1;
            break;
        case 'p':
            std::cout << "Prior image please" << std::endl;
            filePos --; if( filePos <  0 ) filePos = files.size() - 1;
            pickFile(filePos);
            return 1;
            break;
        }
        return -1;
    }
    virtual int32_t onKeyUp( const VstKeyCode &code, VSTGUI::CFrame *f) override {
        return -1;
    }
    
    VSTGUI::CFrame *frame;
    CSvgDisplay *svgDisplay;
    VSTGUI::CTextLabel *l;
    std::vector<std::string> files;
    size_t filePos;
};

void svgExampleCB( VSTGUI::CFrame *f )
{
    new SvgBrowser(f); // yeah I leak it but look this is just demo code, OK?
    
}

int main( int argc, char **argv )
{
    // cocoa_minimal_main([](VSTGUI::CFrame *f) { std::cout << "Got a frame with address " << f << std::endl; } );
    // cocoa_minimal_main(1100, 600, simpleFrameCB);
    cocoa_minimal_main(1100, 1100, svgExampleCB);
}
