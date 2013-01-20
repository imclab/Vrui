/***********************************************************************
ImageReader - Abstract base class to read images from files in a variety
of image file formats.
Copyright (c) 2012 Oliver Kreylos

This file is part of the Image Handling Library (Images).

The Image Handling Library is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

The Image Handling Library is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License along
with the Image Handling Library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
***********************************************************************/

#ifndef IMAGES_IMAGEREADER_INCLUDED
#define IMAGES_IMAGEREADER_INCLUDED

#include <IO/File.h>
#include <Images/Image.h>
#include <Images/RGBImage.h>
#include <Images/RGBAImage.h>

namespace Images {

class ImageReader
	{
	/* Embedded classes: */
	public:
	enum ColorSpace // Enumerated type for image color spaces
		{
		Grayscale,
		RGB
		};
	
	struct ImageSpec // Specification for a sub-image inside the image file
		{
		/* Elements: */
		public:
		unsigned int offset[2]; // Position of image inside the canvas
		unsigned int size[2]; // Width and height of image in pixels
		ColorSpace colorSpace; // Color space of the image
		bool hasAlpha; // Flag if the image has an alpha channel
		unsigned int numComponents; // Number of components in image; typically 1 (grayscale), 2 (grayscale+alpha), 3 (RGB), or 4 (RGB+alpha)
		unsigned int bitsPerComponent; // Maximum number of bits per image component
		bool signed; // Flag if the component values are signed integers
		};
	
	protected:
	struct ImagePlane // Structure describing the memory layout of an image component plane
		{
		/* Elements: */
		public:
		const void* basePtr; // Pointer to lower-left pixel
		size_t pixelSize; // Size of a pixel in bytes
		off_t pixelStride; // Pointer stride within an image row
		off_t rowStride; // Pointer stride between image rows
		};
	
	/* Elements: */
	protected:
	IO::FilePtr file; // Handle of the image file
	unsigned int canvasSize[2]; // Size of the image canvas, i.e., the bounding box of all sub-images
	unsigned int numImages; // Number of sub-images in the image file
	ImageSpec* imageSpecs; // Array of sub-image specifications
	
	/* Protected methods: */
	virtual ImagePlane* readSubImagePlanes(void) =0; // Reads an image and returns an array of one image plane descriptor per image component
	virtual void releaseSubImagePlanes(ImagePlane* planes) =0; // Releases the image's planes after copying
	
	/* Constructors and destructors: */
	public:
	ImageReader(IO::FilePtr sFile); // Creates an image reader for the given file
	virtual ~ImageReader(void);
	
	/* Methods: */
	unsigned int* getCanvasSize(void) const // Returns the size of the image canvas
		{
		return canvasSize;
		}
	unsigned int getCanvasSize(int dimension) const // Ditto, for single dimension
		{
		return canvasSize[dimension];
		}
	unsigned int getNumImages(void) const // Returns the number of sub-images in the image file
		{
		return numImages;
		}
	const ImageSpec* getImageSpec(unsigned int imageIndex) const // Returns the specification of the given sub-image
		{
		return imageSpecs[imageIndex];
		}
	Image<GLubyte,1> readGray8(void); // Reads the image as an 8-bit grayscale image
	Image<GLushort,1> readGray16(void); // Reads the image as a 16-bit grayscale image
	Image<GLfloat,1> readGray32(void); // Reads the image as a 32-bit floating-point grayscale image
	RGBImage readRGB8(void); // Reads the image as an 8-bit RGB image
	RGBAImage readRGBA8(void); // Reads the image as an 8-bit RGB with alpha image
	};

}

#endif
