# global defines used by the Makefiles

# C++ compiler
CPP     = g++

# compile static libraries
CL      = ar rcs

# locally-defined open-MPI C++ wrapper compiler
MPI_CPP = $(HOME)/local/openmpi/v3.0.0/bin/mpic++

# include path to open MPI header files
MPI_INC = /$(HOME)/local/openmpi/v3.0.0/include/
