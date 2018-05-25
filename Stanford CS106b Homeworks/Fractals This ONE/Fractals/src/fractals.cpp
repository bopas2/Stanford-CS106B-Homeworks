/*
 * File: fractals.cpp
 * --------------------------
 * Name: Thomas Lang
 * Section leader: Joshua
 * This file contains fractal problems for CS106B.
 *
 * The only extension I completed can be found by using Recursive Tree
 * PLEASE NOTE, recursive tree is of my own creation, and it is not the recursive tree described in the homework,
 * however it still uses recursion. It was noted that we could : "Add another fractal function representing a fractal image you like."
 * So I created my own version of Recursive tree :)
 *
 * For the recursive tree to work best, one should put the x and y values in the middle of the fullscreened board
 * And orders around 5-6 look the best
 *
 * Also the mandlebrot function is alot quicker in smaller windows rather than fullscreen (for obvious reasons)
 * It can do both, but more patience is required for the fullscreen one, at least on my computer
 */

#include "fractals.h"
#include <cmath>

using namespace std;

void drawTriangleRecursion(GWindow &gw, double x, double y, double size, int order);
void drawTriangle(GWindow &gw, double x, double y, double len);
void drawTreeHelper(GWindow &gw, int x, int y, int size, int order);

//This function calls the recursive function that draws the trianlges, also ensures that order isn't negative
void drawSierpinskiTriangle(GWindow& gw, double x, double y, double size, int order) {
    if(order < 0) {
        throw "Order is less than 0, Invalid input. Please enter an order greater or equal to zero.";    //ensures that the order will be positive
    }
    else {
        drawTriangleRecursion(gw,x,y,size,order);           //begin drawing
    }
}

//this function is where the recursion happens to draw the triangles
void drawTriangleRecursion(GWindow& gw, double x, double y, double size, int order) {
    if(order == 1) {
        drawTriangle(gw,x,y,size);  //if order is 1, we draw the triangle based on the sizes
    }
    else if(order == 0) {
        //do nothing        //if somehow we get to 0 we just quit without drawing anything
    }
    else {
        drawTriangleRecursion(gw, x, y, size / 2, order - 1);
        drawTriangleRecursion(gw,x + size / 2.0, y, size / 2, order - 1);                               //gives the points for the next triangles, because order is > 1
        drawTriangleRecursion(gw, x + (size / 4.0), y + size * sqrt(3) / 4.0,size / 2, order - 1);
    }
}
//this function takes a single coordinate pair and draws a triangle based on the given side length
void drawTriangle(GWindow &gw, double x, double y, double len) {
    GPoint pt1(x,y);
    GPoint pt2(x + len,y);                                      //creates the three points of the triangle
    GPoint pt3(x + (len / 2.0),y +(len * pow(3,.5) / 2.0));

    gw.drawLine(pt1,pt2);
    gw.drawLine(pt1,pt3);           //this draws the lines between the points
    gw.drawLine(pt2,pt3);

}

//this is my SPECIAL tree creater, draws a square, then a square in each corner, until order == 0
void drawTree(GWindow& gw, int x, int y, int size, int order) {
    if(order < 0) {
        return;
    }
    else {
        gw.setColor("#329213");
        gw.fillRect(x - size/2, y + 2.9 * size, 2 * size, size / 1.5);              //these two lines create the trunk and the ground, to make it more like a tree
        gw.setColor("BROWN");
        gw.fillRect(x + size / 4, y + size, size/2, size * 2);
        drawTreeHelper(gw,x,y,size,order);                          //this is the recursion that draws the leaves of the tree
    }
}
//recursive function that does the calculations and drawing of the leaves
void drawTreeHelper(GWindow &gw, int x, int y, int size, int order) {
    if(order == 0) {
        //do nothing
    }
    else {
        //get a random color for the tree rectangle "leaf"
        string r = integerToString(randomInteger(1,98)); string g = integerToString(randomInteger(1,98)); string b = integerToString(randomInteger(1,98));
        string rgb = "#" + r + g + b;
        gw.setColor(rgb);
        gw.fillRect(x,y,size,size); //draw the square

        drawTreeHelper(gw,x - size / 2, y - size/2, size/2, order - 1);
        drawTreeHelper(gw,x + size, y - size/2, size/2, order - 1);
        drawTreeHelper(gw,x - size / 2, y + size, size/2, order - 1);           //recurse! draws the smaller squares in each corner of the original square
        drawTreeHelper(gw,x + size, y + size, size/2, order - 1);
    }
}
//this function goes through the pixels in the image if that coordinate is in the mandelbrot set it will draw it on the screen.
void mandelbrotSet(GWindow& gw, double minX, double incX,
                   double minY, double incY, int maxIterations, int color) {
    Vector<int> palette = setPalette();
    int width = gw.getCanvasWidth();
    int height = gw.getCanvasHeight();
    GBufferedImage image(width,height,0xffffff);            //base code supplied
    gw.add(&image);
    int numOfIterations;
    Grid<int> pixels = image.toGrid(); // Convert image to grid

    for ( int x = 0; x < pixels.numRows(); x++) {                           //loop through each pixel
        for(int y = 0; y < pixels.numCols(); y++) {
            Complex c(minX + incX * y, minY + incY * x);                        //make the needed complex number
            numOfIterations = mandelbrotSetIterations(c, maxIterations);        //determines if something is in the mandelbrot set based on if the coordinates made it through                                                                                //"maxIterations" of loops through the recursive function
            pixels[x][y] = palette[numOfIterations % palette.size()];           //based on number of iterations we use a different palette color
        }
    }
    image.fromGrid(pixels); // Converts and puts the grid back into the image
}

//returns the difference between how many iterations the coordinate pair went through vs how many we need it to go through to be considered a mandelbrot coordinate
int mandelbrotSetIterations(Complex c, int maxIterations) {
    Complex num (0,0);
    return maxIterations - mandelbrotSetIterations(num,c,maxIterations);    //calls recursive function!
}

//the actual recursive function that sees if a coordinate belongs in the set
int mandelbrotSetIterations(Complex z, Complex c, int remainingIterations) {
    if(z.abs() > 4) {
        return remainingIterations;         //it's straying too far away from what we want, not a valid canidate
    }
    else if(remainingIterations == 0) {
        return 0;                           //yes! a valid canidate, made it through all the loops!
    }
    else {
        return mandelbrotSetIterations(z*z + c, c, remainingIterations - 1);    //recurses using the formula supplied in the pdf
    }
}

//function to set the palette
Vector<int> setPalette() {
    Vector<int> colors;

    // Feel free to replace with any palette.
    // You can find palettes at:
    // http://www.colourlovers.com/palettes

    // Example palettes:
    // http://www.colourlovers.com/palette/4480793/in_the_middle
    // string colorSt = "#A0B965,#908F84,#BF3C43,#9D8E70,#C9BE91,#A0B965,#908F84,#BF3C43";

    // http://www.colourlovers.com/palette/4480786/Classy_Glass
    // string colorSt = "#9AB0E9,#C47624,#25269A,#B72202,#00002E,#9AB0E9,#C47624,#25269A";

    // The following is the "Hope" palette:
    // http://www.colourlovers.com/palette/524048/Hope
    string colorSt =  "#5E412F,#FCEBB6,#78C0A8,#F07818,#F0A830,#5E412F,#FCEBB6,#78C0A8";
    Vector<string>colorsStrVec = stringSplit(colorSt,",");
    for (string color : colorsStrVec) {
        colors.add(convertColorToRGB(trim(color)));
    }
    return colors;
}
