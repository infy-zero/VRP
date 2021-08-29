#include <vector>
namespace alns {
void alns::addVehicle(std::vector<std::vector<int>>& solution, int depot, int num = 1) {
	while (num--) {
		std::vector<int> newVeh = {depot, depot};
		solution.push_back(newVeh);
	}
}

} // alns