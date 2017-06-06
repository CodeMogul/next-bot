/**
 *  Shapes library used by the U8g2_GraphicsEngine Library.
 *  
 *  @author Siddhesh Nachane
 *  @version 0.9 17-02-2017
 */

#include "Shapes.h"


uint8_t Shape::objCount = 0;

Shape::Shape(uint8_t id, shapeType_t type) : _id(id), _type(type)
{
    _state = VISIBLE;
    objCount++;
}

void Shape::initAnimation(uint8_t x, uint8_t y)
{
    _state = ANIMATE;

    endX = x;
    endY = y;

    int diffX = endX - startX, diffY = endY - startY;
    // Do not do abs(endX - startX), it may not work in some cases
    dX = abs(diffX);
    stepX = startX < endX ? 1 : -1;
    dY = abs(diffY);
    stepY = startY < endY ? 1 : -1;
    err = (dX > dY ? dX : -dY) / 2;
}

bool Shape::move()
{
    if(_state != ANIMATE) return false;
    
    if (startX == endX && startY == endY)
    {
        _state = VISIBLE;
        return false;
    }

    changeX = 0;
    changeY = 0;
    int16_t err2 = err;

    if (err2 > -dX)
    {
        err -= dY;
        startX += stepX;
        changeX = stepX;
    }

    if (err2 < dY)
    {
        err += dX;
        startY += stepY;
        changeY = stepY;
    }
    return true;
}

// ------------------------------------------------------------------------

Pixel::Pixel(uint8_t id, uint8_t x, uint8_t y):Shape(id, POINT)
{
    startX = x;
    startY = y; 
}

void Pixel::draw(display &disp)
{
    disp.drawPixel(startX, startY);
}
// ------------------------------------------------------------------------

Line::Line(uint8_t id, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
    : Shape(id, LINE)
{
    _points[0].x = x0;
    _points[0].y = y0;
    _points[1].x = x1;
    _points[1].y = y1;

    // check for some condition here
    startX = (_points[0].x + _points[1].x) / 2;
    startY = (_points[0].y + _points[1].y) / 2;
}

bool Line::move()
{
    if (Shape::move())
    {
        _points[0].x += changeX;
        _points[0].y += changeY;
        _points[1].x += changeX;
        _points[1].y += changeY;
        return true;
    }
    return false;
}

void Line::draw(display &disp)
{
    disp.drawLine(_points[0].x, _points[0].y, _points[1].x, _points[1].y);
}

// ------------------------------------------------------------------------

Circle::Circle(uint8_t id, uint8_t x0, uint8_t y0, uint8_t radius, bool filled)
    : Shape(id, CIRCLE), _filled(filled)
{
    // ! Use Shape's startX and startY as Center
    _center.x = x0;
    _center.y = y0;
    _radius = radius;

    //check for some condition
    startX = _center.x;
    startY = _center.y;
}

bool Circle::move()
{
    if (_state == ANIMATE && Shape::move())
    {
        _center.x += changeX;
        _center.y += changeY;
        return true;
    }
    return false;
}

void Circle::draw(display &disp)
{
    if (_filled)    disp.drawDisc(_center.x, _center.y, _radius);
    else            disp.drawCircle(_center.x, _center.y, _radius);
}

// ------------------------------------------------------------------------

Triangle::Triangle(uint8_t id, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
    : Shape(id, TRIANGLE)
{
    _points[0].x = x0;
    _points[0].y = y0;
    _points[1].x = x1;
    _points[1].y = y1;
    _points[2].x = x2;
    _points[2].y = y2;

    startX = (_points[0].x + _points[1].x + _points[2].x) / 3;
    startY = (_points[0].y + _points[1].y + _points[2].y) / 3;
}

bool Triangle::move()
{
    if (Shape::move())
    {
        for (uint8_t i = 0; i < 3; i++)
        {
            _points[i].x += changeX;
            _points[i].y += changeY;
        }
        return true;
    }
    return false;
}
void Triangle::draw(display &disp)
{
    disp.drawTriangle(_points[0].x, _points[0].y, _points[1].x, _points[1].y, _points[2].x, _points[2].y);
}

// ------------------------------------------------------------------------

Rectangle::Rectangle(uint8_t id, uint8_t x, uint8_t y, uint8_t width, uint8_t height, bool filled)
    :Shape(id, RECTANGLE), _width(width), _height(height), _filled(filled)
{
    _leftCorner.x = x;
    _leftCorner.y = y;

    startX = x + (width/2);
    startY = y + (height/2);
}

bool Rectangle::move()
{
    if(Shape::move())
    {
        _leftCorner.x += changeX;
        _leftCorner.y += changeY;
        return true;
    }
    return false;
}

void Rectangle::draw(display& disp)
{
    if(_filled) disp.drawBox(_leftCorner.x, _leftCorner.y, _width, _height);
    else        disp.drawFrame(_leftCorner.x, _leftCorner.y, _width, _height);
}