## COMPUTER VISION - LAB 4

**Topics:** Hough transform and Edge detection

**Goal:** Segment the street lane and the round street signs from the image provided

Create a C++ program, preferably organized in a class, that:

1. Loads an image
2. Colors in red all the pixels belonging to the right-most street lane using only the lines extracted
   using the standard Hough Transform. This is the suggested pipeline:
   a. Generate the Canny image. Use one/more trackbars to decide the best parameters to
   use. The function to generate the image is cv::Canny()
   b. Use the image generated in 2.a as input for the standard Hough transform
   (cv::HoughLines() function). In particular, we suggest you to show the 2 strongest
   lines found on the original image using one/more trackbars to change the parameters
   for the Hough transform. The returned lines should reflect the boundaries of the street
   lane to color.
   c. Color in red all the pixels between the lines.
3. Colors in green the pixels belonging to the round street sign using the same suggestions in 2.
   The function to compute the circles is cv::HoughCircles()
