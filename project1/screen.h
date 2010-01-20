#ifndef _SCREEN_H
#define _SCREEN_H
class Screen {
   public:
      Screen(int pixelWidth, int pixelHeight, double left, double right, double top, double bottom);
      Vector3 * pixelToScreen(int x, int y);

   protected:
      double pixelToScreenX(int x);
      double pixelToScreenY(int y);
   private:
      int _pixelWidth, _pixelHeight;
      double _left, _right, _top, _bottom, _near;
};

Screen::Screen(int pixelWidth, int pixelHeight, double left, double right, double top, double bottom) {
   _pixelWidth = pixelWidth;
   _pixelHeight = pixelHeight;
   _left = left;
   _right = right;
   _top = top;
   _bottom = bottom;
   _near = 1;
}

Vector3 * Screen::pixelToScreen(int x, int y) {
   return new Vector3(pixelToScreenX(x), pixelToScreenY(y), _near);
}

double Screen::pixelToScreenX(int x) {
   double screenWidth = _right - _left;

   // handle a weird condition
   if(_pixelWidth == 1) {
      return _right + screenWidth / 2.0;
   }
   return _left + screenWidth * x / (_pixelWidth - 1);
}

double Screen::pixelToScreenY(int y) {
   double screenHeight = _top - _bottom;

   if(_pixelHeight == 1) {
      return _bottom + screenHeight;
   }

   return _bottom + screenHeight * y / (_pixelHeight - 1);
}
#endif // _SCREEN_H