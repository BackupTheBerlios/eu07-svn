NOTES:

The project is developed in Microsoft Visual Studio C++ 6.0, but compiles fine with .NET as
well. However, please consult:
	http://sparks.discreet.com/knowledgebase/techdocs/searchable/Techdoc_3dsmaxVC7/3dsmax_VC7.htm
In short you need to add an empty file ctl3d.h to maxsdk/include - memory errors can occur,
but we haven't seen it though;-)

INSTALL:

If you already have a working OSG compiled with STLPort you can go
directly to step 8. Just remember to tell Microsoft Visual Studio
c++ 6.0 where to find STLport include, OSG include and OSG library
files (see step 4 on how to do this).

1. Download OpenSceneGraph from
  http://sourceforge.net/project/showfiles.php?group_id=11278&release_id=188165
and extract to D:\ 

2. If you are using MSVC60 download stlport from
  http://www.stlport.org/archive/STLport-4.5.3.zip
and extract to D:\

3. If you are using MSVC60 tell stlport not to use its own iostream. Open 
D:\STLport-4.5.3\stlport\stl_user_config.h and change line 45
from:
//  # define _STLP_NO_OWN_IOSTREAMS 1 
to:
  # define _STLP_NO_OWN_IOSTREAMS 1 

4. Open Microsoft Visual C++ 6.0(similar on .NET), go to tools->options and select
the directories tab. Select Includes files from drop down list and
enter the following directories:
  D:\STLPORT-4.5.3\STLPORT (only MSVC60)
  D:\OpenThreads\include
  D:\Producer\include
  D:\OpenSceneGraph\include
Move them all the way up before std. directories.
Now select the Library files from the drop down list and
enter the following directories:
  D:\OpenThreads\lib\win32
  D:\Producer\lib
  D:\OpenSceneGraph\lib
Again, move them all the way up before std. directories.

5. Open OpenThreads.dsw and build all

6. Open Producer.dsw and build all

7. Open OpenSceneGraph.dsw and build all

8. Install 3ds max 5.0 and remember to include the maxsdk.

9. Open Microsoft Visual Studio C++ 6.0/NET, go to tools -> options and
select the directories tab.

Select the Include files from the drop down list and indicate where
the maxdsk include directory resides on your system:
  3dsmax5\maxsdk\include
Move the directory up before the std. directories.

Select the Library files from the drop down box and indicate where
the maxdsk lib directory resides on your system:
  3dsmax5\maxsdk\lib
Move all the directory up before the std. directories.

10. Use the CVS or download the OpenSceneGraph Exporter for 3ds max from:
  http://OSGExp.vr-c.dk/download/OSGExp.zip
and extract to D:\

11. Open OSGExp.dsw and build all. The Hybrid version is the same
as a Debug version. It enables you to debug the plugin while 3ds max
is running.

12. If you get a compile error in OSGExp.cpp at line 244 from
INT_PTR you should do the following:

  On or around line 123 of VC98\include\basetsd.h change:
	typedef long INT_PTR, *PINT_PTR;
	typedef unsigned long UINT_PTR, *PUINT_PTR;
  to
	typedef int INT_PTR, *PINT_PTR;
	typedef unsigned int UINT_PTR, *PUINT_PTR;

The reason for this error, is that the 3ds max team have
compiled everything up against the Microsoft Platform SDK.
The default VC98 comes with an error definition 
and the Microsoft Platform SDK fixes this. The bad thing
about Microsoft Platform SDK is that it conflicts
with STLPort, Openthreads and Producer and have a size
off over 300 megs when downloading it! So the current
solution is simply to change the definition in 
VC98\include\basetsd.h. Not a pretty solution, but it
works.

13. Add the following directories to the path environment:
  D:\OpenThreads\bin\win32
  D:\Producer\bin
  D:\OpenSceneGraph\bin
  D:\OSGExp\bin
  
14. Open 3dsmax5\plugin.ini and add the following line
under the Directories tag:
  OSG export=D:\OSGExp\bin

15. Open 3dsmax5 and export using File->Export or
File->Export Selected, choose "OpenSceneGraph Exporter (*.ive,*osg)"
as file type and off you go!


See
  http://osgexp.vr-c.dk
for information on using the plugin.

Have fun :-)


Rune Schmidt Jensen, 01.10.2003
Michael Grønager, 02.10.2003
UNI-C