/* Copyright 2020 CNRS-AIST JRL */

#pragma once

#include <Eigen/Core>

namespace jrl
{
namespace qp
{
// definitions
using MatrixConstRef = Eigen::Ref<const Eigen::MatrixXd>;
using MatrixRef = Eigen::Ref<Eigen::MatrixXd>;
using VectorConstRef = Eigen::Ref<const Eigen::VectorXd>;
using VectorRef = Eigen::Ref<Eigen::VectorXd>;
const Eigen::MatrixXd EmptyMatrix = Eigen::MatrixXd(0, 0);
const Eigen::VectorXd EmptyVector = Eigen::VectorXd(0);

namespace constant
{
constexpr std::uint32_t noIterationFlag = 1 << 31;
}
} // namespace qp
}