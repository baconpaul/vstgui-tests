build:
	premake5 xcode4
	xcodebuild build -configuration Release -project vstgui-tests.xcodeproj
	cp resources/Info.plist target/Release/VstguiTests.app/Contents/

clean:
	xcodebuild clean -configuration Release -project vstgui-tests.xcodeproj

clean-all:	clean
	rm -rf obj target VT.xcworkspace vstgui-tests.xcodeproj

run:	build
	./target/Release/VstguiTests.app/Contents/MacOS/VstguiTests 

dist:	build
	rm -rf product_macos
	mkdir product_macos
	rm -rf target/Release/*.dSYM
	cd target/Release && hdiutil create /tmp/tmp.dmg -ov -volname "VstGui Test" -fs HFS+ -srcfolder "."
	cd ../..
	hdiutil convert /tmp/tmp.dmg -format UDZO -o "product_macos/VstguiTests.dmg"

