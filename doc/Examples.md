  <refmeta>
    <refentrytitle>Examples</refentrytitle>
    <manvolnum>3</manvolnum>
    <refmiscinfo>libvips</refmiscinfo>
  </refmeta>

  <refnamediv>
    <refname>libvips examples</refname>
    <refpurpose>A few example Python programs using libvips</refpurpose>
  </refnamediv>

This page shows a few libvips examples using Python. They will work with
small syntax changes in any language with a libvips binding.

The libvips test suite is written in Python and exercises every operation
in the API.  It's also a useful source of examples.

# Average a region of interest box on an image

```python
#!/usr/bin/python3

import sys
import pyvips

left = 10
top = 10
width = 64
height = 64

image = pyvips.Image.new_from_file(sys.argv[1])
roi = image.crop(left, top, width, height)
print('average:', roi.avg())
```

# libvips and numpy

You can use `pyvips.Image.new_from_memory()` to make a vips image from
an area of memory. The memory array needs to be laid out band-interleaved,
as a set of scanlines, with no padding between lines.

```python
#!/usr/bin/python3

import sys
import time

import pyvips
from PIL import Image
import numpy as np

if len(sys.argv) != 3:
    print(f'usage: {sys.argv[0]} input-filename output-filename')
    sys.exit(-1)

# map vips formats to np dtypes
format_to_dtype = {
    'uchar': np.uint8,
    'char': np.int8,
    'ushort': np.uint16,
    'short': np.int16,
    'uint': np.uint32,
    'int': np.int32,
    'float': np.float32,
    'double': np.float64,
    'complex': np.complex64,
    'dpcomplex': np.complex128,
}

# map np dtypes to vips
dtype_to_format = {
    'uint8': 'uchar',
    'int8': 'char',
    'uint16': 'ushort',
    'int16': 'short',
    'uint32': 'uint',
    'int32': 'int',
    'float32': 'float',
    'float64': 'double',
    'complex64': 'complex',
    'complex128': 'dpcomplex',
}

# load with PIL
start_pillow = time.time()
pillow_img = np.asarray(Image.open(sys.argv[1]))
print('Pillow Time:', time.time()-start_pillow)
print('original shape', pillow_img.shape)

# load with vips to a memory array
start_vips = time.time()
img = pyvips.Image.new_from_file(sys.argv[1], access='sequential')
mem_img = img.write_to_memory()

# then make a numpy array from that buffer object
np_3d = np.ndarray(buffer=mem_img,
                   dtype=format_to_dtype[img.format],
                   shape=[img.height, img.width, img.bands])

print('Vips Time:', time.time()-start_vips)
print('final shape', np_3d.shape)

# verify we have the same result
print('Sum of the Differences:', np.sum(np_3d-pillow_img))

# make a vips image from the numpy array
height, width, bands = np_3d.shape
linear = np_3d.reshape(width * height * bands)
vi = pyvips.Image.new_from_memory(linear.data, width, height, bands,
                                  dtype_to_format[str(np_3d.dtype)])

# and write back to disc for checking
vi.write_to_file(sys.argv[2])
```

# Build huge image mosaic

This makes a 100,000 x 100,000 black image, then inserts all the images you
pass on the command-line into it at random positions. libvips is able to run
this program in sequential mode: it'll open all the input images at the same
time, and stream pixels from them as it needs them to generate the output.

To test it, first make a large 1-bit image. This command will take the
green channel and write as a 1-bit fax image. `wtc.jpg` is a test 10,000
x 10,000 jpeg:

```bash
$ vips extract_band wtc.jpg x.tif[squash,compression=ccittfax4,strip] 1
```

Now make 1,000 copies of that image in a subdirectory:

```bash
$ mkdir test
$ for i in {1..1000}; do cp x.tif test/$i.tif; done
```

And run this Python program on them:

```bash
$ time python try255.py x.tif[squash,compression=ccittfax4,strip,bigtiff] test/*
real	1m59.924s
user	4m5.388s
sys	0m8.936s
```

It completes in just under two minutes on this laptop, and needs about
7gb of RAM to run. It would need about the same amount of memory for a
full-colour RGB image, I was just keen to keep disc usage down. 

If you wanted to handle transparency, or if you wanted mixed CMYK and RGB
images, you'd need to do some more work to convert them all into the same
colourspace before inserting them.

```python
#!/usr/bin/python3
#file try255.py

import sys
import random
import pyvips

# this makes a 8-bit, mono image of 100,000 x 100,000 pixels, each pixel zero
im = pyvips.Image.black(100000, 100000)

for filename in sys.argv[2:]:
    tile = pyvips.Image.new_from_file(filename, access='sequential')

    im = im.insert(tile,
                   random.randint(0, im.width - tile.width),
                   random.randint(0, im.height - tile.height))

im.write_to_file(sys.argv[1])
```
