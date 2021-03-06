#include <iostream>
#include <iomanip>

#if MAC
#include "cocoa_minimal_main.h"
#include "CSvgDisplay.h"
#include "dirent.h"
#endif

#include "vstgui/lib/cframe.h"
#include "vstgui/lib/controls/ctextlabel.h"

#include <sstream>
#include <vector>

#if WINDOWS
#include <windows.h>
#include "win32minimal.h"
#endif


class ShowClickLabel : public VSTGUI::CTextLabel {
public:
    ShowClickLabel( VSTGUI::CRect pos ) : CTextLabel(pos) {
    }

    virtual VSTGUI::CMouseEventResult onMouseDown(
        VSTGUI::CPoint &where,
        const VSTGUI::CButtonState &buttonState
        ) override {
        std::cout << "ONMOUSEDoOWN" << std::endl;
        std::ostringstream ss;
        ss << "BtSt=" << std::hex << buttonState();
        setText(ss.str().c_str());
        invalid();
        std::cout << "SET TEXT TO " << ss.str() << std::endl;
        return VSTGUI::CTextLabel::onMouseDown(where, buttonState);
    }
};

void simpleFrameCB( VSTGUI::CFrame *f )
{
    f->setBackgroundColor( VSTGUI::CColor( 200, 200, 210 ) );
    std::cout << "SimpleFrameCB" << std::endl;
    std::vector<std::string> labs = { "RMB", "Center", "Left", "Side L", "Side R" };
    for( int i=0; i<5; ++i )
    {
        std::cout << "Adding " << i << " th text" << std::endl;
        VSTGUI::CRect pos = VSTGUI::CRect( VSTGUI::CPoint( 20 , 20 + i * 50  ),
                                           VSTGUI::CPoint( 200, 45 ) );
        ShowClickLabel *l = new ShowClickLabel( pos );

        std::ostringstream ss;
        ss << "Click with Button " << labs[i];
        l->setText( ss.str().c_str() );
        l->setFontColor( VSTGUI::kRedCColor );
        l->setBackColor( VSTGUI::kWhiteCColor );
        l->setFrameColor( VSTGUI::kBlueCColor );
        f->addView( l, pos );
    }
}

#if MAC
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
            VSTGUI::CRect pos( VSTGUI::CPoint( 0, 0 ), VSTGUI::CPoint( sz.getWidth()/2, 20 ) );
            l = new VSTGUI::CTextLabel(pos);
            l->setText( "SVG Test" );
            l->setBackColor( VSTGUI::kWhiteCColor );
            l->setFontColor( VSTGUI::kBlueCColor );
            l->setFrameColor( VSTGUI::kBlueCColor );
            f->addView( l, pos );

            VSTGUI::CRect iPos( VSTGUI::CPoint( sz.getWidth()/2, 0), VSTGUI::CPoint( sz.getWidth()/2,20) );
            VSTGUI::CTextLabel *ilab = new VSTGUI::CTextLabel(iPos);
            ilab->setText( "Commands: n/p next and prev; 0 biggest; 1-4 1x-4x zoom" );
            ilab->setBackColor( VSTGUI::kWhiteCColor );
            ilab->setFontColor( VSTGUI::kBlueCColor );
            ilab->setFrameColor( VSTGUI::kBlueCColor );
            f->addView( ilab, pos );

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
        std::cout << "Picking file " << std::dec << filePos << " -> '" << files[filePos] << "'" << std::endl;
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
            if( filePos == 0 ) filePos = files.size() - 1; else filePos--;
            pickFile(filePos);
            return 1;
            break;
        case '0':
            svgDisplay->setZoomCmd(0);
            svgDisplay->invalid();
            return 1;
            break;
        case '1':
            svgDisplay->setZoomCmd(1);
            svgDisplay->invalid();
            return 1;
            break;
        case '2':
            svgDisplay->setZoomCmd(2);
            svgDisplay->invalid();
            return 1;
            break;
        case '3':
            svgDisplay->setZoomCmd(3);
            svgDisplay->invalid();
            return 1;
            break;
        case '4':
            svgDisplay->setZoomCmd(4);
            svgDisplay->invalid();
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
#endif

#if MAC
int main( int argc, char **argv )
{
    // cocoa_minimal_main([](VSTGUI::CFrame *f) { std::cout << "Got a frame with address " << f << std::endl; } );
    // cocoa_minimal_main(1100, 600, simpleFrameCB);
   cocoa_minimal_main(1100, 1100, simpleFrameCB);
}
#endif

#if WINDOWS
void *hInstance;

int WINAPI wWinMain(HINSTANCE _hInstance, HINSTANCE hPrevInstance,
		    PWSTR pCmdLine, int nCmdShow)
{
  hInstance = _hInstance;
  return win32minimal_main(_hInstance, 1100, 1100, nCmdShow, simpleFrameCB );
}
#endif
 
