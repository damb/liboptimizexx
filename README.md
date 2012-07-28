liboptimizexx
=============

C++ library for global search and optimization algorithms

liboptimizexx provides global search and optimization algorithms working with
any application on a discrete parameter space. A global algorithm mainly is
responsible for sending an application through a parameter space grid while
an application must be made available by inheritance of class
optimize::ParameterSpaceVisitor. 
If using a global algorithm both the previously created parameter space builder
and the parameters need to be passed. It then will care about the generation of
its parameter space and executing the application.
While an application actually acts as a forward algorithm which takes the
responsibility for the calculation the parameter space on the other hand is
described with a grid consisting of both subgrids and nodes of which the latter
will store any result data type.

After finishing the calculations the results are accessible using iterators
which provide the possibility of iterating over the entire parameter space in
different manners.

Up to now only the following global algorithms are provided:
 - Grid Search
 - Montecarlo

liboptimizexx, written in C++ (standard C++0x), strongly uses design patterns.
Additionally it provides parallel computing but unfortunately while starting the
development current compilers didn't support fully thread facilities in C++0x,
so there still exists the dependency to the Boost Thread library.

Installing the library is done using the provided Makefile while a doxygen
documentation is existing, too.
