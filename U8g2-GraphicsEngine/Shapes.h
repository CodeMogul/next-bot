/**
 *  Shapes library used by the U8g2_GraphicsEngine Library.
 *  
 *  @author Siddhesh Nachane
 *  @version 0.9 17-02-2017
 */

#ifndef Shapes_h
#define Shapes_h

#include "U8g2lib.h"
#include "Arduino.h"

typedef U8G2_SH1106_128X64_NONAME_1_4W_HW_SPI display;

/** Enum defining states of a shape/object in GraphicEngine.
 *
 *  Possible Values: VISIBLE, HIDDEN, ANIMATE
 */
enum shapeState_t {VISIBLE, HIDDEN, ANIMATE};

/** Enum for the type of shape. 
 *  
 *  For internal purposes. Might be used later.
 *  Possible Values: POINT, LINE, TRIANGLE, RECTANGLE, CIRCLE
 */
enum shapeType_t {POINT, LINE, TRIANGLE, RECTANGLE, CIRCLE};

/** A structure to hold the co-ordinates of a point in 2D Space.
 *  
 *  May repersent a pixel on the display in some cases.
 */
struct Point{
    uint8_t x;
    uint8_t y;
};

/** Base class for shapes/objects.
 * 
 *  Is a pure virtual class and cannot be instantiated.
 */
class Shape
{
    public:
        uint8_t _id;                /**< An unique ID alloted to each shape at generation.  */
        shapeType_t _type;          /**< Stores the type of shape. Eg:LINE, CIRCLE, etc. */
        shapeState_t _state;        /**< Stores the cuurent state of shape. */
        static uint8_t objCount;        /**< A variable to track the total number of objects created. */
        int8_t changeX, changeY;    
        
        /** Constructor for Shape class.
         *  
         *  @param id Unique ID for each shape object.
         *  @param type Type of shape.
         *  @param state State of shape.
         */
        Shape(uint8_t, shapeType_t);

        /** Initialises the shape object to move towards a final point w.r.t a refernce point.
         *  
         *  Initalises protected variables for computation of Bresenham's Line Algorithm and
         *  sets the state of the object to ANIMATE.
         */
        void initAnimation(uint8_t, uint8_t);

        /** Sets the shape's state to HIDDEN*/
        void hide() { _state = HIDDEN; }

        /** Sets the shape's state to VISIBLE*/
        void show() {_state = VISIBLE; }

        /** Moves the reference point using Bresesnham's Line Algortihm.
         *  
         *  Moves reference point towards final point.
         *  Bresenham's Algorithm calculates the next point in the path to a final point.
         *  This call updates both startX and startY, and stores the changes in changeX and changeY
         *  
         *  @return true if reference point is moved towards final point, false if its reached the final point.
         */
        virtual bool move();
        
        /** Draws the shape on the OLED.
         *  
         *  Pure virtual funtion (to be implemented by Derived class).
         *  @param disp Reference to the display object. 
         */
        virtual void draw(display&) = 0;

    protected:
        uint8_t startX,    /**< x-coordinate for the reference point of the shape.\ 
                                Should be set by the constructor of derived class.*/
                startY,     /**< y-coordinate for the reference point of the shape.\  
                                Should be set by the constructor of derived class.*/
                endX,       /**< x-coordinate for the final point of the shape.\  
                                Is set by initAnimation().*/
                endY;       /**< y-coordinate for the final point of the shape.\  
                                Is set by initAnimation().*/
        int16_t dX, dY, stepX, stepY;
        int16_t err, err2;
        
};

class Pixel: public Shape
{
    public:
        /** Constructor for Pixel class.
         *  Here co-ordinates of the pixel are stored in startX and startY itself.
         *  @param id Unique id for the Pixel(Shape) object.
         *  @param x x-coordinate of the point.
         *  @param y y-coordinate of the point.
         */
        Pixel(uint8_t, uint8_t, uint8_t);

        /** Calls base Shape::move() and updates own points accordingly. 
        *
        *   @return true if object is moved towards final point, false if its reached the final point.
        */
        bool move();

        /** Draws line on the OLED using display object's functions.
         *  
         *  @param disp Reference to the display object. 
         */
        void draw(display&);
};

/** Represents a Line object to be displayed by the GraphicEngine */
class Line: public Shape
{
    public:
        /** Constructor for Line class.
         *  @param id Unique id for the Line(Shape) object.
         *  @param x0 x-coordinate of one point.
         *  @param y0 y-coordinate of one point.
         *  @param x1 x-coordinate of other point.
         *  @param y1 y-coordinate of other point.
         */
        Line(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);

        /** Calls base Shape::move() and updates own points accordingly. 
        *
        *   @return true if object is moved towards final point, false if its reached the final point.
        */
        bool move();

        /** Draws line on the OLED using display object's functions.
         *  
         *  @param disp Reference to the display object. 
         */
        void draw(display&);

    private:
        Point _points[2];
};

/** Represents a Circle object to be displayed by the GraphicEngine */
class Circle: public Shape
{
    public:
        /** Constructor for Circle class.
         *  @param id Unique id for the Circle(Shape) object.
         *  @param x0 x-coordinate of the center.
         *  @param y0 y-coordinate of the center.
         *  @param radius Radius of the circle.
         *  @param filled If the circle is filled(Disc) or not.
         */
        Circle(uint8_t, uint8_t, uint8_t, uint8_t, bool);

        /** Calls base Shape::move() and updates own points accordingly. 
        *
        *   @return true if object is moved towards final point, false if its reached the final point.
        */
        bool move();

        /** Draws circle on the OLED using display object's functions.
         *  
         *  @param disp Reference to the display object. 
         */
        void draw(display&);

    private:
        Point _center;
        uint8_t _radius;
        bool _filled;
};

/** Represents a Triangle object to be displayed by the GraphicEngine */
class Triangle: public Shape
{
    public:
        /** Constructor for Circle class.
         *  @param id Unique id for the Circle(Shape) object.
         *  @param x0 x-coordinate of the first point.
         *  @param y0 y-coordinate of the first point.
         *  @param x1 x-coordinate of the second point.
         *  @param y1 y-coordinate of the second point.
         *  @param x2 x-coordinate of the third point.
         *  @param y2 y-coordinate of the third point.
         */
        Triangle(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);

        /** Calls base Shape::move() and updates own points accordingly. 
        *
        *   @return true if object is moved towards final point, false if its reached the final point.
        */
        bool move();

        /** Draws triangle on the OLED using display object's functions.
         *  
         *  @param disp Reference to the display object. 
         */
        void draw(display&);

    private:
        Point _points[3];
};

/** Represents a Rectangle object to be displayed by the GraphicEngine */
class Rectangle: public Shape
{
    public:
        /** Constructor for Rectangle class.
         *  @param id Unique id for the Circle(Shape) object.
         *  @param x0 x-coordinate of the left corner.
         *  @param y0 y-coordinate of the left corner.
         *  @param radius Width of the rectangle.
         *  @param radius Height of the rectangle.
         *  @param filled If the circle is filled(Box) or not.
         */
        Rectangle(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, bool);

        /** Calls base Shape::move() and updates own points accordingly. 
        *
        *   @return true if object is moved towards final point, false if its reached the final point.
        */
        bool move();

        /** Draws rectangle on the OLED using display object's functions.
         *  
         *  @param disp Reference to the display object. 
         */
        void draw(display&);

    private:
        Point _leftCorner;           /**< Left corner of rectangle. */
        uint8_t _width, _height;
        bool _filled;
};

#endif