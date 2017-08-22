# PBSImageToolbox
The library should work on Linux as well as on Windows.

 

This library should have the following functions in the first place:

 

ImageInfo:

    # of pages for multipage images (Tiff, PNG?, JPG?)
    Thumbnail resolution (0 if not present)
    # of layers in File
    Mask present (Y/N)
    Color depth
    Color space
    Resolution (main image)
    Filetype (sometimes the file extension is *.JPG but the file in fact is PNG and similar cases)
    EXIF information (all available tags)

Image conversion:

    TIFF >> PNG (preserving mask)
    TIFF >> EPS (preserving mask)
    EPS >> PNG (preserving mask)
