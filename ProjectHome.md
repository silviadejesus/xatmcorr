This program aims to be a simple interface for mass atmospheric correction for files downloaded from Brazilian National Institute for Space Research (INPE).

Please report any bugs found in [Issues](http://code.google.com/p/xatmcorr/issues/list)

Version 2.6 will be frozen as the last stable version that uses 8 bits images.
All versions after 2.6 are now using floating point images, so if storage size is a problem, you may want to use version 2.6.

Update 13/03/2012:
Now the program has it's own atmospheric correction implementation, imported from [GRASS GIS](http://grass.fbk.eu). Therefore, the images now use floating point improving accuracy.
One new extension was added: An experimental research on water bodies reflectance correction.