# Construction of Wavelet trees using RRR structure

Project for Bioinformatics course (2016/2017) at University of Zagreb, Faculty of Electrical Engineering and Computing, Croatia.

Authors: Mislav Magerl, Matija Milišić, Mateo Šimonović.

## Repository structure

In the `articles` folder you can find some links that describe the structures, documentation from students in the same course 2015/2016, and other course related PDFs.

In the `documentation` folder you can find LaTeX source code for the documentation of this project in Croatian. The root folder contains compiled documentation in PDF.

In the `code` folder you can find all the souce code for this project.

## Code organization

`include` folder contains header files for Wavelet Tree, RRR structure and Brute force implementation of Wavelet Tree.

`src` folder contains implementations of Wavelet Tree and RRR structure.

`other` folder contains implementations of the structures from students in the same course 2015/2016. This is used to compare results of time and memory complexity.

Other folders are used to generate and prepare some datasets and to test time and memory usage.

## Testing

Testing framework used in the project is `googletest`. Tests are written in `unittest` folder. To run the tests you first have to make google test:

1. `git clone https://github.com/google/googletest`
2. `cd googletest`
3. `cmake googletest && make`
4. `cp libgtest.a <PROJECT>/code/bin/ubuntu`

To run tests run the script `bin/ubuntu/run_tests.sh`.

## References

References can be found in bibtex format in `documentation/literature.bib`.
