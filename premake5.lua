solution "VT"
configurations { "Debug", "Release" }
language "C++"

floatingpoint "Fast"

configuration { "Debug" }
defines { "DEBUG=1", "RELEASE=0" }
symbols "On"
targetdir "target/Debug"

configuration { "Release*" }
defines { "DEBUG=0", "RELEASE=1" }
optimize "Speed"
targetdir "target/Release"

configuration {}

project "vstgui-tests"
kind "WindowedApp"
targetname "VstguiTests"

flags
{
   "StaticRuntime"
}

includedirs
{
   "deps/vstgui.surge",
   "deps/nanosvg/src",
   "src"
}

defines
{
   "MAC=1"
}

buildoptions
{
   "-std=c++17",
   "-stdlib=libc++",
   "-ferror-limit=0",
}

files
{
   "src/main.cpp",
   "src/cocoa_minimal_main.mm",
   "src/nanosvg_link.cpp",
   "src/CSvgDisplay.cpp",
   
   "deps/vstgui.surge/vstgui/vstgui_mac.mm",
   "deps/vstgui.surge/vstgui/vstgui_uidescription_mac.mm",
}

links
{
   "Accelerate.framework",
   "ApplicationServices.framework",
   "AudioUnit.framework",
   "Carbon.framework",
   "CoreAudio.framework",
   "CoreAudioKit.framework",
   "CoreServices.framework",
   "CoreText.framework",
   "Cocoa.framework",
   "CoreFoundation.framework",
   "OpenGL.framework",
   "QuartzCore.framework",
}
