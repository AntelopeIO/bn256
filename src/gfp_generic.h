#pragma once
namespace bn256 {
   struct gfp;
   void gfp_neg(gfp& c, const gfp& a) noexcept;
   void gfp_add(gfp& c, const gfp& a, const gfp& b) noexcept;
   void gfp_sub(gfp& c, const gfp& a, const gfp& b) noexcept;
   void gfp_mul(gfp& c, const gfp& a, const gfp& b) noexcept;
}
