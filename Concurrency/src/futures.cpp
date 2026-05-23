#include <vector>
#include <future>

std::vector<int> produce(size_t){
	// TODO: generate random numbers
}

int summarize(std::vector<std::future<std::vector<int>>>&&){
	// TODO: compute and return the sum of elements
}

int main(){
	// TODO: spawn several producers
	// TODO: collect the futures, compute and announce summary
}