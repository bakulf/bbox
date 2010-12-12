qmake -r -spec win32-msvc2010 CONFIG+=release CONFIG+=embed_manifest_exe
nmake
copy src\release\bbox.exe win32\package
copy e:\Qt\4.7.1\bin\QtCore4.dll win32\package
copy e:\Qt\4.7.1\bin\QtGui4.dll win32\package
