// turtle.hpp under gold++.turtles

#ifndef __GOLDPLUSPLUS_TURTLES_TURTLE_HPP__
#define __GOLDPLUSPLUS_TURTLES_TURTLE_HPP__

#define NS_BEGIN namespace gold {
#define NS_END }

#include <cmath>
#include <utilites>
#include <type_traits>
#include <concepts>
#include <GL/glut.h>

// Since this library supports C++ 20, this should be.
#include <numbers>
static constexpr double PI = std::numbers::pi_v<long double>;

NS_BEGIN
inline namespace turtle {
  template <typename T>
  concept Unsigned_Char = std::is_integral_v<T> || std::is_unsigned_v<T> || std::is_same_v<T, GLubyte>;
  
  class Turtle {
  private:
    double x;
    double y;
    double dir;
    bool draw;
    bool is_degree;
    bool is_fill;
    struct point {
      double x;
      double y;
    }
  public:
    Turtle();
    Turtle& reset();
    Turtle& setpos(const double&, const double&);
    std::pair<double, double> getpos();
    Turtle& setx(const double&);
    Turtle& sety(const double&);
    Turtle& setdir(const double&);
    Turtle& degree_mode();
    Turtle& radian_mode();
    Turtle& pendown();
    Turtle& penup();
    Turtle& left(const double&);
    Turtle& right(const double&);
    Turtle& forward(const double&);
    Turtle& backward(const double&);
    Turtle& run_circle(const double&);
    Turtle& run_arc(const double& const double&);
    Turtle& dot();
    Turtle& begin_fill();
    Turtle& end_fill();
    Turtle& setpensize(const double&);
    Turtle& setdotsize(const double&);
    
    template <typename T> requires Unsigned_Char<T>
    Turtle& pencolor(T, T, T);
    template <typename T> requires std::integral<T>
    Turtle& pencolor(T);
    template <typename T> requires Unsigned_Char<T>
    Turtle& fillcolor(T, T, T);
    template <typename T> requires std::integral<T>
    Turtle& fillcolor(T);
    Turtle& resetcolor();
  }
}
NS_END
#undef NS_BEGIN
#undef NS_END
#endif // __GOLDPLUSPLUS_TURTLES_TURTLE_HPP__
