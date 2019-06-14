rm Release/fusion
rm fusion
gcc src/*.cpp -I/usr/local/Cellar/libpng/1.6.37/include/libpng16 -L/usr/local/Cellar/libpng/1.6.37/lib -L'.' -lpng16 -framework OpenGL -framework GLUT -arch x86_64 -lfmod -lfmodL -lstdc++ -o fusion
install_name_tool -change @rpath/libfmodL.dylib /Users/lat/Desktop/thing/fusion/Release/libfmodL.dylib fusion
install_name_tool -change @rpath/libfmod.dylib /Users/lat/Desktop/thing/fusion/Release/libfmod.dylib fusion
cp fusion Release/fusion


