build:
	premake5 xcode4
	xcodebuild build -configuration Release -project vstgui-tests.xcodeproj

clean:
	xcodebuild clean -configuration Release -project vstgui-tests.xcodeproj

clean-all:	clean
	rm -rf obj target VT.xcworkspace vstgui-tests.xcodeproj
