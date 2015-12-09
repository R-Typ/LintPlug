# LintPlug
PC Lint plugin for Qt Creator

## Compiling LintPlug

Prerequisites:

* Qt library in the same version as used by target Qt Creator (Qt 5.5.1 for QtCreator 3.5.1)
The installed toolchain have to match the one Qt was compiled with.

## Builds for lates QtCreator releases

	1.	Create a working directory under which to check out Qt Creator.
	
	2.	In the working directory, check out the respective branch of Qt Creator (master branch or latest version, see
       <https://code.qt.io/cgit/qt-creator/qt-creator.git>).

	3.	Check out LintPlug from https://github.com/R-Typ/LintPlug.git
	
	4.	Build Qt Creator. See https://github.com/qtproject/qt-creator for step by step instructions.
	
	5.	Set environment variable QTC_SOURCE to point to Qt Creator directory from step 2 and QTC_BUILD to point to Qt Creator build directory from step 4. Alternatively you can set these values directly in lintplug.pro file.
	
	6.	Build release version of LintPlug using QtCreator or from command line. Target LintPlug.dll/so is automatically deployed in lib/qtcreator/plugins of Qt Creator build. 
	   
## Other notes
I didn't find a reliable way to get active project sources if different configurations are present in .pro file.
There is also no way to get directory where MOC files are generated.
Therefore all includes will be checked in project and moc files should be in build directory.

Because output is sorted, there is no sense to have messages with code 830 and 831. Just suppress them in startup options
under Tools/Options/Analizer/PC Lint:

-e830  		// Location cited in prior message
-e831		// Reference cited in prior message

I used example.lnt as command line argument for my tests. There are some screenshots in /screenshots directory.




