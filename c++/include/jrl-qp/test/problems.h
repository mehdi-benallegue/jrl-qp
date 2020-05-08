/* Copyright 2020 CNRS-AIST JRL
 */

#pragma once

#include <Eigen/Core>
#include <jrl-qp/api.h>

namespace jrlqp::test
{
  struct JRLQP_DLLAPI SeparatedFeasibilityConstraints;

  /** Representation of the feasibility problem
    *    l <= Cx <= u
    *    xl <= x <= xu
    */
  struct JRLQP_DLLAPI FeasibilityConstraints
  {
    FeasibilityConstraints() = default;
    FeasibilityConstraints(const FeasibilityConstraints&) = default;
    FeasibilityConstraints(FeasibilityConstraints&&) = default;
    FeasibilityConstraints(const SeparatedFeasibilityConstraints& cstr);
    bool wellFormed(bool noEq = false) const;

    Eigen::MatrixXd C;
    Eigen::VectorXd l;
    Eigen::VectorXd u;
    Eigen::VectorXd xl;
    Eigen::VectorXd xu;
    bool transposedMat = false;
  };

  /** Representation of the feasibility problem
    *    Ex = f
    *    l <= Cx <= u
    *    xl <= x <= xu
    * where the equality constraint are explicitely separated.
    * In this version, all bounds l_i and u_i are expected to be different
    */
  struct JRLQP_DLLAPI SeparatedFeasibilityConstraints
    : public FeasibilityConstraints
  {
    SeparatedFeasibilityConstraints() = default;
    SeparatedFeasibilityConstraints(const SeparatedFeasibilityConstraints&) = default;
    SeparatedFeasibilityConstraints(SeparatedFeasibilityConstraints&&) = default;
    SeparatedFeasibilityConstraints(const FeasibilityConstraints& feas);

    bool wellFormed() const;

    Eigen::MatrixXd E;
    Eigen::VectorXd f;
  };

  /** Representation of the Linear least-square problem
    *  min. 0.5 ||A x - b ||^2
    *  s.t. feasibility constraints
    * where the feasibility constraints are given by SeparatedFeasibilityConstraints
    * if \tparam Separated = true and FeasibilityConstraints otherwise.
    */
  template<bool Separated = false>
  struct LeastSquareProblem 
    : public std::conditional_t<Separated, 
                                SeparatedFeasibilityConstraints, 
                                FeasibilityConstraints>
  {
    bool wellFormed() const;

    using Base = std::conditional_t<Separated, SeparatedFeasibilityConstraints, FeasibilityConstraints>;
    Eigen::MatrixXd A;
    Eigen::VectorXd b;
  };

  /** Representation of the Linear least-square problem
    *  min. 0.5 x^T G x + a^T x + objCst
    *  s.t. feasibility constraints
    * where the feasibility constraints are given by SeparatedFeasibilityConstraints
    * if \tparam Separated = true and FeasibilityConstraints otherwise.
    */
  template<bool Separated = false>
  struct QPProblem 
    : public std::conditional_t<Separated, 
                                SeparatedFeasibilityConstraints, 
                                FeasibilityConstraints>
  {
    QPProblem() = default;
    QPProblem(const QPProblem&) = default;
    QPProblem(QPProblem&&) = default;
    template<bool LSSeparated>
    QPProblem(const LeastSquareProblem<LSSeparated>& ls);
    template<bool LSSeparated>
    QPProblem& operator=(const LeastSquareProblem<LSSeparated>& ls);
    bool wellFormed() const;

    using Base = std::conditional_t<Separated, SeparatedFeasibilityConstraints, FeasibilityConstraints>;
    Eigen::MatrixXd G;
    Eigen::VectorXd a;
    double objCst = 0;
  };

  template<bool Separated>
  inline bool LeastSquareProblem<Separated>::wellFormed() const
  {
    bool b1 = Base::wellFormed();
    bool b2 = transposedMat ? (A.cols() == C.rows()) : (A.cols() == C.cols());
    bool b3 = A.rows() == b.size();
    return b1 && b2 && b3;
  }

  template<bool Separated>
  template<bool LSSeparated>
  inline QPProblem<Separated>::QPProblem(const LeastSquareProblem<LSSeparated>& ls)
    : Base(ls)
    , G(ls.A.transpose()*ls.A)
    , a(-ls.A.transpose()*ls.b)
  {
  }

  template<bool Separated>
  template<bool LSSeparated>
  inline QPProblem<Separated>& QPProblem<Separated>::operator=(const LeastSquareProblem<LSSeparated>& ls)
  {
    new (this) QPProblem<Separated>(ls);
    return *this;
  }

  template<bool Separated>
  inline bool QPProblem<Separated>::wellFormed() const
  {
    bool b1 = Base::wellFormed();
    bool b2 = transposedMat ? (G.cols() == C.rows()) : (G.cols() == C.cols());
    bool b3 = G.cols() == a.size();
    return b1 && b2 && b3;
  }
}