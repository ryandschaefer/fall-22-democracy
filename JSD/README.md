# Democracy Lab - JSD
## By: Ryan Schaefer

This folder contains the code for the custom JSD function I built in C++. I then used Rcpp to make a package in R that contains this function. To install this package, clone this repository, set your working directory in R to this folder, and run the following command in R:

```install.packages("jsdPackage_1.0.tar.gz", type = "source", repos = NULL)```

Use ```?jsd``` to view the help page for this function to see how to use it.

The file ```JSD.cpp``` contains the Rcpp code used to create the package (minus some documentation comments). 
The file ```LogLikelihood_1.0.tar.gz``` is the executable file containing the R package. Run the previously given command to install it. The ```jsdPackage``` subfolder contains the source code for the installable package. This will always contain the most recent version of the code.

If you get an error trying to run jsd or displaying the documentation, try running ```.rs.restartR()``` and trying again.
