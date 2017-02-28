/**
 *  A wrapper around U8g2 library. Acts as a graphics engine for various shapes.
 *  
 *  @author Siddhesh Nachane
 *  @version 0.9 19-02-2017
 */

#include "U8g2_GraphicsEngine.h"
//#include "U8g2lib.h"
//#include "Shapes.h"

uint8_t GraphicEngine::_objCount = 0;

GraphicEngine::GraphicEngine(display& disp, uint8_t maxObjects):_oled(disp), _maxObjects(maxObjects)
{
    _objects = new Shape*[maxObjects];
}

uint8_t GraphicEngine::drawPixel(uint8_t x, uint8_t y)
{
    if(_objCount == _maxObjects) return -1;

    _objects[_objCount] = new Pixel(_objCount, x, y);
    _objCount++;
    return _objCount-1;
}

uint8_t GraphicEngine::drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
    if(_objCount == _maxObjects) return -1;

    _objects[_objCount] = new Line(_objCount, x0, y0, x1, y1);
    _objCount++;
    return _objCount-1;
}

uint8_t GraphicEngine::drawCircle(uint8_t x, uint8_t y, uint8_t radius)
{
    if(_objCount == _maxObjects) return -1;

    _objects[_objCount] = new Circle(_objCount, x, y, radius, false);
    _objCount++;
    return _objCount-1;
}

uint8_t GraphicEngine::drawDisc(uint8_t x, uint8_t y, uint8_t radius)
{
    if(_objCount == _maxObjects) return -1;

    _objects[_objCount] = new Circle(_objCount, x, y, radius, true);
    _objCount++;
    return _objCount-1;
}

uint8_t GraphicEngine::drawTriangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
    if(_objCount == _maxObjects) return -1;

    _objects[_objCount] = new Triangle(_objCount, x0, y0, x1, y1, x2, y2);
    _objCount++;
    return _objCount-1;
}

uint8_t GraphicEngine::drawRectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
    if(_objCount == _maxObjects) return -1;

    _objects[_objCount] = new Rectangle(_objCount, x, y, width, height, false);
    _objCount++;
    return _objCount-1;
}

uint8_t GraphicEngine::drawBox(uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
    if(_objCount == _maxObjects) return -1;

    _objects[_objCount] = new Rectangle(_objCount, x, y, width, height, true);
    _objCount++;
    return _objCount-1;
}

bool GraphicEngine::updateFrame()
{
    bool change = false;
    for(uint8_t i = 0 ; i < _objCount ; i++)
    {
        if(_objects[i]->move())
            change =true;
    }
     
    if(change)
    {
        _oled.firstPage();  
        do {
            for(uint8_t i = 0 ; i < _objCount ; i++)
                _objects[i]->draw(_oled);
        }
        while(_oled.nextPage());
        return true;
    }
    else return false;
}

void GraphicEngine::glide(uint8_t id, uint8_t x, uint8_t y)
{
	_objects[id]->initAnimation(x,y);
}
