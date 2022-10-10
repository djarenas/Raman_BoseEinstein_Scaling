# Raman_BoseEinstein_Scaling
Raman scattering spectra --> Out-of-phase ("Imaginary") component of the Raman susceptibility. 

This C++ program takes a file with Raman spectrum (Wavenumber and Raman scattering intensity) and a specified temperature. It first checks that the input file has the right format (2 columns) and that there is no missing data. It then reads each wavenumber and intensity into a customly-declared datapoint structure. The intensity is scaled based on the Bose Einstein (BE) distribution for the temperature and each wavenumber (frequency). The scaled intensity is output to a file. An example input file "Bi25FeO39.txt" is provided. An input file with improper data is also provided "Improper_datafile.txt".

Compilation:
g++  -std=c++11  BoseCorrection.cpp  -o  bc.exe


Execution syntax:
bc.exe < Input_Filename >  < Temperature > < Output_Filename >

Execution example:
bc.exe Bi25FeO39.txt  373  output_file.txt

This code is an updated version of previous code used in the publications:
Arenas, D. J., et al. "Raman spectroscopy evidence of inhomogeneous disorder in the bismuth-oxygen framework of Bi25InO 39 and other sillenites." Physical Review B 86.14 (2012): 144116.
Arenas, D. J., et al. "Raman study of phonon modes in bismuth pyrochlores." Physical Review B 82.21 (2010): 214302.

The BE conversion is common in Raman spectroscopy analysis. Further information can be found in: 
- Sun, Shuo, Dhanusha TN Rathnayake, and Yinsheng Guo. "Asymmetrical Spectral Continuum between Anti-Stokes and Stokes Scattering Revealed in Low-Frequency Surface-Enhanced Raman Spectroscopy." The Journal of Physical Chemistry C 126.27 (2022): 11193-11200.
- Arenas, D. J., et al. "Raman study of phonon modes in bismuth pyrochlores." Physical Review B 82.21 (2010): 214302.
- Choi, K. Y., et al. "Raman scattering study of Nd1− xSrxMnO3 (x= 0.3, 0.5)." Journal of Physics: Condensed Matter 15.19 (2003): 3333.
