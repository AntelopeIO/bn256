#include <gfp2.h>
#include <sstream>

namespace bn256 {

   gfp2 gfp2::gfp2_decode(const gfp2& in) {
      return {in.x_.mont_decode(),  in.y_.mont_decode()};
   }

   const gfp2& gfp2::set_zero() {
      x_.set_zero();
      y_.set_zero();
      return *this;
   }

   const gfp2& gfp2::set_one() {
      x_.set_zero();
      y_ = new_gfp(1);
      return *this;
   }

   bool gfp2::is_zero() const {
      constexpr gfp zero{};
      return zero == x_ && zero == y_;
   }

   bool gfp2::is_one() const {
      constexpr gfp zero{};
      static const gfp one = new_gfp(1);
      return zero == x_ && one == y_;
   }

   const gfp2& gfp2::conjugate(const gfp2& a) {
      y_ = a.y_;
      x_ = a.x_.neg();
      return *this;
   }

   const gfp2& gfp2::neg(const gfp2& a) {
      x_ = a.x_.neg();
      y_ = a.y_.neg();
      return *this;
   }

   const gfp2& gfp2::add(const gfp2& a, const gfp2& b) {
      x_ = a.x_.add(b.x_);
      y_ = a.y_.add(b.y_);
      return *this;
   }

   const gfp2& gfp2::sub(const gfp2& a, const gfp2& b) {
      x_ = a.x_.sub(b.x_);
      y_ = a.y_.sub(b.y_);
      return *this;
   }

   // See "Multiplication and Squaring in Pairing-Friendly Fields",
   // http://eprint.iacr.org/2006/471.pdf
   const gfp2& gfp2::mul(const gfp2& a, const gfp2& b) {
      gfp tx{}, t{}, ty{};

      tx = a.x_.mul(b.y_);
      t  = b.x_.mul(a.y_);
      tx = tx.add(t);

      ty = a.y_.mul(b.y_);
      t = a.x_.mul(b.x_);
      ty = ty.sub(t);

      x_ = tx;
      y_ = ty;

      return *this;
   }

   const gfp2& gfp2::mul_scalar(const gfp2& a, const gfp& b) {
      x_ = a.x_.mul(b);
      y_ = a.y_.mul(b);
      return *this;
   }

   // MulXi sets e=ξa where ξ=i+9 and then returns e.
   const gfp2& gfp2::mul_xi(const gfp2& a) {
      gfp tx;
      tx = a.x_.add(a.x_);
      tx = tx.add(tx);
      tx = tx.add(tx);
      tx = tx.add(a.x_);

      tx = tx.add(a.y_);

      gfp ty;
      ty = a.y_.add(a.y_);
      ty = ty.add(ty);
      ty = ty.add(ty);
      ty = ty.add(a.y_);

      ty = ty.sub(a.x_);

      x_ = tx;
      y_ = ty;

      return *this;
   }

   const gfp2& gfp2::square(const gfp2& a) {
      // Complex squaring algorithm:
      // (xi+y)² = (x+y)(y-x) + 2*i*x*y
      gfp tx{}, ty{};
      tx = a.y_.sub(a.x_);
      ty = a.x_.add(a.y_);
      ty = tx.mul(ty);

      tx = a.x_.mul(a.y_);
      tx = tx.add(tx);

      x_ = tx;
      y_ = ty;

      return *this;
   }

   const gfp2& gfp2::invert(const gfp2& a) {
      // See "Implementing cryptographic pairings", M. Scott, section 3.2.
      // ftp://136.206.11.249/pub/crypto/pairings.pdf
      gfp t1{}, t2{};

      t1 = a.x_.mul(a.x_);
      t2 = a.y_.mul(a.y_);
      t1 = t1.add(t2);

      gfp inv = t1.invert();

      t1 = a.x_.neg();

      x_ = t1.mul(inv);
      y_ = a.y_.mul(inv);

      return *this;
   }

   bool gfp2::operator==(const gfp2& rhs) const {
      return x_ == rhs.x_ &&
             y_ == rhs.y_;
   }

   bool gfp2::operator!=(const gfp2& rhs) const {
      return !(rhs == *this);
   }

   std::string gfp2::string() const {
      std::stringstream ss;
      ss << "(" << x_.string() << ", " << y_.string() << ")";
      return ss.str();
   }

   std::ostream& operator << (std::ostream& os, const gfp2& v) {
        return os << "(" << v.x_ << ", " << v.y_ << ")";
    }
}
