## COMPUTER VISION - LAB 1

**Topics:** Introduction to OpenCV, image loading, pixel manipulation, the RGB and HSV color spaces

**Goal:** Change the soccer shirt color of all the robots in the image

In this lab, you shall perform a **self-study** activity on:

- What is a mouse callback
- What is the HSV color space and conversion between RGB and HSV

1 - Write a program that:

1. Loads the image stored inside the data folder
2. Shows the image on a window
3. Captures the left click of the mouse and computes the mean RGB color over a 9x9 neighborhood
   of the clicked point
4. Use 3. to segment the robots soccer shirt by applying some static threshold to the three
   channels R, G and B
5. When the soccer shirts are at least 30% segmented, apply a new color to them(let’s use RGB =
   (92,37,201))

2 - Write a program that:

1. Does the same as before, but uses the HSV space. (SUGGESTION: you should operate only on
   the H channel, once you segmented the shirts, if **old** is the current color of each segmented
   pixel, the new color will have new.H = old.H + 30)
