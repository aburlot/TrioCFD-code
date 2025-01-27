/****************************************************************************
* Copyright (c) 2021, CEA
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
* 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
* 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*****************************************************************************/
//////////////////////////////////////////////////////////////////////////////
//
// File:        Loi_paroi_Ramstorfer.cpp
// Directory:   $TRUST_ROOT/src/Turbulence/Correlations
// Version:     /main/18
//
//////////////////////////////////////////////////////////////////////////////

#include <Loi_paroi_Ramstorfer.h>
#include <Correlation_base.h>
#include <Pb_Multiphase.h>
#include <QDM_Multiphase.h>
#include <Navier_Stokes_std.h>
#include <Domaine_Poly_base.h>
#include <TRUSTTrav.h>
#include <Cond_lim_base.h>
#include <Param.h>
#include <math.h>
#include <Nom.h>
#include <Motcle.h>
#include <TRUSTTab_parts.h>
#include <Champ_Face_base.h>

Implemente_instanciable(Loi_paroi_Ramstorfer, "Loi_paroi_Ramstorfer", Loi_paroi_base);

Sortie& Loi_paroi_Ramstorfer::printOn(Sortie& os) const
{
  return os;
}

Entree& Loi_paroi_Ramstorfer::readOn(Entree& is)
{
  return Loi_paroi_base::readOn(is);
}

void Loi_paroi_Ramstorfer::completer()
{
  Loi_paroi_base::completer();

  Pb_Multiphase *pbm = sub_type(Pb_Multiphase, pb_.valeur()) ? &ref_cast(Pb_Multiphase, pb_.valeur()) : nullptr;
  if (!pbm || pbm->nb_phases() == 1) Process::exit(que_suis_je() + " : This is a two-phase wall law!");

  DoubleTab const * d_bulles = (pb_.valeur().has_champ("diametre_bulles")) ? &pb_.valeur().get_champ("diametre_bulles").valeurs() : NULL ;
  if (!d_bulles) Process::exit(que_suis_je() + " : you must define a bubble diameter ! This is a two-phase wall law.");
}

void Loi_paroi_Ramstorfer::calc_y_plus(const DoubleTab& vit, const DoubleTab& nu_visc)
{
  Domaine_VF& domaine = ref_cast(Domaine_VF, pb_.valeur().domaine_dis().valeur());
  DoubleTab& u_t = valeurs_loi_paroi_["u_tau"], &y_p = valeurs_loi_paroi_["y_plus"];
  const DoubleTab& n_f = domaine.face_normales();
  const DoubleVect& fs = domaine.face_surfaces();
  const IntTab& f_e = domaine.face_voisins();
  const DoubleTab& d_bulles = pb_->get_champ("diametre_bulles").valeurs(),
                   & alpha  = pb_->get_champ("alpha").valeurs();

  int nf_tot = domaine.nb_faces_tot(), D = dimension, N = vit.line_size();

  DoubleTab pvit_elem(0, N * dimension);
  if (nf_tot == vit.dimension_tot(0))
    {
      const Champ_Face_base& ch = ref_cast(Champ_Face_base, pb_->equation(0).inconnue().valeur());
      domaine.domaine().creer_tableau_elements(pvit_elem);
      ch.get_elem_vector_field(pvit_elem, true);
    }

  int n=0; // pour l'instant, turbulence dans seulement une phase

  for (int f = 0 ; f < nf_tot ; f ++)
    if (Faces_a_calculer_(f,0)==1)
      {
        int c = (f_e(f,0)>=0) ? 0 : 1 ;
        if (f_e(f, (c==0) ? 1 : 0 ) >= 0) Process::exit("Error in the definition of the boundary conditions for wall laws");
        int e = f_e(f,c);

        double u_orth = 0 ;
        DoubleTrav u_parallel(D);
        if (nf_tot == vit.dimension_tot(0))
          {
            for (int d = 0; d <D ; d++) u_orth -= pvit_elem(e, N*d+n)*n_f(f,d)/fs(f); // ! n_f pointe vers la face 1 donc vers l'exterieur de l'element, d'ou le -
            for (int d = 0 ; d < D ; d++) u_parallel(d) = pvit_elem(e, N*d+n) - u_orth*(-n_f(f,d))/fs(f) ; // ! n_f pointe vers la face 1 donc vers l'exterieur de l'element, d'ou le -
          }
        else
          {
            for (int d = 0; d <D ; d++) u_orth -= vit(nf_tot + e * D+d, n)*n_f(f,d)/fs(f); // ! n_f pointe vers la face 1 donc vers l'exterieur de l'element, d'ou le -
            for (int d = 0 ; d < D ; d++) u_parallel(d) = vit(nf_tot + e * D + d, n) - u_orth*(-n_f(f,d))/fs(f) ; // ! n_f pointe vers la face 1 donc vers l'exterieur de l'element, d'ou le -
          }

        double residu = 0 ;
        for (int d = 0; d <D ; d++) residu += u_parallel(d)*n_f(f,d)/fs(f);
        if (residu > 1e-8) Process::exit("Loi_paroi_adaptative : Error in the calculation of the parallel velocity for wall laws");
        double norm_u_parallel = std::sqrt(domaine.dot(&u_parallel(0), &u_parallel(0)));

        double y_loc = (c==0) ? domaine.dist_face_elem0(f,e) : domaine.dist_face_elem1(f,e) ;
        y_p(f, n) = std::max(y_p_min_, calc_y_plus_loc(norm_u_parallel, nu_visc(e, n), y_loc, y_p(f, n), &d_bulles(e, 0), &alpha(e, 0)));
        u_t(f, n) = y_p(f, n)*nu_visc(e, n)/y_loc;
      }
}

