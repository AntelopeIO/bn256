#include <optate.h>
#include <array>
#include <constants.h>

namespace bn256 {

   struct line_function_result_t {
      gfp2 a_{};
      gfp2 b_{};
      gfp2 c_{};
      twist_point out_{};
   };

   line_function_result_t
   line_function_add(const twist_point& r, const twist_point& p, const curve_point& q, const gfp2& r2) {
      line_function_result_t rc{};
      gfp2 b{}, d{}, h{}, i{}, e{}, j{}, l1{}, v{}, t{}, t2{};

      // See the mixed addition algorithm from "Faster Computation of the
      // Tate Pairing", http://arxiv.org/pdf/0904.0854v3.pdf
      b.mul(p.x_, r.t_);

      d.add(p.y_, r.z_);
      d.square(d);
      d.sub(d, r2);
      d.sub(d, r.t_);
      d.mul(d, r.t_);

      h.sub(b, r.x_);
      i.square(h);

      e.add(i, i);
      e.add(e, e);

      j.mul(h, e);

      l1.sub(d, r.y_);
      l1.sub(l1, r.y_);

      v.mul(r.x_, e);

      rc.out_.x_.square(l1);
      rc.out_.x_.sub(rc.out_.x_, j);
      rc.out_.x_.sub(rc.out_.x_, v);
      rc.out_.x_.sub(rc.out_.x_, v);

      rc.out_.z_.add(r.z_, h);
      rc.out_.z_.square(rc.out_.z_);
      rc.out_.z_.sub(rc.out_.z_, r.t_);
      rc.out_.z_.sub(rc.out_.z_, i);

      t.sub(v, rc.out_.x_);
      t.mul(t, l1);
      t2.mul(r.y_, j);
      t2.add(t2, t2);
      rc.out_.y_.sub(t, t2);

      rc.out_.t_.square(rc.out_.z_);

      t.add(p.y_, rc.out_.z_);
      t.square(t);
      t.sub(t, r2);
      t.sub(t, rc.out_.t_);

      t2.mul(l1, p.x_);
      t2.add(t2, t2);
      rc.a_.sub(t2, t);

      rc.c_.mul_scalar(rc.out_.z_, q.y_);
      rc.c_.add(rc.c_, rc.c_);

      rc.b_.neg(l1);
      rc.b_.mul_scalar(rc.b_, q.x_);
      rc.b_.add(rc.b_, rc.b_);

      return rc;
   }

   line_function_result_t line_function_double(const twist_point& r, const curve_point& q) {

      line_function_result_t rc{};

      // See the doubling algorithm for a=0 from "Faster Computation of the
      // Tate Pairing", http://arxiv.org/pdf/0904.0854v3.pdf
      gfp2 a{}, b{}, c{}, d{}, e{}, g{}, t{};

      a.square(r.x_);
      b.square(r.y_);
      c.square(b);

      d.add(r.x_, b);
      d.square(d);
      d.sub(d, a);
      d.sub(d, c);
      d.add(d, d);

      e.add(a, a);
      e.add(e, a);

      g.square(e);

      rc.out_.x_.sub(g, d);
      rc.out_.x_.sub(rc.out_.x_, d);

      rc.out_.z_.add(r.y_, r.z_);
      rc.out_.z_.square(rc.out_.z_);
      rc.out_.z_.sub(rc.out_.z_, b);
      rc.out_.z_.sub(rc.out_.z_, r.t_);

      rc.out_.y_.sub(d, rc.out_.x_);
      rc.out_.y_.mul(rc.out_.y_, e);

      t.add(c, c);
      t.add(t, t);
      t.add(t, t);

      rc.out_.y_.sub(rc.out_.y_, t);

      rc.out_.t_.square(rc.out_.z_);

      t.mul(e, r.t_);
      t.add(t, t);
      rc.b_.neg(t);
      rc.b_.mul_scalar(rc.b_, q.x_);

      rc.a_.add(r.x_, e);
      rc.a_.square(rc.a_);
      rc.a_.sub(rc.a_, a);
      rc.a_.sub(rc.a_, g);

      t.add(b, b);
      t.add(t, t);

      rc.a_.sub(rc.a_, t);

      rc.c_.mul(rc.out_.z_, r.t_);
      rc.c_.add(rc.c_, rc.c_);
      rc.c_.mul_scalar(rc.c_, q.y_);

      return rc;
   }

