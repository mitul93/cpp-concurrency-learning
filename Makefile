# ── Help ──────────────────────────────────────────────────────────────────────
.PHONY: help
help: ## Show this help message
	@grep -E '^[a-zA-Z_-]+:.*##' $(MAKEFILE_LIST) \
	  | awk 'BEGIN {FS = ":.*##"}; {printf "  \033[36m%-35s\033[0m %s\n", $$1, $$2}'

.PHONY: clean
clean: ## Remove build artifacts from build folder
	@cd build && \
		rm -rf CMakeCache.txt CMakeFiles Makefile cmake_install.cmake src

.PHONY: all
all: ## Build all examples
	@mkdir -p build
	@cd build && \
		cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_EXAMPLES=ON -DBUILD_BENCHMARKS=ON .. && \
		make -j $(nproc)

.PHONY: all_debug
all_debug: ## Build all examples with Debug build
	@mkdir -p build
	@cd build && \
		cmake -DCMAKE_BUILD_TYPE=Debug .. && \
		make -j $(nproc)

.PHONY: build_benchmark
build_benchmark: ## Build benchmark targets
	@mkdir -p build
	@cd build && \
		cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_EXAMPLES=OFF -DBUILD_BENCHMARKS=ON .. && \
		make -j $(nproc)

.PHONY: build_w_tsan
build_w_tsan: ## Build with thread sanitizer
	@mkdir -p build
	@cd build && \
		cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_EXAMPLES=ON -DENABLE_TSAN=ON -DCMAKE_BUILD_TYPE=Debug -DBUILD_BENCHMARKS=OFF .. && \
		make -j $(nproc)

.PHONY: publish_nuget_packages
publish_nuget_packages: ## Build and publish packages
	@mkdir -p build
	@cd build && \
		cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_EXAMPLES=OFF ..