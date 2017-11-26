# open MPI C++ compiler
MPI_CPP = $(HOME)/user_libs/openmpi_3p0p0/bin/mpic++

# GNU C++ compiler
CPP     = g++

# compile static libraries
CL      = ar rcs

PROC_INC = -I/$(HOME)/user_libs/openmpi_3p0p0/include
