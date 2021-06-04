#pragma once

#include <map>
#include <vector>

#include "linearAlgebra.hpp"

namespace Jamming {

struct Parameter {
  protected:
    unsigned m_networkSize;
    unsigned long long m_linkSize;

  public:
    Parameter() {}
    Parameter(const unsigned&, const unsigned long long&);

    const std::vector<unsigned> getNearCritical(const double&) const;
};

Parameter::Parameter(const unsigned& t_networkSize,
                     const unsigned long long& t_linkSize)
    : m_networkSize(t_networkSize),
      m_linkSize(t_linkSize) {
}

const std::vector<unsigned> Parameter::getNearCritical(const double& t_strategy) const {
    std::vector<unsigned> nearCritical = {};

    if (m_networkSize == 10000U && m_linkSize == 25000ULL) {
        if (t_strategy == 0.5) {
            nearCritical = linearAlgebra::arange(70U, 100U, 1U);
        } else if (t_strategy == 0.85) {
            nearCritical = linearAlgebra::arange(70U, 120U, 1U);
        } else if (t_strategy == 0.95) {
            nearCritical = linearAlgebra::arange(65U, 180U, 1U);
        }
    }

    return nearCritical;
}

} // namespace Jamming
