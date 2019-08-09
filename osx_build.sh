rm Release/fusion
rm fusion
gcc src/*.cpp -I/usr/local/Cellar/libpng/1.6.37/include/libpng16 -L/usr/local/Cellar/libpng/1.6.37/lib -L'.' -lpng16 -framework OpenGL -framework GLUT -arch x86_64 -lfmod -lfmodL -lstdc++ -framework carbon -o fusion
install_name_tool -change @rpath/libfmodL.dylib @executable_path/libfmodL.dylib fusion
install_name_tool -change @rpath/libfmod.dylib @executable_path/libfmod.dylib fusion
install_name_tool -change /usr/local/opt/libpng/lib/libpng16.16.dylib @executable_path/libpng16.16.dylib fusion
cp fusion Release/fusion


