#include <iomanip>
#include <iostream>
#include <string>

#define RKE_PRINT_MOVING(paramName, moving)                                                  \
  std::cout << "Variable " << std::quoted(paramName) << ": {" << std::quoted("i") << ": "    \
            << moving.i() << ", " << std::quoted("str") << ": " << std::quoted(moving.str()) \
            << "}" << std::endl;

#define RKE_PRINT_MOVING_CONSTRUCTOR()                                \
  std::cout << __FUNCTION__ << "() at line " << __LINE__ << " with "; \
  RKE_PRINT_MOVING("this", (*this));

namespace rke {
  class Moving {
   public:
    Moving(int i, std::string str) : i_(i), iInitial_(i), str_(str) {
      RKE_PRINT_MOVING_CONSTRUCTOR();
    }
    Moving(const Moving& other) : i_(other.i_), str_(other.str_) {
      i_++;
      str_ += "_copying";
      RKE_PRINT_MOVING_CONSTRUCTOR();
    }
    Moving(Moving&& other) : i_(std::move(other.i_)), str_(std::move(other.str_)) {
      i_++;
      str_ += "_moving";
      RKE_PRINT_MOVING_CONSTRUCTOR();
    }

    virtual ~Moving() { RKE_PRINT_MOVING_CONSTRUCTOR(); }

    Moving& operator=(const Moving& other) {
      i_ = other.i_;
      str_ = other.str_;
      i_++;
      str_ += "_assigning";
      RKE_PRINT_MOVING_CONSTRUCTOR();
      return *this;
    }
    Moving& operator=(Moving&& other) {
      std::swap(i_, other.i_);
      std::swap(str_, other.str_);
      i_++;
      str_ += "_moveassigning";
      RKE_PRINT_MOVING_CONSTRUCTOR();
      return *this;
    }

    const int& i() const { return i_; }
    const int& iInital() const { return iInitial_; }
    const std::string& str() const { return str_; }

   private:
    int i_;
    int iInitial_;
    std::string str_;
  };

  Moving mover(Moving moving) {
    return moving;
  }
}  // namespace rke

int main() {
  auto moving1 = rke::Moving(1, "rke1");
  RKE_PRINT_MOVING("moving1", moving1);
  std::cout << std::endl;

  auto moving2(moving1);
  RKE_PRINT_MOVING("moving1", moving1);
  RKE_PRINT_MOVING("moving2", moving2);
  std::cout << std::endl;

  auto moving3(std::move(moving1));
  RKE_PRINT_MOVING("moving1", moving1);
  RKE_PRINT_MOVING("moving3", moving3);
  std::cout << std::endl;

  moving3 = moving2;
  RKE_PRINT_MOVING("moving2", moving2);
  RKE_PRINT_MOVING("moving3", moving3);
  std::cout << std::endl;

  auto moving4(rke::mover(rke::Moving(4, "rke4")));
  RKE_PRINT_MOVING("moving4", moving4);
  std::cout << std::endl;

  auto moving5 = rke::mover(rke::Moving(5, "rke5"));
  RKE_PRINT_MOVING("moving5", moving5);
  std::cout << std::endl;

  auto moving6 = std::move(moving5);
  RKE_PRINT_MOVING("moving5", moving5);
  RKE_PRINT_MOVING("moving6", moving6);
  std::cout << std::endl;

  auto moving7 = rke::Moving(7, "rke7");
  RKE_PRINT_MOVING("moving7", moving7);
  moving7 = rke::Moving(8, "rke8");
  RKE_PRINT_MOVING("moving7", moving7);
  std::cout << std::endl;

  return 0;
}
