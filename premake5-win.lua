solution "VT"
configurations { "Debug", "Release" }
platforms {"x64" }
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
   "WINDOWS=1"
}

buildoptions
{
   -- "-std=c++17",
   -- "-stdlib=libc++",
   -- "-ferror-limit=0",
}

files
{
   "src/main.cpp",
   "src/win32minimal.cpp",
   "src/nanosvg_link.cpp",
   "src/CSvgDisplay.cpp",
   
   "deps/vstgui.surge/vstgui/vstgui_win32.cpp",
}

links
{
}
