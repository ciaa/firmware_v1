Pthreads-Win32
~~~~~~~~~~~~~~

See file README what this is and where is has been downloaded from.

Pthreads-Win32 is used in IsoAgLib at least for
  tools/can_server
.

The resulting DLL bin/win32/pthreadVC2.dll has been built by using
pthread.dsp, after having replaced the /MD compile flag by /MT.
(Otherwise MS Visual Studio needs to be installed to be able to start
the CAN server and all other executables relying on pthreadVC2.dll.)
