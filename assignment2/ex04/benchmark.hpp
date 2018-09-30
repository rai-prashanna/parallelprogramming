#ifndef lacpp_benchmark_hpp
#define lacpp_benchmark_hpp benchmark_hpp

/* a small benchmarking framework by David Klaftenegger, 2015
 * please report bugs or suggest improvements to david.klaftenegger@it.uu.se
 */

#include <atomic>
#include <chrono>
#include <random>
#include <thread>
#include <vector>

enum class worker_status {wait, work, finish};

static const int RANDOM_VALUE_RANGE_MIN = 0;
static const int RANDOM_VALUE_RANGE_MAX = 65536;

/* template is used to allow functions/functors of any signature */
template<typename Function>
void worker(unsigned int random_seed, double& ops_per_sec, std::atomic<worker_status>* status, Function fun) {
	/* set up random number generator */
	std::mt19937 engine(random_seed);
	std::uniform_int_distribution<int> uniform_dist(RANDOM_VALUE_RANGE_MIN, RANDOM_VALUE_RANGE_MAX);
	/* for time measurements */
	typedef std::chrono::high_resolution_clock clock;
	/* wait for everyone to be allowed to start */
	while(*status == worker_status::wait);
	std::chrono::time_point<clock> start_time = clock::now();
	long items = 0;
	while(*status == worker_status::work) {
		auto random = uniform_dist(engine);
		/* do specified work */
		fun(random);
		items++;
	}
	std::chrono::time_point<clock> end_time = clock::now();
	double time = std::chrono::duration<double, std::ratio<1, 1000>>(end_time - start_time).count();
	ops_per_sec = items / time;
	std::this_thread::sleep_for(std::chrono::seconds(1));	
	return;
}


template<typename Function>
void benchmark(int threadcnt, std::string identifier, Function fun) {
	/* initialize worker status */
	std::atomic<worker_status> status;
	status = worker_status::wait;

	/* spawn workers */
	std::vector<double> ops_per_second(threadcnt);
	std::vector<std::thread*> workers;
	std::random_device rd;
	for(int i = 0; i < threadcnt; i++) {
		auto seed = rd();
		auto& result = ops_per_second[i];
		auto w = new std::thread([seed, &result, &status, fun]() { worker(seed, result, &status, fun); });
		workers.push_back(w);
	};

	/* start work for 5s */
	status = worker_status::work;
	std::this_thread::sleep_for(std::chrono::seconds(5));
	status = worker_status::finish;

	/* make sure all workers terminated */
	for(auto& w : workers) {
		w->join();
		delete w;
	}
	workers.clear();

	/* compute sum of partial results */
	double result = 0.0;
	for(auto& v : ops_per_second) {
		result += v;
	}
	std::cout << identifier << u8" / threads: " << threadcnt << u8" - thousands of operations per second: " << std::fixed << result << "\n";
}

#endif // lacpp_benchmark_hpp
