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
Text* GraphicEngine::lines = NULL;

GraphicEngine::GraphicEngine(display& disp, uint8_t maxObjects):_oled(disp), _maxObjects(maxObjects)
{
    _objects = new Shape*[maxObjects];
    for(int i = 0 ; i < _maxObjects ; i++) 
        _objects[i] = NULL;
}

int8_t GraphicEngine::drawPixel(uint8_t x, uint8_t y)
{
    if(_objCount == _maxObjects) return -1;

    _objects[_objCount] = new Pixel(_objCount, x, y);
    _objCount++;
    return _objCount-1;
}

int8_t GraphicEngine::drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
    if(_objCount == _maxObjects) return -1;

    _objects[_objCount] = new Line(_objCount, x0, y0, x1, y1);
    _objCount++;
    return _objCount-1;
}

int8_t GraphicEngine::drawCircle(uint8_t x, uint8_t y, uint8_t radius)
{
    if(_objCount == _maxObjects) return -1;

    _objects[_objCount] = new Circle(_objCount, x, y, radius, false);
    _objCount++;
    return _objCount-1;
}

int8_t GraphicEngine::drawDisc(uint8_t x, uint8_t y, uint8_t radius)
{
    if(_objCount == _maxObjects) return -1;

    _objects[_objCount] = new Circle(_objCount, x, y, radius, true);
    _objCount++;
    return _objCount-1;
}

int8_t GraphicEngine::drawTriangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
    if(_objCount == _maxObjects) return -1;

    _objects[_objCount] = new Triangle(_objCount, x0, y0, x1, y1, x2, y2);
    _objCount++;
    return _objCount-1;
}

int8_t GraphicEngine::drawRectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
    if(_objCount == _maxObjects) return -1;

    _objects[_objCount] = new Rectangle(_objCount, x, y, width, height, false);
    _objCount++;
    return _objCount-1;
}

int8_t GraphicEngine::drawBox(uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
    if(_objCount == _maxObjects) return -1;

    _objects[_objCount] = new Rectangle(_objCount, x, y, width, height, true);
    _objCount++;
    return _objCount-1;
}

void GraphicEngine::drawStr(char *str)
{
    Text *p = new Text();
    p->data = str;
    p->next = NULL;
    if(!lines) lines = p;
    else {
        Text *temp = lines;
        while(temp->next) temp = temp->next;
        temp->next = p;
    }
}

void GraphicEngine::updateStr()
{
    Text *temp = lines;

    _oled.setFont(u8g2_font_9x15B_tr);
    _oled.setFontMode(1);  /* activate transparent font mode */
    int y = 14;

    while(temp) {
        _oled.drawStr(0, y, temp->data);
        temp = temp->next;
        y = y + 16;
    } 
}

void GraphicEngine::clear()
{
    // Clear all shapes from Memory
    for(uint8_t i = 0 ; i < _objCount ; i++)
    {
        delete _objects[i];
        _objects[i] = NULL;
    }

    _objCount = 0;

    // Clear all Text from Memory
    Text *temp = lines, *next;
    while(temp) {
        next = temp->next;
        //delete(temp->data);
        delete temp;
        temp = next;
    }
    lines = NULL;

    // Clear screen
    _oled.clear();
}

bool GraphicEngine::updateFrame()
{
    bool print = false;
    for(uint8_t i = 0 ; i < _objCount ; i++)
    {
        if(_objects[i]->_state == VISIBLE || _objects[i]->move())
            print =true;
    }
     
    if(print || lines)
    {
        _oled.firstPage();  
        do {
            _oled.setDrawColor(1);       /* set Color to On mode for Shapes */
            for(uint8_t i = 0 ; i < _objCount ; i++)
                _objects[i]->draw(_oled);

            if(lines) {
                _oled.setDrawColor(2);       /* set Color to XOR mode for Text */
                updateStr();
            }
        }
        while(_oled.nextPage());
    }
    
    return print;
}

void GraphicEngine::glide(int8_t id, uint8_t x, uint8_t y)
{
	if(id > -1 && _objects[id])
        _objects[id]->initAnimation(x,y);
}
