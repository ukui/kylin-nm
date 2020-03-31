# NAME
    kylin-nm - kylin network monitor used in ubuntu-kylin operation system
    
# DESCRIPTION
    kylin-nm is a Qt based applet and uses some interface provided by NetworkManager. 
    It provides a GUI for users to connect or disconnect wired or wireless network which managed by NetworkManager. 
    Users can also create new wired network and configure a old network. 
    By click button at left bottom in the main window, a network configure window of NetworkManager will show in the screen.
    Users can get some information about network directly by clicking one item in the network list, these information shown in extention area.

# BUILD KYLIN-NM
    down the source sode
    install dependency packages(see cntrol files in the debian directory) 
    execute debuild command in the root directory of project
    execute sudo dpkg -i packagename.deb to install
