#include <iomanip>
#include <iostream>
#include <string>
#include <utility>

#define RKE_PRINT_MOVING(paramName, moving)                                                  \
  std::cout << "At line " << __LINE__ << ": variable " << std::quoted(paramName) << " with " \
            << moving << std::endl

#define RKE_PRINT_MOVING_CONSTRUCTOR() \
  std::cout << "\t" << __FUNCTION__ << "() at line " << __LINE__ << " with " << (*this) << std::endl

namespace rke {
  class Moving {
   public:
    Moving(int i, std::string str);
    virtual ~Moving();

    Moving(const Moving& other);
    Moving(Moving&& other) noexcept;

    Moving& operator=(const Moving& other);
    Moving& operator=(Moving&& other) noexcept;

    [[nodiscard]] int i() const { return i_; }
    [[nodiscard]] int count() const { return count_; }
    [[nodiscard]] const std::string& str() const { return str_; }

   private:
    int i_;
    int count_;
    std::string str_;
  };

  std::ostream& operator<<(std::ostream& out, const Moving& moving) {
    return out << "{" << std::quoted("i") << ": " << moving.i() << ", " << std::quoted("count")
               << ": " << moving.count() << ", " << std::quoted("str") << ": "
               << std::quoted(moving.str()) << "}";
  }

  Moving::Moving(int i, std::string str)
      : i_(i), count_(0), str_(std::move(str) + "_constructing") {
    RKE_PRINT_MOVING_CONSTRUCTOR();
  }

  Moving::~Moving() {
    RKE_PRINT_MOVING_CONSTRUCTOR();
  }

  Moving::Moving(const Moving& other) : i_(other.i_), count_(other.count_), str_(other.str_) {
    count_++;
    str_ += "_copying";
    RKE_PRINT_MOVING_CONSTRUCTOR();
  }

  Moving::Moving(Moving&& other) noexcept
      : i_(other.i_), count_(other.count_), str_(std::move(other.str_)) {
    count_++;
    str_ += "_moving";
    RKE_PRINT_MOVING_CONSTRUCTOR();
  }

  Moving& Moving::operator=(const Moving& other) {
    count_ = other.count_;
    i_ = other.i_;
    str_ = other.str_;
    count_++;
    str_ += "_assigning";
    RKE_PRINT_MOVING_CONSTRUCTOR();
    return *this;
  }

  Moving& Moving::operator=(Moving&& other) noexcept {
    std::swap(count_, other.count_);
    std::swap(i_, other.i_);
    std::swap(str_, other.str_);
    count_++;
    str_ += "_moveassigning";
    RKE_PRINT_MOVING_CONSTRUCTOR();
    return *this;
  }

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
  RKE_PRINT_MOVING("moving1", moving1);  // NOLINT(bugprone-use-after-move)
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
  RKE_PRINT_MOVING("moving5", moving5);  // NOLINT(bugprone-use-after-move)
  RKE_PRINT_MOVING("moving6", moving6);
  std::cout << std::endl;

  auto moving7 = rke::Moving(7, "rke7");
  RKE_PRINT_MOVING("moving7", moving7);
  moving7 = rke::Moving(8, "rke8");
  RKE_PRINT_MOVING("moving7", moving7);
  std::cout << std::endl;

  return 0;
}
