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

buildoptions
{
   "-std=c++17",
   "-stdlib=libc++",
   "-ferror-limit=0",
}

files
{
   "src/main.cpp",
}

links
{
   "CoreFoundation.framework"
}
