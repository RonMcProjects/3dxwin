# 3D cosine plot

This graphic was originally from a TRS-80 COCO magazine, either Color Computer or Rainbow.

It plots the graph for cos(x) + 0.4cos(3x), from -1.5PI to +1.5PI, rotated in 3D and viewed from above at an angle.

At the time it took 1 hour and 40 minutes to plot out on the COCO 1.  On an APPLE-2 using a basic compiler (to eliminate the interpreter slowness) it ran in 40 minutes.  

I then ported it to Turbo-C.  On a 286: 8 minutes.  On a 386: 2 minutes.  On a 486: 15 seconds.  On a Pentium: ~3 seconds.

After that it drew faster than the eye could see.  I ported it to Solaris Unix X11 in the early nineties and that's close to the version you see here.  I had to recover it from an old hard drive connected to a USB adapter.

This version builds on Linux and with only a few compiler warning corrections is unchanged from the Solaris version.  A testament to the longevity of that graphic system.

