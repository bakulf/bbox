BBox
====

...something like dropbox based on subversion.

The concept
-----------

DropBox is quite easy to implement using a subversion repository. This code is
just a multiplatform client that keeps some local folders updated and commits your changes.

Here, the list of main features:

* Systray icon and menu with notifications of changes from remote users
* Wizard for configuration and first checkout of the subversion repository
* AutoCommit can be activated
* List of logs and list of changes are nicely visible
* Manage conflicts
* FileSystem watcher for changes and commit notification


How to install/compile it
-------------------------

This application is based on Qt4. I have compiled BBox on GNU/Linux, Microsoft Windows and Mac OS X 10.5.x and 10.6.x.

### Cloning the repo

It might be obvious but you need to clone this git repository in order to get sources compiled ;)

So open a Terminal (or use your favorite git GUI) and type:

    (user)$ git clone git://github.com/bakulf/bbox.git

There you go, you're done.
Then go in the right folder:

    (user)$ cd ./bbox

now we can go on and compile.

### Linux (ubuntu/debian)

    (root)# apt-get install libqt4-dev make g++ qt4-make

then, this command should be enough:

    (user)$ qmake
    (user)$ make
    (root)# make install

### Windows

I use Microsoft [Visual Studio Express 2010][msvc] and then [Qt4][qt] for visual studio 2008.
Then, I load some env variables using this script:

    (cmd)$ vcvars32.bat

This script is under 'Microsoft Visual Studio 10.0\VC\bin' folder. Then,

    (cmd)$ qmake -r -spec win32-msvc2010 CONFIG+=release
    (cmd)$ nmake

then, the binary should be here: .\src\release\bbox.exe

But don't worry: I will provide a BBoxInstaller.exe quite soon.

### MacOSX

You need to [Download Qt Creator][qtCmac] and the [Qt SDK][qtMac] in order to compile the binaries. 
Then open the project in ./bbox.pro in Qt Creator Application you just downloaded, and click build.
You'll have a new folder called src-build-desktop.
Inside there you'll have a bbox.app Mac OS X bundle.

Right after that if you want to package the software in order to distribute it, you'll need to open a Terminal tab or window and type

    (user)$ cd /path/to/git/bbox/src-build-desktop
    (user)$ macdeployqt bbox.app

What this command does, is basically include Qt frameworks needed for the application to run, so that those who don't have Qt SDK installed can run this too.
Now you can both install and distribuite the build. 


How to install the server
-------------------------

The Web is full of excellent tutorials about how to install SVN (subversion) + Apache + mod_dav.
I don't want to write here how to do this step. But soon-ish I will provide a couple of good URLs
about this procedure.


Contributing
------------

1. Fork it.
2. Create a branch (`git checkout -b my_bbox`)
3. Commit your changes (`git commit -am "Added Snarkdown"`)
4. Push to the branch (`git push origin my_bbox`)
5. Create an [Issue][1] with a link to your branch


[msvc]: http://www.microsoft.com/express/Windows/
[qt]: http://qt.nokia.com/downloads
[qtCmac]: http://qt.nokia.com/downloads/qt-creator-binary-for-mac
[qtMac]: http://qt.nokia.com/downloads/sdk-mac-os-cpp
