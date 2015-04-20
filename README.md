# LintPlug
PC Lint plugin for QtCreator
Please adjust QtCreator sources directory in .pro file.

I didn't find a reliable way to get active project sources if different configurations are present in .pro file.
There is also no way to get directory where MOC files are generated.
Therefore all includes will be checked in project check mode moc files should be in build directory.

Because output is sorted, there is no sense to have messages with code 830 and 831. Just suppress them in startup options
under Tools/Options/Analizer/PC Lint:

-e830  		// Location cited in prior message
-e831		// Reference cited in prior message

I used example.lnt as command line argument for my tests. There are some screenshots under /screenshots directory.
