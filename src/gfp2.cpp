#include <gfp2.h>
#include <sstream>

namespace bn256 {

   gfp2 gfp2::gfp2_decode(const gfp2& in) noexcept {
      return {in.x_.mont_decode(),  in.y_.mont_decode()};
   }


   gfp2 gfp2::conjugate() const noexcept {
      return { x_.neg(), y_};
   }

   gfp2 gfp2::neg() const noexcept {
      return { x_.neg(), y_.neg() };
   }

   gfp2 gfp2::add(const gfp2& b) const noexcept {
      return {x_.add(b.x_), y_.add(b.y_)};
   }

   gfp2 gfp2::sub(const gfp2& b) const noexcept {
      return { x_.sub(b.x_), y_.sub(b.y_)};
   }

   // See "Multiplication and Squaring in Pairing-Friendly Fields",
   // http://eprint.iacr.org/2006/471.pdf
   gfp2 gfp2::mul(const gfp2& b) const noexcept {
      const gfp2& a = *this;
      gfp tx, t;

      tx = a.x_.mul(b.y_);
      t  = b.x_.mul(a.y_);
      tx = tx.add(t);

      gfp ty;
      ty = a.y_.mul(b.y_);
      t = a.x_.mul(b.x_);
      ty = ty.sub(t);

      return { tx, ty};
   }

   gfp2 gfp2::mul_scalar(const gfp& b) const noexcept {
      return { x_.mul(b), y_.mul(b)};
   }

   // MulXi sets e=ξa where ξ=i+9 and then returns e.
   gfp2 gfp2::mul_xi() const noexcept {
      const gfp2& a = *this;
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

      return { tx, ty };
   }

   gfp2 gfp2::square() const noexcept {
      // Complex squaring algorithm:
      // (xi+y)² = (x+y)(y-x) + 2*i*x*y
      const gfp2& a = *this;
      gfp tx{}, ty{};
      tx = a.y_.sub(a.x_);
      ty = a.x_.add(a.y_);
      ty = tx.mul(ty);

      tx = a.x_.mul(a.y_);
      tx = tx.add(tx);

      return { tx, ty };
   }

   gfp2 gfp2::invert() const noexcept {
      // See "Implementing cryptographic pairings", M. Scott, section 3.2.
      // ftp://136.206.11.249/pub/crypto/pairings.pdf
      const gfp2& a = *this;
      gfp t1{}, t2{};

      t1 = a.x_.mul(a.x_);
      t2 = a.y_.mul(a.y_);
      t1 = t1.add(t2);

      gfp inv = t1.invert();

      t1 = a.x_.neg();

      return {t1.mul(inv),  a.y_.mul(inv)};
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