double Loi_paroi_Ramstorfer::calc_y_plus_loc(double u_par, double nu, double y, double y_p_0, const double *d_bulles, const double *alpha)
{
  double eps = eps_y_p_;
  int step = 1, iter_max = 20;

  double y_p = y_p_0 ;
  double u_tau = nu*y_p/y;

  do
    {
      y_p = y_p - (u_plus_de_y_plus(y_p, nu, y, d_bulles, alpha) - u_par/u_tau)/(deriv_u_plus_de_y_plus(y_p, nu, y, d_bulles, alpha) + u_par/(u_tau*y_p) );
      step = step+1;
      u_tau = nu*y_p/y;
    }
  while( (std::fabs(u_plus_de_y_plus(y_p, nu, y, d_bulles, alpha) - u_par/u_tau) > eps) and (step < iter_max));

  assert ( (std::fabs(u_par/u_tau - u_plus_de_y_plus(y_p, nu, y, d_bulles, alpha)) < eps_y_p_*10) and (step < iter_max));

  return y_p;
}

double Loi_paroi_Ramstorfer::u_plus_de_y_plus(double y_p, double nu, double y, const double *d_bulles, const double *alpha) // Ramstorfer model
{
  double u_tau = y_p*y/nu;
  double log_law = std::log(y_p+limiteur_y_p)/von_karman_ + 5.1;

  double kr_p = 0;
  Pb_Multiphase pbm = ref_cast(Pb_Multiphase, pb_.valeur());
  for (int i = 1 ; i < pbm.nb_phases() ; i++) kr_p += d_bulles[i]*alpha[i];
  kr_p *= u_tau/nu ;

  return log_law - (kr_p < 11.3 ? 0 : std::log(1+C_kr_*kr_p)/von_karman_);
}

double Loi_paroi_Ramstorfer::deriv_u_plus_de_y_plus(double y_p, double nu, double y, const double *d_bulles, const double *alpha) // Ramstorfer model
{
  double u_tau = y_p*y/nu;
  double d_log_law = 1/((y_p+limiteur_y_p)*von_karman_);

  double kr = 0;
  Pb_Multiphase pbm = ref_cast(Pb_Multiphase, pb_.valeur());
  for (int i = 1 ; i < pbm.nb_phases() ; i++) kr += d_bulles[i]*alpha[i];
  double kr_p = kr * u_tau/nu ;

  return d_log_law - (kr_p < 11.3 ? 0 : (C_kr_*kr/y)/((1+C_kr_*y_p*kr/y)/von_karman_));
}