   void mul_line(gfp12& ret, const gfp2& a, const gfp2& b, const gfp2& c) {
      gfp6 a2{}, t2{}, t3{};
      gfp2 t{};

      a2.y_.set(a);
      a2.z_.set(b);
      a2.mul(a2, ret.x_);
      t3.mul_scalar(ret.y_, c);

      t.add(b, c);
      t2.y_.set(a);
      t2.z_.set(t);
      ret.x_.add(ret.x_, ret.y_);

      ret.y_.set(t3);

      ret.x_.mul(ret.x_, t2);
      ret.x_.sub(ret.x_, a2);
      ret.x_.sub(ret.x_, ret.y_);
      a2.mul_tau(a2);
      ret.y_.add(ret.y_, a2);
   }

   // sixuPlus2NAF is 6u+2 in non-adjacent form.
   std::array<int8_t, 65> six_u_plus_2_naf = {
      0, 0, 0, 1, 0, 1, 0, -1, 0, 0, 1, -1, 0, 0, 1, 0,
      0, 1, 1, 0, -1, 0, 0, 1, 0, -1, 0, 0, 0, 0, 1, 1,
      1, 0, 0, -1, 0, 0, 1, 0, 0, 0, 0, 0, -1, 0, 0, 1,
      1, 0, 0, -1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 1, 0, 1,
      1
   };

   // miller implements the Miller loop for calculating the Optimal Ate pairing.
   // See algorithm 1 from http://cryptojedi.org/papers/dclxvi-20100714.pdf
   gfp12 miller(const twist_point& q, const curve_point& p) {
      gfp12 ret{};
      ret.set_one();
      std::cout << "ret: " << ret.string() << std::endl;

      twist_point a_affine{};
      a_affine.set(q);
      a_affine.make_affine();
      std::cout << "aAffine: " << a_affine.string() << std::endl;

      curve_point b_affine{};
      b_affine.set(p);
      b_affine.make_affine();
      std::cout << "bAffine: " << b_affine.string() << std::endl;

      twist_point minus_a{}, r{};
      minus_a.neg(a_affine);
      r.set(a_affine);
      std::cout << "r: " << r.string() << std::endl;

      gfp2 r2{};
      r2.square(a_affine.y_);

      for (auto i = six_u_plus_2_naf.size()-1; i > 0; i--) {
         auto lf_result = line_function_double(r, b_affine);
         if (i != six_u_plus_2_naf.size()-1) {
            ret.square(ret);
         }

         mul_line(ret, lf_result.a_, lf_result.b_, lf_result.c_);
         r = lf_result.out_;

         switch (six_u_plus_2_naf[i-1]) {
            case 1:
               lf_result = line_function_add(r, a_affine, b_affine, r2);
               break;
            case -1:
               lf_result = line_function_add(r, minus_a, b_affine, r2);
               break;
            default:
               continue;
         }

         mul_line(ret, lf_result.a_, lf_result.b_, lf_result.c_);
         r = lf_result.out_;
      }

      // In order to calculate Q1 we have to convert q from the sextic twist
      // to the full GF(p^12) group, apply the Frobenius there, and convert
      // back.
      //
      // The twist isomorphism is (x', y') -> (xω², yω³). If we consider just
      // x for a moment, then after applying the Frobenius, we have x̄ω^(2p)
      // where x̄ is the conjugate of x. If we are going to apply the inverse
      // isomorphism we need a value with a single coefficient of ω² so we
      // rewrite this as x̄ω^(2p-2)ω². ξ⁶ = ω and, due to the construction of
      // p, 2p-2 is a multiple of six. Therefore we can rewrite as
      // x̄ξ^((p-1)/3)ω² and applying the inverse isomorphism eliminates the
      // ω².
      //
      // A similar argument can be made for the y value.

      twist_point q1{};
      q1.x_.conjugate(a_affine.x_);
      q1.x_.mul(q1.x_, constants::xi_to_p_minus_1_over_3);

      q1.y_.conjugate(a_affine.y_);
      q1.y_.mul(q1.y_, constants::xi_to_p_minus_1_over_2);
      q1.z_.set_one();
      q1.t_.set_one();

      // For Q2 we are applying the p² Frobenius. The two conjugations cancel
      // out and we are left only with the factors from the isomorphism. In
      // the case of x, we end up with a pure number which is why
      // xiToPSquaredMinus1Over3 is ∈ GF(p). With y we get a factor of -1. We
      // ignore this to end up with -Q2.

      twist_point minus_q2;
      minus_q2.x_.mul_scalar(a_affine.x_, constants::xi_to_p_squared_minus_1_over_3);
      minus_q2.y_.set(a_affine.y_);
      minus_q2.z_.set_one();
      minus_q2.t_.set_one();

      r2.square(q1.y_);
      auto lf_result = line_function_add(r, q1, b_affine, r2);
      mul_line(ret, lf_result.a_, lf_result.b_, lf_result.c_);
      r = lf_result.out_;

      r2.square(minus_q2.y_);
      lf_result = line_function_add(r, minus_q2, b_affine, r2);
      mul_line(ret, lf_result.a_, lf_result.b_, lf_result.c_);
      r = lf_result.out_;

      return ret;
   }

