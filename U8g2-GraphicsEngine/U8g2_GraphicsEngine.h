/**
 *  A wrapper around U8g2 library. Acts as a graphics engine for various shapes.
 *  
 *  @author Siddhesh Nachane
 *  @version 0.9 19-02-2017
 */

#ifndef U8g2_GraphicsEngine_h
#define U8g2_GraphicsEngine_h

#include "Arduino.h"
#include "Shapes.h"

/** A structure representing a Line of Text on the Display.
 *
 * Acts as a node in linked list with HEAD = *lines.
 * Each node stores a line of text that fits on the screen, given the selected font.
 * Wrapping, etc., should be done before adding string to this list.
 */
typedef struct text {
  char *data;
  struct text *next;
} Text;

/** A graphic engine for U8g2 library.
 *
 *  A wrapper for the U8g2 library. This a Graphic Engine that stores different shapes as objetcs.
 *  It allows allows gliding of objects form one point to another.
 *  Thus giving the user ability to make some beautiful animations.
 */
class GraphicEngine
{
    public:
        /** Constructor for GraphicEngine class.
         *  
         *  @param disp A refernce to U8g2 object.
         *  @param maxObjects Maximum number of objects to be contatined by the graphic engine.
         */
        GraphicEngine(display&, uint8_t);

        /** Initializer for the GraphicEngine.
         *  One cannot use the graphic engine without calling this function.
         */
        void begin() { _oled.begin(); }

        /** Moves a shape from one refernce point to a final point.
         *  
         *  @param id The ID of the shape.
         *  @param x x-coordinate of final position.
         *  @param y y-coordinate of final position.
         */
        void glide(int8_t, uint8_t, uint8_t);

        /** Creates a Pixel object and draws it on the frame when updateFrame() is called.
         *  
         *  @param id Unique id for the Pixel(Shape) object.
         *  @param x x-coordinate of the pixel.
         *  @param y y-coordinate of the pixel.
         *
         *  @return An integer ID of the object if created, else -1.
         */
        int8_t drawPixel(uint8_t, uint8_t);

        /** Creates a Line object and draws it on the frame when updateFrame() is called..
         *  
         *  @param x0 x-coordinate of one point.
         *  @param y0 y-coordinate of one point.
         *  @param x1 x-coordinate of other point.
         *  @param y1 y-coordinate of other point.
         *  
         *  @return An integer ID of the object if created, else -1.
         */
        int8_t drawLine(uint8_t, uint8_t, uint8_t, uint8_t);

        /** Creates a Line object and draws it on the frame when updateFrame() is called..
         *  
         *  @param x0 x-coordinate of the center.
         *  @param y0 y-coordinate of the center.
         *  @param radius Radius of the circle.
         *  
         *  @return An integer ID of the object if created, else -1.
         */
        int8_t drawCircle(uint8_t, uint8_t, uint8_t);

        /** Creates a Circle object that is filled and draws it on the frame when updateFrame() is called..
         *  
         *  @param x0 x-coordinate of the center.
         *  @param y0 y-coordinate of the center.
         *  @param radius Radius of the disc.
         *  
         *  @return An integer ID of the object if created, else -1.
         */
        int8_t drawDisc(uint8_t, uint8_t, uint8_t);

        /** Creates a Traingle object and draws it on the frame when updateFrame() is called..
         *  
         *  @param x0 x-coordinate of the first point.
         *  @param y0 y-coordinate of the first point.
         *  @param x1 x-coordinate of the second point.
         *  @param y1 y-coordinate of the second point.
         *  @param x2 x-coordinate of the third point.
         *  @param y2 y-coordinate of the third point
         *  
         *  @return An integer ID of the object if created, else -1.
         */
        int8_t drawTriangle(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);

        /** Creates a Rectangle object and draws it on the frame when updateFrame() is called..
         *  
         *  @param x0 x-coordinate of the left corner.
         *  @param y0 y-coordinate of the left corner.
         *  @param radius Width of the rectangle.
         *  @param radius Height of the rectangle.
         *  
         *  @return An integer ID of the object if created, else -1.
         */
        int8_t drawRectangle(uint8_t, uint8_t, uint8_t, uint8_t);

        /** Creates a Rectangle object that is filled and draws it on the frame when updateFrame() is called..
         *  
         *  @param x0 x-coordinate of the left corner.
         *  @param y0 y-coordinate of the left corner.
         *  @param radius Width of the box.
         *  @param radius Height of the box.
         *  
         *  @return An integer ID of the object if created, else -1.
         */
	    int8_t drawBox(uint8_t, uint8_t, uint8_t, uint8_t);

        /** Adds a String to the print buffer, to be printed on the next line.
         *
         *  @param str C-type char pointer to a string.
         */
        void drawStr(char *str);

        /** Prints the string buffer to the OLED Display.
         *
         */
        void updateStr();

        /** Clear display function.
         *
         * Deletes all Shape objects and Text from them Memory/buffer.
         * Frees the memory and clears the display.
         */
        void clear();

        /** Member function to draw all current visible and animated objects on the screen.
         *
         *  Has to be called by the user at rate at which he/she wishes to update the frame.
         */
        bool updateFrame();

    private:
        uint8_t _maxObjects;        /**< Max number of objects that can be contained or displayed by the Graphics Engine.*/
        static uint8_t _objCount;   /**< Current nuumber objects present in the GraphicEngine.*/
        Shape **_objects;           /**< A pointer array to all shapes created.*/
        display& _oled;             /**< A local reference to the U8g2 object.*/
        static Text *lines;         /**< A linked list containing all Text as wrapped Lines. */
};

#endif