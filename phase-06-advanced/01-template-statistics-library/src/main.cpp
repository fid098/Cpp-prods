#include <iostream>
#include <iomanip>
#include "stats.hpp"

int main() {
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "=== Template Statistics Library ===\n\n";

    std::vector<double> returns = {0.01, -0.02, 0.03, -0.01, 0.02, 0.015, -0.005, 0.025};
    Stats<double> s{returns};

    std::cout << "Returns stats (double):\n";
    std::cout << "  mean:     " << s.mean()     << "\n";
    std::cout << "  variance: " << s.variance() << "\n";
    std::cout << "  std_dev:  " << s.std_dev()  << "\n";
    std::cout << "  skewness: " << s.skewness() << "\n";
    std::cout << "  kurtosis: " << s.kurtosis() << " (excess)\n";
    std::cout << "  min/max:  " << s.min() << " / " << s.max() << "\n";

    // Same with float
    std::vector<float> float_data = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
    Stats<float> sf{float_data};
    std::cout << "\nFloat stats: mean=" << sf.mean() << " std_dev=" << sf.std_dev() << "\n";

    // Correlation
    std::vector<double> spx = {0.01, 0.02, -0.01, 0.03, -0.02};
    std::vector<double> qqq = {0.015, 0.025, -0.012, 0.035, -0.022};
    std::cout << "\nCorrelation(SPX, QQQ): " << correlation(spx, qqq) << "\n";

    return 0;
}
