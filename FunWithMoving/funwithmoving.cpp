#include <iomanip>
#include <iostream>
#include <string>
#include <utility>

#define RKE_PRINT_VARIABLE(paramName, variable) \
  std::cout << "Variable " << std::quoted(paramName) << " with " << (variable) << std::endl

namespace rke {
  class Moving {
   public:
    explicit Moving(int i);
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
    int count_ = 1;
    std::string str_;
  };

  std::ostream& operator<<(std::ostream& out, const Moving& moving) {
    return out << "{" << std::quoted("i") << ": " << moving.i() << ", " << std::quoted("count")
               << ": " << moving.count() << ", " << std::quoted("str") << ": "
               << std::quoted(moving.str()) << "}";
  }

  Moving::Moving(int i) : i_(i), str_(std::to_string(i) + "_constructing") {}

  Moving::~Moving() {
    count_--;
    std::cout << "               Deleting " << (*this) << std::endl;
  }

  Moving::Moving(const Moving& other) : i_(other.i_), count_(other.count_), str_(other.str_) {
    count_++;
    str_ += "_copying";
  }

  Moving::Moving(Moving&& other) noexcept
      : i_(other.i_), count_(other.count_), str_(std::move(other.str_)) {
    count_++;
    str_ += "_moving";
  }

  Moving& Moving::operator=(const Moving& other) {
    count_ = other.count_;
    i_ = other.i_;
    str_ = other.str_;
    count_++;
    str_ += "_assigning";
    return *this;
  }

  Moving& Moving::operator=(Moving&& other) noexcept {
    std::swap(count_, other.count_);
    std::swap(i_, other.i_);
    std::swap(str_, other.str_);
    count_++;
    str_ += "_moveassigning";
    return *this;
  }

  Moving mover(const Moving& moving) {
    return moving;
  }
}  // namespace rke

int main() {
  rke::Moving moving1(1);
  RKE_PRINT_VARIABLE("moving1", moving1);

  /* auto moving2 = rke::Moving(2); does not first call the constructor and then the (move)
   * assigment operator but just the constructor as moving2 was previously unassigned. */
  auto moving2 = rke::Moving(2);
  RKE_PRINT_VARIABLE("moving2", moving2);
  std::cout << std::endl;

  /* auto moving3(moving1); obviously calls copy constructor. */
  auto moving3(moving1);  // NOLINT(performance-unnecessary-copy-initialization)
  RKE_PRINT_VARIABLE("moving1", moving1);
  RKE_PRINT_VARIABLE("moving3", moving3);
  std::cout << std::endl;

  /* auto moving4 = moving1; also calls the copy constructor.  */
  auto moving4 = moving1;  // NOLINT(performance-unnecessary-copy-initialization)
  RKE_PRINT_VARIABLE("moving1", moving1);
  RKE_PRINT_VARIABLE("moving4", moving4);
  std::cout << std::endl;

  //  moving3 = moving2;
  //  RKE_PRINT_MOVING("moving2", moving2);
  //  RKE_PRINT_MOVING("moving3", moving3);
  //  std::cout << std::endl;
  //
  //  auto moving4(rke::mover(rke::Moving(4)));
  //  RKE_PRINT_MOVING("moving4", moving4);
  //  std::cout << std::endl;
  //
  //  auto moving5 = rke::mover(rke::Moving(5));
  //  RKE_PRINT_MOVING("moving5", moving5);
  //  std::cout << std::endl;
  //
  //  auto moving6 = std::move(moving5);
  //  RKE_PRINT_MOVING("moving5", moving5);  // NOLINT(bugprone-use-after-move)
  //  RKE_PRINT_MOVING("moving6", moving6);
  //  std::cout << std::endl;
  //
  //  auto moving7 = rke::Moving(7);
  //  RKE_PRINT_MOVING("moving7", moving7);
  //  moving7 = rke::Moving(8);
  //  RKE_PRINT_MOVING("moving7", moving7);
  //  std::cout << std::endl;

  return 0;
}
