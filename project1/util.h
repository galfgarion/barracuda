double clamp(const double x, const double min, const double max)
{
   if(x < min)
      return min;
   else if(x > max)
      return max;
   else
      return x;
}
