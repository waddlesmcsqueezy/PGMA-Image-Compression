# PGMA-Image-Compression

This project was to test the effects of quad tree traversal compression on PGMA images which contain a space separated value list of gray levels, therefore they are easy to directly manipulate via C.

                        Image	Size before LZ compression	              Size after LZ compression
  Original Baboon.pgma	            1230kb	                                      287kb
  Output @ 0 variance	              1272kb	                                      287kb
  Output @ 16 variance	            1272kb	                                      283kb
  Output @ 64 variance	            1273kb	                                      252kb
  Output @ 256 variance	            1272kb	                                      192kb


Interestingly, the size before we use LZ compression on the quad tree processed image, they are bigger by about 40kb across all levels of compression (probably comes down to inefficiency of the program’s use of whitespace), but after LZ compression, the higher compression level in the program, the smaller the file size. This is likely due to LZ compression making use of duplicate data in the file like 2 pixels being the same color etc.