   // finalExponentiation computes the (p¹²-1)/Order-th power of an element of
   // GF(p¹²) to obtain an element of GT (steps 13-15 of algorithm 1 from
   // http://cryptojedi.org/papers/dclxvi-20100714.pdf)
   gfp12 final_exponentiation(const gfp12& in) {
      gfp12 t1{}, inv{}, t2{}, fp{}, fp2{}, fp3{},
              fu{}, fu2{}, fu3{}, y3{}, fu2p{}, fu3p{},
              y2{}, y0{}, y1{}, y4{}, y5{}, y6{}, t0{};

      // This is the p^6-Frobenius
      t1.x_.neg(in.x_);
      t1.y_.set(in.y_);

      inv.invert(in);
      t1.mul(t1, inv);

      t2.frobenius_p2(t1);
      t1.mul(t1, t2);

      fp.frobenius(t1);
      fp2.frobenius_p2(t1);
      fp3.frobenius(fp2);

      fu.exp(t1, constants::u);
      fu2.exp(fu, constants::u);
      fu3.exp(fu2, constants::u);

      y3.frobenius(fu);
      fu2p.frobenius(fu2);
      fu3p.frobenius(fu3);
      y2.frobenius_p2(fu2);

      y0.mul(fp, fp2);
      y0.mul(y0, fp3);

      y1.conjugate(t1);
      y5.conjugate(fu2);
      y3.conjugate(y3);
      y4.mul(fu, fu2p);
      y4.conjugate(y4);

      y6.mul(fu3, fu3p);
      y6.conjugate(y6);

      t0.square(y6);
      t0.mul(t0, y4);
      t0.mul(t0, y5);
      t1.mul(y3, y5);
      t1.mul(t1, t0);
      t0.mul(t0, y2);
      t1.square(t1);
      t1.mul(t1, t0);
      t1.square(t1);
      t0.mul(t1, y1);
      t1.mul(t1, y0);
      t0.square(t0);
      t0.mul(t0, t1);

      return t0;
   }

   gfp12 optimal_ate(const twist_point& a, const curve_point& b) {
      auto e = miller(a, b);
      auto ret = final_exponentiation(e);

      if (a.is_infinity() || b.is_infinity()) {
         ret.set_one();
      }
      return ret;
   }
}