#include "thread-timer.h"

void ThreadTimer::ThreadProcessTimeout(const std::function<void()>& handler, int waitFor) { // TODO: take handle and kill instead of: take func -> run -> take handle -> kill
	// Use a std::promise to signal the thread completion
	std::promise<void> exitSignal;
	std::future<void> exitSignalFuture = exitSignal.get_future();

	// Create a thread that will execute the handler
	std::thread worker([handler, &exitSignal] {
	  handler();
	  exitSignal.set_value();  // Signal that the function has completed
	});

	// Wait for the specified timeout
	if (exitSignalFuture.wait_for(std::chrono::milliseconds(waitFor)) == std::future_status::timeout) {
		TerminateThread(worker.native_handle(), 1);
	}

	if (worker.joinable()) {
		worker.join();
	}
}

ThreadTimer::ThreadTimer(int timeoutNumber) : timeoutNumber(timeoutNumber) {}
